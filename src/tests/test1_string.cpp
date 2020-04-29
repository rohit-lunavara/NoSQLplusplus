#include <iostream>
#include <fstream>
#include <sstream>
#include "DataBase.h"
#include "DataBase_Errors.h"

int main () {
	using std::cout ;
	DataBase<std::string, std::string> db ;

	cout << std::boolalpha ;
	cout << db.set("test", "value") << "\n" ;
	// cout << db.get("fail") << "\n" ;
	return 0 ;
}