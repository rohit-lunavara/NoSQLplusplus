#include <ostream>

constexpr double EARTH_RADIUS = 6371.; // km

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
};

std::ostream& operator<<(std::ostream& os, const GeoCoordinate& coord);
