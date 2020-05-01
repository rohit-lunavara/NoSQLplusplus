#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include "Database.h"

void time_test (long iter) {
	long count { 0 } ;
	std::ifstream input_file { "../src/bible.txt" } ;
	if ( !input_file ) {
		std::cerr << "File not found.\n" ;
		std::exit(1) ;
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
			++count ;
		}
		if ( count >= iter ) {
			break ;
		}
	}
	
	auto t1 = std::chrono::high_resolution_clock::now() ;
	
	/***************************************************/

	auto keys = str_db.get_keys() ;

	// for ( const auto& k : keys) {
	// 	std::cout << k << " : " << str_db.get(k) << "\n" ;
	// }
	str_db.get(keys) ;

	/***************************************************/

	auto t2 = std::chrono::high_resolution_clock::now() ;
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() ;
	
	std::cout << iter << " : " << duration << "\n" ;
}


int main () {
	for ( long i { 2000 } ; i <= 490000 ; i += 2000 )
		time_test(i) ;

	return 0 ;
}