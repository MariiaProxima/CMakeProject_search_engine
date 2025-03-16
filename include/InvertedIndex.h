#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <mutex>
#include <thread>
#include <sstream>

struct Entry {
	size_t doc_id, count;

	//ќператор дл€ проведени€ тестовых сценариев
	bool operator ==(const Entry& other) const {
		return (doc_id == other.doc_id &&
			count == other.count);
	}
};
class InvertedIndex {
public:
	InvertedIndex() = default;

	//ќбновить или заполнить базу документов, по которой будем совершать
	void UpdateDocumentBase(std::vector<std::string> input_docs);

	//—трока с текстом из документа и его пор€дковый номер
	void PutFreqDictionary(std::string input_words, int i);

	//„астотный словарь
	std::map<std::string, std::vector<Entry>> GetFreqDictionary();

	//ћетод определ€ет количество вхождений слова word в загруженной базе документов
	std::vector<Entry> GetWordCount(const std::string& word);
private:
	std::vector<std::string> docs; // список содержимого документов
	std::map<std::string, std::vector<Entry>> freq_dictionary; // частотный словарь
};
