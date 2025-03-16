#include "../include/InvertedIndex.h"

std::mutex dateAdd;

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs)
{
	std::vector<std::thread> threadUpdate;
	for (int i = 0; i < input_docs.size(); ++i)
	{
		std::fstream file;
		file.open(input_docs[i]);
		if (!file)
		{
			std::cerr << "File " << input_docs[i] << " is not found\n";
			continue;
		}
		std::string words;
		std::getline(file, words);
		docs.push_back(words);
		file.close();

		threadUpdate.emplace_back(&InvertedIndex::PutFreqDictionary, this, words, i);
	}
	for (auto& i : threadUpdate)
	{
		i.join();
	}
}

void InvertedIndex::PutFreqDictionary(std::string input_words, int i)
{
	Entry entry{};
	std::string word;
	std::istringstream words(input_words);
	int words_count = 0;

	while (std::getline(words, word, ' '))
	{
		std::vector<Entry> entry_vector;
		std::lock_guard<std::mutex> guard(dateAdd);

		if (word.length() > 100)
		{
			std::cerr << "The word has an incorrect size in file " << i << "\n";
			continue;
		}
		if (words_count > 1000)
		{
			std::cerr << "There are more than 1000 words in file " << i << "\n";
			break;;
		}
		if (!freq_dictionary.count(word))
		{
			entry.doc_id = i;
			entry.count = 1;
			entry_vector.push_back(entry);
			freq_dictionary.insert({ word, entry_vector });
		}
		else
		{
			auto it = freq_dictionary.find(word);
			bool found = false;
			for (auto& j : it->second)
			{
				if (j.doc_id == i)
				{
					++j.count;
					found = true;
					break;
				}
			}
			if (!found)
			{
				entry.doc_id = i;
				entry.count = 1;
				it->second.push_back(entry);
			}
		}
		++words_count;
	}
}

std::map<std::string, std::vector<Entry>> InvertedIndex::GetFreqDictionary()
{
	return freq_dictionary;
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word)
{
	auto it = freq_dictionary.find(word);
	if (freq_dictionary.count(word))
	{
		return it->second;
	}
	else
	{
		std::vector<Entry> empty_vector;
		return empty_vector;
	}
}
