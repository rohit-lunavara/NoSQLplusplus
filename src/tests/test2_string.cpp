#include <iostream>
#include <fstream>
#include <sstream>
#include "DataBase.h"

int main () {
	std::ifstream input_file { "../src/book.txt" } ;
	if ( !input_file ) {
		std::cerr << "File not found.\n" ;
		return 1 ;
	}

	// Stores every line of occurence for every word
	DataBase<std::string, std::string> str_db ;
	std::string line ;
	while ( !input_file.eof() ) {
		std::getline(input_file, line) ;
		// Removing punctuation
		line.erase(std::remove_if(line.begin(), line.end(), ispunct), line.end()) ;
		// Making all characters lowercase
		std::transform(line.begin(), line.end(), line.begin(), ::tolower) ;

		std::istringstream iss { line } ;
		std::string word ;
		while (iss >> word) {
			str_db.append(word, line + "\n") ;
		}
	}

	// All unique words in the text
	std::cout << "---------------------------------------------------------------------------\n" ;
	std::cout << "Unique words in the text :\n" ;
	int count { 0 } ;
	auto ks = str_db.get_keys() ;
	for ( const auto& k : ks ) {
		std::cout << k << " " ;
		count++ ;
	}
	std::cout << "\n" ;
	std::cout << "---------------------------------------------------------------------------\n" ;

	// Count of unique words in the text
	std::cout << "---------------------------------------------------------------------------\n" ;
	std::cout << "Count of unique words in the text : " << count << "\n" ;
	std::cout << "---------------------------------------------------------------------------\n" ;

	// Unique words in the text
	std::cout << "---------------------------------------------------------------------------\n" ;
	std::cout << "Unique words in the text and the number of occurences :\n" ;
	for ( const auto& k : ks ) {
		auto v = str_db.get(k) ;
		std::cout << k << " : " << std::count(v.begin(), v.end(), '\n') << "\n" ;
	}
	std::cout << "\n" ;
	std::cout << "---------------------------------------------------------------------------\n" ;

	// Line of occurence for specific words from the text
	std::cout << "---------------------------------------------------------------------------\n" ;
	std::cout << "Line of occurence for specific words :\n" ;
	std::cout << "---------------------------------------------------------------------------\n" ;
	std::cout << "faithless :\n" << str_db.get("faithless") << "\n" ;
	std::cout << "---------------------------------------------------------------------------\n" ;
	std::cout << "amazing :\n" << str_db.get("amazing") << "\n" ;
	std::cout << "---------------------------------------------------------------------------\n" ;
	std::cout << "indulgently :\n" << str_db.get("indulgently") << "\n" ;
	std::cout << "---------------------------------------------------------------------------\n" ;

	// Checking if a specific word exists in the text
	std::cout << "---------------------------------------------------------------------------\n" ;
	std::cout << "Check if specific word exists in the text : \n" ;
	std::cout << "---------------------------------------------------------------------------\n" ;
	try {
		std::cout << "imparted :\n" << str_db.get("imparted") << "\n" ;
		std::cout << "---------------------------------------------------------------------------\n" ;
		std::cout << "rural :\n" << str_db.get("rural") << "\n" ;
		std::cout << "---------------------------------------------------------------------------\n" ;
		std::cout << "brewery :\n" << str_db.get("brewery") << "\n" ;
		std::cout << "---------------------------------------------------------------------------\n" ;
		std::cout << "aptitude :\n" << str_db.get("aptitude") << "\n" ;
		std::cout << "---------------------------------------------------------------------------\n" ;
	}
	catch (KeyNotFoundException& er) {
		std::cerr << er.what() << "\n" ;
		std::cout << "---------------------------------------------------------------------------\n" ;
	}

	// Using exist instead of exceptions to check if a key exists and then retrieving it
	std::vector<std::string> rare_words { "imparted", "rural", "brewery", "aptitude" } ;
	std::cout << "---------------------------------------------------------------------------\n" ;
	std::cout << "Using exist() instead of exception handling to check for the word in the text :\n" ;
	std::cout << "---------------------------------------------------------------------------\n" ;
	for ( const auto& word : rare_words ) {
		if ( str_db.exist(word) ) {
			std::cout << "\"" <<  word << "\" :\n" << str_db.get(word) << "\n" ;
		}
		else {
			std::cout << "\"" <<  word << "\" not found in the text.\n" ;
		}
		std::cout << "---------------------------------------------------------------------------\n" ;
	}

	// Check the length
	std::cout << "---------------------------------------------------------------------------\n" ;
	std::cout << "Number of characters in each line for unique words in the text :\n" ;
	for ( const auto& k : ks ) {
		auto v_len = str_db.size(k) ;
		std::cout << k << " : " << v_len << "\n" ;
	}
	std::cout << "---------------------------------------------------------------------------\n" ;

	// Up to first 40 characters for each key
	std::cout << "---------------------------------------------------------------------------\n" ;
	int lim { 40 } ;
	std::cout << "Up to first " << lim << " characters of all lines for unique words in the text :\n" ;
	for ( const auto& k : ks ) {
		auto v_len = str_db.getrange(k, 0, lim) ;
		std::cout << k << " : " << v_len << "\n" ;
	}
	std::cout << "---------------------------------------------------------------------------\n" ;

	// Removing words from text with few occurences
	std::cout << "---------------------------------------------------------------------------\n" ;
	std::cout << "Removing words from text with less than " << lim << " occurences :\n" ;
	for ( const auto& k : ks ) {
		auto v = str_db.get(k) ;
		if ( std::count(v.begin(), v.end(), '\n') < lim ) {
			std::cout << "Removing \"" << k <<  "\".\n" ;
			str_db.remove(k) ;
		}
	}
	std::cout << "\n" ;
	std::cout << "---------------------------------------------------------------------------\n" ;

	return 0 ;
}