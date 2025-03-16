#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "../json-develop/single_include/nlohmann/json.hpp"

class ConverterJSON {
public:
	ConverterJSON() = default;

	//Проверить наличие файла config.json
	void ConfigJsonExist();

	//Метод получения содержимого файлов
	std::vector<std::string> GetTextDocuments();

	//Метод считывает поле max_responses для определения предельного
	int GetResponsesLimit() const;

	//Метод получения запросов из файла requests.json
	std::vector<std::string> GetRequests();

	//Положить в файл answers.json результаты поисковых запросов
	void putAnswers(std::vector<std::vector<std::pair<int, float>>>
		answers);
private:
	nlohmann::json objJson;
	std::fstream fileInput;
	int responsesLimit = 5;
};
