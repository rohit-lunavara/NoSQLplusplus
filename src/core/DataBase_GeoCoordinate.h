#include "HashTable.h"
#include "GeoCoordinate.h"
#include "SphericalRTree.h"
#include <set>

template <>
class DataBase<std::string, Geography::GeoCoordinate>
{
private:
        RTree::SphericalRTree<std::string> rtree_;
        HashTable<std::string, Geography::GeoCoordinate> table_;
public:
        const Geography::GeoCoordinate& get(const std::string& key);
        bool set(const std::string& key, const Geography::GeoCoordinate& value...);
        bool del(const std::string& key);
        double distance(const std::string& key1, const std::string& key2);
        std::set<std::string> radius(const std::string& key, double rad);
};


// O (logN)
bool DataBase<std::string, Geography::GeoCoordinate>::set(
        const std::string& key, 
        const Geography::GeoCoordinate& value...)
{
        rtree_.insert(key, value, 0.);
        table_[key] = value;
        return true;
}

// O (1)
const Geography::GeoCoordinate&
DataBase<std::string, Geography::GeoCoordinate>::get(const std::string& key)
{
        return table_[key];
}

// O (N)
bool DataBase<std::string, Geography::GeoCoordinate>::del(const std::string& key)
{
        rtree_.remove(key, table_[key], 0.001);
        table_.erase(key);
        return true;
}

// O (1)
double DataBase<std::string, Geography::GeoCoordinate>::distance(
    const std::string& key1, 
    const std::string& key2)
{
    const auto& geo1 = table_[key1];
    const auto& geo2 = table_[key2];
    return Geography::distance(geo1, geo2);
}


// O (logN)
std::set<std::string> DataBase<std::string, Geography::GeoCoordinate>::radius(
        const std::string& key, 
        double rad)
{
        std::set<std::string> results;
        rtree_.search(table_[key], rad, &results);
        return results;
}
