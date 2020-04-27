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
//     not needed because all are public
//     friend std::ostream& operator<<(std::ostream& os, const GeoCoordinate& coord);
//     friend double distance(const GeoCoordinate& coord1, const GeoCoordinate& coord2);
//     friend void combine_sphere(GeoCoordinate& c, double& r,
//         GeoCoordinate c1, double r1,
//         GeoCoordinate c2, double r2);
};

std::ostream& operator<<(std::ostream& os, const GeoCoordinate& coord);

double distance(const GeoCoordinate& coord1, const GeoCoordinate& coord2);


// parameters are passed by value intentionally to avoid possible bug
// when you set the same value for c and c1
void combine_sphere(GeoCoordinate& c, double& r,
        GeoCoordinate c1, double r1, 
        GeoCoordinate c2, double r2);

double spherical_area(double radius);

bool overlap(const GeoCoordinate& c1, double r1,
        const GeoCoordinate& c2, double r2);

}