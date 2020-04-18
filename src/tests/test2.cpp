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
	return 0;
}