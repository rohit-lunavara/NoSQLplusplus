#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include "DataBase.h"

using namespace std;

void test1()
{
	using namespace Geography;
	DataBase<string, GeoCoordinate> db;
        cout << db.set("New York", {30.7128_N, 24.0060_W}, {"XX"}) << '\n';
	cout << db.set("New York", {50.7128_N, 74.0060_W}) << '\n';
        cout << db.set("New York", {40.7128_N, 74.0060_W}, {"XX"}) << '\n';
        cout << db.set("New York", {10.7128_N, 24.0060_W}, {"NX"}) << '\n';
	cout << db.set("London", {51.5074_N, 0.1278_W}) << '\n';

	cout << db.get("New York") << '\n';
	cout << db.get("London") << '\n';
	
	cout << "distance: " << db.distance("New York", "London") << "km\n";
}

void test2()
{
	using namespace Geography;
	DataBase<string, GeoCoordinate> db;
	cout << db.set("New York", {40.7128_N, 74.0060_W}, {}) << '\n';
	cout << db.set("London", {51.5074_N, 0.1278_W}, {}) << '\n';

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

                db.set(cityname.str(), {latitude, longitude}, { });
                ++ncities;
        }

        
        set<string> results = db.radius("New York, NY", 15.);
        cout << "search result=======================\n";
        for (const auto& r : results)
        {
                cout << r << "\n";
        }

        db.remove("Union City, NJ");

        
        set<string> results2 = db.radius("New York, NY", 15.);
        cout << "search result=======================\n";
        for (const auto& r : results2)
        {
                cout << r << "\n";
        }
}

void test_interactive()
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

                db.set(cityname.str(), {latitude, longitude}, { });
                ++ncities;
        }

        cout << "set all cities successfully!\n";
        while(true)
        {
                string linecommand;
                getline(cin, linecommand);
                istringstream ss(linecommand);
        
                string opt;
                getline(ss, opt, ' ');
                if (opt == "radius")
                {
                        string cityname, state;
                        getline(ss, cityname, ',');
                        getline(ss, state, ' ');
                        getline(ss, state, ' ');
                       

                        string radius;
                        getline(ss, radius, '\n');
                        double rad = stof(radius);

                        cout << "[query]" << "cityname: " << cityname
                                << ", state: " << state
                                << ", radius: " << rad << "\n";
                        set<string> results = db.radius(cityname + ", " + state, rad);
                        cout << "[results]\n";
                        for (const auto& r : results)
                        {
                                cout << r << ", distance: "
                                        << db.distance(r, cityname + ", " + state) 
                                        << "\n";
                        }    
                }
                else if (opt == "rename")
                {
                        string cityname1, state1;
                        getline(ss, cityname1, ',');
                        getline(ss, state1, ' ');
                        getline(ss, state1, ' ');

                        string cityname2, state2;
                        getline(ss, cityname2, ',');
                        getline(ss, state2, ' ');
                        getline(ss, state2, ' ');    

                        string city1 = cityname1 + ", " + state1;
                        string city2 = cityname2 + ", " + state2;

                        cout << "[command] rename " << city1 << " as "
                                << city2 << "\n";

                        if (db.rename(city1, city2))
                        {
                                cout << "successful\n";
                        }
                        else
                        {
                                cout << "failed\n";
                        }
                }
                else if (opt == "exit")
                {
                        break;
                }
                else
                {
                        cout << "[command] not supported\n";
                }

        }
}

// 1. write down why using data type
// 2. dataset bigger: cities, locations 

int main()
{
	// test1();
	// test2();
        // test3();

	// this does not compile
	// DataBase<int, int> db;
	// cout << db.set(1, 2) << '\n';
	// cout << db.get(1) << '\n';

	// we need to specialize a class each key type
	// DataBase<SupportedKey, string> db;

        test_interactive();

	return 0;
}