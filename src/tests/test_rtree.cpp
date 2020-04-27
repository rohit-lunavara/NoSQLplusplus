#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
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


int main()
{
        // test1();
        // test2();
        // test3();
        
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
                float latitude;
                float longitude;

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
        }
        
        cout << rtree.search({40.6943,-73.9249}, 100) << "\n";

        return 0;
}