#include <iostream>
#include <fstream>
#include <sstream>
#include "DataBase.h"
#include "DataBase_Errors.h"

int main () {
	using std::cout ;
	using std::cerr ;
	DataBase<std::string, std::string> db ;

	cout << std::boolalpha ;
	
	// Set
	cout << "------------------------------\n" ;
	cout << db.set("test", "value") << "\n" ;
	
	// Get
	cout << "------------------------------\n" ;
	try {
		cout << db.get("fail") << "\n" ;
	}
	catch (KeyNotFoundException& er) {
		cerr << er.what() << "\n" ;
	}

	// Getset
	cout << "------------------------------\n" ;
	cout << db.getset("test", "new_value") << "\n" ;
	cout << db.get("test") << "\n" ;

	// Remove
	cout << "------------------------------\n" ;
	db.set("hello", "world") ;
	if ( db.remove("hello") ) {
		cout << "\"hello\" removed.\n" ;
	}
	try {
		cout << db.get("hello") << "\n" ;
	}
	catch (KeyNotFoundException& er) {
		cerr << er.what() << "\n" ; 
	}

	// Rename
	cout << "------------------------------\n" ;
	cout << db.get("test") << "\n" ;
	cout << db.rename("test", "better_test") << "\n" ;
	cout << db.get("better_test") << "\n" ;
	try {
		cout << db.get("test") << "\n" ;
	}
	catch (KeyNotFoundException& er) {
		cerr << er.what() << "\n" ;
	}
	cout << db.rename("beter_test", "even_better_test") << "\n" ;

	// Exist
	cout << "------------------------------\n" ;
	cout << db.exist("test") << "\n" ;
	cout << db.exist("better_test") << "\n" ;
	cout << db.exist("hello") << "\n" ;

	// Append
	cout << "------------------------------\n" ;
	cout << db.append("better_test", "+some_other_value") << "\n" ;
	cout << db.get("better_test") << "\n" ;
	cout << db.append("new_test", "completely_new_value") << "\n" ;
	cout << db.get("new_test") << "\n" ;

	// Getrange
	cout << "------------------------------\n" ;
	cout << db.getrange("new_test", 1, 5) << "\n" ;
	cout << db.getrange("new_test", -10, -5) << "\n" ;
	cout << db.getrange("new_test", 1, -5) << "\n" ;
	cout << db.getrange("new_test", -10, 0) << "\n" ;

	// Setrange
	cout << "------------------------------\n" ;
	cout << db.setrange("new", 8, "+world") << "\n" ;
	cout << db.get("new") << "\n" ;
	cout << db.setrange("new", 30, "+world") << "\n" ;
	cout << db.get("new") << "\n" ;
	cout << db.setrange("new", 5, "+world") << "\n" ;
	cout << db.get("new") << "\n" ;

	// Strlen
	cout << "------------------------------\n" ;
	cout << db.size("new") << " " << db.get("new") << "\n" ;
	cout << db.size("new_test") << " " << db.get("new_test") << "\n" ;
	cout << db.size("better_test") << " " << db.get("better_test") << "\n" ;

	// Mset
	// cout << "------------------------------\n" ;
	// std::map<std::string, std::string> nums {
	// 	{ "1", "one" },
	// 	{ "2", "two" },
	// 	{ "3", "three" },
	// 	{ "4", "four" }
	// } ;
	// auto num_res = db.set(nums) ;
	// int all_set { 0 } ;
	// for ( const auto& v : num_res ) {
	// 	if ( v ) {
	// 		all_set += 1 ;
	// 	}
	// }
	// if ( all_set == num_res.size() - 1 ) {
	// 	cout << "All numbers set.\n" ;
	// }

	// Mget
	cout << "------------------------------\n" ;
	std::vector<std::string> res ;
	try {
		res = db.get({"new", "new_test", "better_test"}) ;	
	}
	catch (KeyNotFoundException& er) {
		std::cerr << er.what() << "\n" ;
	}
	for ( const auto& val : res ) {
		cout << val << "\n" ;
	}

	// Mgetset
	// cout << "------------------------------\n" ;

	// cout << "------------------------------\n" ;
	// cout << "------------------------------\n" ;
	// cout << "------------------------------\n" ;
	// cout << "------------------------------\n" ;
	// cout << "------------------------------\n" ;

	
	cout << "------------------------------\n" ;
	return 0 ;
}