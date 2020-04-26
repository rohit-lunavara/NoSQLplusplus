#include <iostream>
#include "SphericalRTree.h"
#include "GeoCoordinate.h"

using namespace std;
using namespace RTree;
using namespace Geography;

int main()
{
        SphericalRTree<string> rtree;
        rtree.insert("New York", {40.7128_N, 74.0060_E}, 0.);

        return 0;
}