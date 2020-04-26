#pragma once
#include <ostream>

namespace Geography {


constexpr double MEAN_EARTH_RADIUS = 6371.; // km
constexpr double pi = 3.1415926535897932;

constexpr long double operator"" _N (long double latitude)
{
    return latitude;
}
constexpr long double operator"" _S (long double latitude)
{
    return -latitude;
}
constexpr long double operator"" _W (long double longitude)
{
    return -longitude;
}
constexpr long double operator"" _E (long double longitude)
{
    return longitude;
}


class GeoCoordinate
{
public:
    float latitude;
    float longitude;

    GeoCoordinate(float latitude = 0., float longitude = 0.);
    friend std::ostream& operator<<(std::ostream& os, const GeoCoordinate& coord);
    friend double distance(const GeoCoordinate& coord1, const GeoCoordinate& coord2);
};

std::ostream& operator<<(std::ostream& os, const GeoCoordinate& coord);

double distance(const GeoCoordinate& coord1, const GeoCoordinate& coord2);
}