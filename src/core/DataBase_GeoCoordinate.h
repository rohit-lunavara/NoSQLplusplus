#include "HashTable.h"
#include "GeoCoordinate.h"
#include <cmath>


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
    const GeoCoordinate& geo1 = internalStorage_[key1];
    const GeoCoordinate& geo2 = internalStorage_[key2];

    double phi1 = pi / 180. * geo1.latitude;
    double phi2 = pi / 180. * geo2.latitude;

    double dphi = phi1 - phi2;
    double dlam = pi / 180. * (geo1.longitude - geo2.longitude);

    double a = sin(0.5 * dphi) * sin(0.5 * dphi)
        + cos(phi1) * cos(phi2) * sin(0.5 * dlam) * sin(0.5 * dlam);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));

    return MEAN_EARTH_RADIUS * c;
}
