#include <iostream>
#include <memory>
#include "DataBase.h"

using namespace std;

void test1()
{
	DataBase<string, string> db;
	cout << db.set("hello", "world", 'n') << '\n';
	cout << db.set("hello", "new", 'n') << '\n';
	cout << db.get("hello") << '\n';

	// this does not compile because distance is not declared for string
	// cout << db.distance("hello", "hello") << '\n';
}


int main()
{
	DataBase<string, GeoCoordinate> db;
	cout << db.set("New York", {40.7128_N, 74.0060_E}) << '\n';
	cout << db.set("London", {51.5074_N, 0.1278_W}) << '\n';

	cout << db.get("New York") << '\n';
	cout << db.get("London") << '\n';
	
	cout << "distance: " << db.distance("New York", "London") << "km\n";

	// this does not compile
	// DataBase<int, int> db;
	// cout << db.set(1, 2) << '\n';
	// cout << db.get(1) << '\n';

	// we need to specialize a class each key type
	// DataBase<SupportedKey, string> db;

	return 0;
}