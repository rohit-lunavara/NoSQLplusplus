#include "HashTable.h"
#include "GeoCoordinate.h"


template <>
class DataBase<std::string, GeoCoordinate>
{
private:
	HashTable<std::string, GeoCoordinate> internalStorage_;
public:
    const GeoCoordinate& get(const std::string& key);
	bool set(const std::string& key, const GeoCoordinate& value...);
    bool del(const std::string& key);

    double distance(const std::string& key1, const std::string& key2);
};


bool DataBase<std::string, GeoCoordinate>::set(const std::string& key, const GeoCoordinate& value...)
{
    internalStorage_[key] = value;
    return true;
}


const GeoCoordinate& DataBase<std::string, GeoCoordinate>::get(const std::string& key)
{
    return internalStorage_[key];
}


double DataBase<std::string, GeoCoordinate>::distance(
    const std::string& key1, 
    const std::string& key2)
{
    return 0.;
}

