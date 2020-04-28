#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include "SphericalRTree.h"
#include "GeoCoordinate.h"

using namespace std;
using namespace RTree;
using namespace Geography;

void test1()
{
        GeoCoordinate a {40.7128_N, 74.0060_E};
        GeoCoordinate b {51.5074_N, 0.1278_W};
        
        GeoCoordinate c;
        double radius;

        combine_sphere(c, radius, a, 0., b, 0.);

        cout << c << ", " << radius << endl;
}

void test2()
{
        GeoCoordinate a {0., 170._W};
        GeoCoordinate b {0., 10._E};
        
        GeoCoordinate c;
        double radius;

        combine_sphere(c, radius, a, 100000., b, 2000.);

        cout << c << ", " << radius << endl;        
}

void test3()
{
        GeoCoordinate a {40.7128_N, 74.0060_E};
        GeoCoordinate b {41.7128_N, 72.0060_E};

        double radius = 1.;

        combine_sphere(a, radius, a, radius, b, 5000.);

        cout << a << ", " << radius << endl;        
}

void test4()
{
       int ncities = 0;
        SphericalRTree<string> rtree;
        cout << "begin" << endl;
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

                // cout << cityname.str() << endl;
                // cout << latitude << endl;
                // cout << longitude << endl;
                rtree.insert(cityname.str(), {latitude, longitude}, 0.);
                ++ncities;
        }
        
        
        cout << rtree.search({40.6943,-73.9249}, 15) << "\n";
        cout << "total us cities: " << ncities << "\n";
        cout << distance({40.7245,-74.1725}, {40.6943,-73.9249}) << "\n";
}


void test5()
{
        int ncities = 0;
        SphericalRTree<string> rtree;
        cout << "begin" << endl;
        ifstream fin("uscities.txt");

        unordered_map<string, GeoCoordinate> records;

        while (fin)
        {
                if (ncities >= 10)
                {
                        break;
                }

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

                records[cityname.str()] = {latitude, longitude};
                rtree.insert(cityname.str(), {latitude, longitude}, 0.);
                ++ncities;
        }

        for (const auto& x : records)
        {
                cout << x.first << ", " << x.second << "\n";
        }

        rtree.print_tree();
        cout << "search result=======================\n";
        rtree.search({47.9038_N, 121.986_W}, 500.);
        rtree.remove("Roslyn, WA", records["Roslyn, WA"], 0.);
        rtree.print_tree();
        cout << "search result=======================\n";
        rtree.search({47.9038_N, 121.986_W}, 500.);
        rtree.remove("South Creek, WA", records["South Creek, WA"], 0.);
        rtree.print_tree();
        rtree.remove("Gig Harbor, WA", records["Gig Harbor, WA"], 0.);
        rtree.print_tree();
        rtree.remove("Lake Cassidy, WA", records["Lake Cassidy, WA"], 0.);
        rtree.print_tree();
        rtree.remove("Jamestown, WA", records["Jamestown, WA"], 0.);
        rtree.print_tree();
        rtree.remove("Chain Lake, WA", records["Chain Lake, WA"], 0.);
         rtree.print_tree();
        rtree.remove("Curlew Lake, WA", records["Curlew Lake, WA"], 0.);
        rtree.print_tree();
        rtree.remove("Sprague, WA", records["Sprague, WA"], 0.);
        rtree.remove("Three Lakes, WA", records["Three Lakes, WA"], 0.);
        rtree.remove("Tenino, WA", records["Tenino, WA"], 0.);
        cout << "search result=======================\n";
        rtree.search({47.9038_N, 121.986_W}, 500.);
}

void test6()
{
        int ncities = 0;
        SphericalRTree<string> rtree;
        cout << "begin" << endl;
        ifstream fin("uscities.txt");

        unordered_map<string, GeoCoordinate> records;

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

                records[cityname.str()] = {latitude, longitude};
                rtree.insert(cityname.str(), {latitude, longitude}, 0.);
                ++ncities;
        }
        

        cout << "search result=======================\n";
        cout << "cities: " << rtree.search({47.9038_N, 121.986_W}, 50000.) << endl;

        int rmcities = 0;
        for (const auto& x : records)
        {
                if (rmcities >= 20000) break;
                try {
                        rtree.remove(x.first, x.second, 0.);
                } catch (const exception& e)
                {
                        cout << x.first << endl;
                        cout << e.what() << endl;
                }

                ++rmcities;
               
        }
        rtree.print_tree();
        cout << "search result=======================\n";
        cout << "cities: " << rtree.search({47.9038_N, 121.986_W}, 50000.) << endl;     
}

int main()
{
        // test1();
        // test2();
        // test3();
        // test4();
        // test5();
        test6();

        return 0;
}