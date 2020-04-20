#include "GeoCoordinate.h"
#include <ostream>
#include <stdexcept>

GeoCoordinate::GeoCoordinate(float latitude, float longitude)
{
    if (latitude < -85.05112878 || latitude > 85.05112878 ||
        longitude < -180. || longitude > 180)
    {
        throw std::runtime_error("invalid latitude or longitude!\nlatitude should be degree between -85.05112878 and 85.05112878\nlongitude should be degree between -180 and 180");
    }
    this->latitude = latitude;
    this->longitude = longitude;
}

std::ostream& operator<<(std::ostream& os, const GeoCoordinate& coord)
{
    
    if (coord.latitude > 0.)
    {
        os << coord.latitude << 'N';
    }
    else if (coord.latitude < 0.)
    {
         os << -coord.latitude << 'S';
    }
    os << ", ";

    if (coord.longitude > 0.)
    {
        os << coord.longitude <<  'E';
    }
    else if (coord.longitude < 0.)
    {
        os << -coord.longitude << 'W';
    }
    return os;
}