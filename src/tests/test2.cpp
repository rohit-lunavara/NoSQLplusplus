#include <iostream>
#include <fstream>
#include <sstream>
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

void test2()
{
	using namespace Geography;
	DataBase<string, GeoCoordinate> db;
	cout << db.set("New York", {40.7128_N, 74.0060_E}) << '\n';
	cout << db.set("London", {51.5074_N, 0.1278_W}) << '\n';

	cout << db.get("New York") << '\n';
	cout << db.get("London") << '\n';
	
	cout << "distance: " << db.distance("New York", "London") << "km\n";
}

void test3()
{
        using namespace Geography;
        int ncities = 0;
        DataBase<string, GeoCoordinate> db;

        ifstream fin("uscities.txt");
        while (fin)
        {
                string s;
                if (!getline(fin, s))
                {
                        break;
                }

                istringstream ss(s);
                int index = 0;
                stringstream cityname;
                float latitude {0.};
                float longitude {0.};

                while(ss)
                {
                        string entry;
                        if (!getline(ss, entry, ','))
                        {
                                break;
                        }
                        
                        if (index == 0)
                        {
                                cityname << entry << ", ";
                        }
                        else if (index == 1)
                        {
                                cityname << entry;
                        }
                        else if (index == 2)
                        {
                                latitude =  stof(entry);
                        }
                        else if (index == 3)
                        {
                                longitude = stof(entry);
                        }

                        ++index;
                }

                db.set(cityname.str(), {latitude, longitude});
                ++ncities;
        }

        set<string> results;
        db.radius(results, "New York, NY", 15.);
        for (const auto& r : results)
        {
                cout << r << "\n";
        }

        db.del("Union City, NJ");
        set<string> results2;
        db.radius(results2, "New York, NY", 15.);
        for (const auto& r : results2)
        {
                cout << r << "\n";
        }
}

// 1. write down why using data type
// 2. dataset bigger: cities, locations 

int main()
{
	// test1();
	// test2();
        test3();
	// this does not compile
	// DataBase<int, int> db;
	// cout << db.set(1, 2) << '\n';
	// cout << db.get(1) << '\n';

	// we need to specialize a class each key type
	// DataBase<SupportedKey, string> db;

	return 0;
}