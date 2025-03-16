#include "../include/ConverterJSON.h"
#include "../include/InvertedIndex.h"
#include "../include/SearchEngine.h"
#include <exception>

int main()
{
	ConverterJSON converterJson;
	InvertedIndex invertedIndex;

	try
	{
		converterJson.ConfigJsonExist();
	}
	catch (const std::exception&)
	{
		std::cerr << "Config.json does not exist\n";
	}

	std::vector<std::string> fileList = converterJson.GetTextDocuments();
	invertedIndex.UpdateDocumentBase(fileList);

	SearchServer searchServer(invertedIndex);
	std::vector<std::string> queryList = converterJson.GetRequests();
	std::vector <std::vector<RelativeIndex>> relativeIndexList = searchServer.search(queryList, converterJson.GetResponsesLimit());

	std::vector<std::vector<std::pair<int, float>>> answers;

	for (auto& relIndex : relativeIndexList)
	{
		std::pair<int, float> _result;
		std::vector<std::pair<int, float>> _answers;

		for (auto& it : relIndex)
		{
			_result = std::make_pair(it.doc_id, it.rank);
			_answers.push_back(_result);
		}
		answers.push_back(_answers);
	}
	converterJson.putAnswers(answers);
}
