#include "GeoCoordinate.h"
#include <ostream>
#include <stdexcept>
#include <cmath>

namespace Geography {
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


double distance(const GeoCoordinate& coord1, const GeoCoordinate& coord2)
{
    double phi1 = pi / 180. * coord1.latitude;
    double phi2 = pi / 180. * coord2.latitude;

    double dphi = phi1 - phi2;
    double dlam = pi / 180. * (coord1.longitude - coord2.longitude);

    double a = sin(0.5 * dphi) * sin(0.5 * dphi)
        + cos(phi1) * cos(phi2) * sin(0.5 * dlam) * sin(0.5 * dlam);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));

    return MEAN_EARTH_RADIUS * c;   
}

}