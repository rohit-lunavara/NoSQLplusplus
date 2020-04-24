#include "HashTable.h"
#include "GeoCoordinate.h"

template <>
class DataBase<std::string, Geography::GeoCoordinate>
{
private:
	HashTable<std::string, Geography::GeoCoordinate> internalStorage_;
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
    return true;
}


const Geography::GeoCoordinate&
DataBase<std::string, Geography::GeoCoordinate>::get(const std::string& key)
{
    return internalStorage_[key];
}


double DataBase<std::string, Geography::GeoCoordinate>::distance(
    const std::string& key1, 
    const std::string& key2)
{
    const auto& geo1 = internalStorage_[key1];
    const auto& geo2 = internalStorage_[key2];
    return Geography::distance(geo1, geo2);
}

