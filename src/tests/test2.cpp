#include <iostream>
#include <memory>
#include "DataBase.h"

using namespace std;

int main()
{
	DataBase<string, string> db;
	cout << db.set("hello", "world", 'n') << '\n';
	cout << db.set("hello", "new", 'n') << '\n';
	cout << db.get("hello") << '\n';

	// this does not compile
	// DataBase<int, int> db;
	// cout << db.set(1, 2) << '\n';
	// cout << db.get(1) << '\n';

	// we need to specialize a class each key type
	// DataBase<SupportedKey, string> db;

	return 0;
}