#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include "InvertedIndex.h"

struct RelativeIndex {
	size_t doc_id;
	float rank;
	bool operator ==(const RelativeIndex& other) const {
		return (doc_id == other.doc_id && rank == other.rank);
	}
};
class SearchServer {
public:
	/*В конструктор класса передаётся ссылка на класс
	InvertedIndex, чтобы SearchServer мог узнать частоту слов встречаемых
	в запросе
	*/
	SearchServer(InvertedIndex& idx) : _index(idx) { };

	//Строка с одним запросом, его номером и максимальным количеством ответов на него
	void Request(const std::string& request, int i, int limits);

	/* Метод обработки поисковых запросов возвращает отсортированный список
	релевантных ответов для заданных запросов
	*/
	std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input, int limit);
private:
	InvertedIndex _index;
	std::vector<std::vector<RelativeIndex>> search_result;
};
