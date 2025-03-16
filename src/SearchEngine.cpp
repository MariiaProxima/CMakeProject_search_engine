#include "../include/SearchEngine.h"

std::mutex date_update;

void SearchServer::Request(const std::string& request, int i, int limit)
{
	RelativeIndex relativeIndex{};
	std::vector<RelativeIndex> result;

	InvertedIndex invertedIndex;
	invertedIndex.PutFreqDictionary(request, i);

	std::map<std::string, std::vector<Entry>> maps_freq_dictionary = invertedIndex.GetFreqDictionary();
	std::map<std::string, size_t> words_count;
	int count = 0;
	for (const auto& it : maps_freq_dictionary)
	{
		words_count.insert({ it.first, it.second[0].count });
		count += it.second[0].count;
	}
	if (count > 10)
	{
		std::cerr << "Request " << i << " has incorrect size\n";
		words_count.clear();
	}
	std::multimap<size_t, std::string> wordsCount;
	for (const auto& word : words_count)
	{
		wordsCount.insert({ word.second, word.first });
	}
	std::set<size_t> doc_id_list;
	std::set<size_t> ::iterator it;

	for (const auto& one_word : wordsCount)
	{
		if (_index.GetWordCount(one_word.second).empty())
		{
			continue;
		}
		for (int j = 0; j < _index.GetWordCount(one_word.second).size(); ++j)
		{
			doc_id_list.insert(_index.GetWordCount(one_word.second)[j].doc_id);
		}
		std::lock_guard<std::mutex> guard(date_update);
		if (doc_id_list.empty())
		{
			search_result[i] = result;
		}
		else
		{
			float max_abs_relative = 0;
			for (it = doc_id_list.begin(); it != doc_id_list.end(); ++it)
			{
				relativeIndex.rank = 0;
				relativeIndex.doc_id = *it;

				for (const auto& word : wordsCount)
				{
					for (int k = 0; k < _index.GetWordCount(word.second).size(); ++k)
					{
						if (_index.GetWordCount(word.second)[k].doc_id == relativeIndex.doc_id)
						{
							relativeIndex.rank += (float)_index.GetWordCount(word.second)[k].count;
						}
					}
				}
				if (relativeIndex.rank > max_abs_relative)
				{
					max_abs_relative = relativeIndex.rank;
				}
				result.push_back(relativeIndex);
			}
			for (int i = 0; i < result.size(); ++i)
			{
				for (int j = 0; j < result.size() - 1 - i; ++j)
				{
					if (result[j].rank < result[j + 1].rank)
					{
						std::swap(result[j], result[j + 1]);
					}
				}
			}
			auto begin = result.cbegin();
			auto end = result.cend();
			if (result.size() >= limit)
			{
				result.erase(begin + limit, end);
			}
			for (auto& k : result)
			{
				k.rank /= max_abs_relative;
			}
			search_result[i] = result;
		}
	}

}

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input, int limit)
{
	std::vector<std::thread> thread_search;
	if (queries_input.size() > 1000)
	{
		std::cerr << "There are more than 1000 requests\n";
		search_result.resize(1000);
	}
	else
	{
		search_result.resize(queries_input.size());
	}
	for (int i = 0; i < queries_input.size() && queries_input.size() != 1000; ++i)
	{
		thread_search.emplace_back(&SearchServer::Request, this, std::ref(queries_input[i]), i, limit);
	}
	for (auto& i : thread_search)
	{
		i.join();
	}
	return search_result;
}
