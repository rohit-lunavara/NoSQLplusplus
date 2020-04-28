#include "HashTable.h"
#include "GeoCoordinate.h"
#include "SphericalRTree.h"

template <>
class DataBase<std::string, Geography::GeoCoordinate>
{
private:
	HashTable<std::string, Geography::GeoCoordinate> internalStorage_;
        RTree::SphericalRTree<std::string> rtree_;
public:
        const Geography::GeoCoordinate& get(const std::string& key);
        bool set(const std::string& key, const Geography::GeoCoordinate& value...);
        bool del(const std::string& key);
        double distance(const std::string& key1, const std::string& key2);
};

bool DataBase<std::string, Geography::GeoCoordinate>::set(
        const std::string& key, 
        const Geography::GeoCoordinate& value...)
{
        internalStorage_[key] = value;
        rtree_.insert(key, value, 0.);
        return true;
}


const Geography::GeoCoordinate&
DataBase<std::string, Geography::GeoCoordinate>::get(const std::string& key)
{
        return internalStorage_[key];
}

bool DataBase<std::string, Geography::GeoCoordinate>::del(const std::string& key)
{
        rtree_.remove(key, internalStorage_[key], 0.);
        internalStorage_.erase(key);
}

double DataBase<std::string, Geography::GeoCoordinate>::distance(
    const std::string& key1, 
    const std::string& key2)
{
    const auto& geo1 = internalStorage_[key1];
    const auto& geo2 = internalStorage_[key2];
    return Geography::distance(geo1, geo2);
}
