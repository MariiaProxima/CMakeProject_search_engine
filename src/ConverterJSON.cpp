#include "../include/ConverterJSON.h"

void ConverterJSON::ConfigJsonExist()
{
	fileInput.open("config.json");
	if (!fileInput)
	{
		std::cerr << "There is no 'config.json' file\n";
	}
	fileInput >> objJson;
	fileInput.close();
	if (!objJson.count("config"))
	{
		std::cerr << "'config.json' is empty\n";
	}
	if (objJson["config"].count("max_responses"))
	{
		responsesLimit = objJson["config"]["max_responses"];
	}
	std::cout << objJson["config"]["name"] << "\nVersion: " << objJson["config"]["version"] << "\n";
}

//Преобразование списка файлов из JSON файла в вектор
std::vector<std::string> ConverterJSON::GetTextDocuments()
{
	std::vector<std::string> fileList;
	for (const auto& file : objJson["files"])
	{
		fileList.push_back(file);
	}
	return fileList;
}

int ConverterJSON::GetResponsesLimit() const
{
	return responsesLimit;
}

//Преобразование списка запросов из JSON файла в вектор
std::vector<std::string> ConverterJSON::GetRequests()
{
	std::vector<std::string> queryList;
	objJson.clear();
	fileInput.open("requests.json");
	if (fileInput.is_open())
	{
		fileInput >> objJson;
		fileInput.close();
		for (const auto& request : objJson["requests"])
		{
			queryList.push_back(request);
		}
	}
	else
	{
		std::cerr << "the file 'requests.json' is not found\n";
	}
	return queryList;
}

//Преобразование вектора с результатами поиска в JSON файл
void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>> answers)
{
	fileInput.open("answers.json", std::ios::out);
	objJson = { {"Answers: ", {}} };
	for (int i = 0; i < answers.size(); ++i)
	{
		objJson["Answer:"]["request" + std::to_string(i) + ":"];
		if (answers[i].empty())
		{
			objJson["Answer:"]["request" + std::to_string(i) + ":"] = { {"result:" , "false"} };
			continue;
		}
		else
		{
			objJson["Answer:"]["request" + std::to_string(i) + ":"] = { {"result:" , "true"} };
		}
		for (int j = 0; j < answers[i].size(); ++j)
		{
			objJson["Answer:"]["request" + std::to_string(i) + ":"]["relevance:"].push_back({ {"docid:",
				answers[i][j].first}, {"rank:", ceil(answers[i][j].second * 1000) / 1000} });
		}
	}
	fileInput << objJson;
	fileInput.close();
}
