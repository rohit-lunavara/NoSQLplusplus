#include "GeoCoordinate.h"
#include <ostream>
#include <stdexcept>
#include <cmath>
#include <iostream>
#include <limits>

using namespace std;


namespace Geography {
GeoCoordinate::GeoCoordinate(float latitude, float longitude)
{
    if (latitude < -90. || latitude > 90. ||
        /*latitude < -85.05112878 || latitude > 85.05112878 ||*/
        longitude < -180. || longitude > 180.)
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
    else
    {
            os << 0.;
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
    else
    {
            os << 0.;
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


void combine_sphere(GeoCoordinate& c, double& r,
        GeoCoordinate c1, double r1,
        GeoCoordinate c2, double r2)
{
        double dist = distance(c1, c2);

        if (dist + r1 <= r2) // sphere 1 is in sphere 2
        {
                c = c2;              
                r = r2;
                // cout << "c1: " << c1 << endl;
                // cout << "c2: " << c2 << endl;
                // cout << "c: " << c << endl;
                // cout << "r: " << r1 << ", " << r2 << ", " << r << endl;
        }
        else if (dist + r2 <= r1) // sphere 2 is in sphere 1
        {
                c = c1;
                r = r1;
        }
        else
        {
                double theta1 = pi / 180. * (90. - c1.latitude);
                double phi1 = pi / 180. * c1.longitude;

                double theta2 = pi / 180. * (90. - c2.latitude);
                double phi2 = pi / 180. * c2.longitude;

                double x1 = sin(theta1) * cos(phi1);
                double y1 = sin(theta1) * sin(phi1);
                double z1 = cos(theta1);


                double x2 = sin(theta2) * cos(phi2);
                double y2 = sin(theta2) * sin(phi2);
                double z2 = cos(theta2);


                double total_angle = acos(x1*x2 + y1*y2 + z1*z2);
                double angle1 = 0.5 * (dist + r2 - r1) / dist * total_angle;
                double angle2 = total_angle - angle1;

                double rr = sin(angle1) / (sin(angle1) + sin(angle2));


                double x = (1 - rr) * x1 + rr * x2;
                double y = (1 - rr) * y1 + rr * y2;
                double z = (1 - rr) * z1 + rr * z2;
                

                double norm = sqrt(x*x + y*y + z*z);

                // cout << "norm: " << norm << endl;
                if (fabs(norm) > 1e-12)
                {
                        double theta = acos(z / norm);
                        
                        double phi = atan2(y, x);

                        c.latitude = 180. / pi * (0.5 * pi - theta);
                        c.longitude = 180. / pi * phi;
                        r = 0.5 * (dist + r1 + r2);
                }
                else // cover the full earth
                {
                        c = c1;
                        r = numeric_limits<double>::infinity();
                }

               
        }

        if (r >= pi * MEAN_EARTH_RADIUS)
        {
                r = numeric_limits<double>::infinity();
        }

        // cout << "merge r: " << r << endl;

}

double spherical_area(double radius)
{
        return 4 * pi * MEAN_EARTH_RADIUS * MEAN_EARTH_RADIUS 
                * sin(0.5 * radius / MEAN_EARTH_RADIUS)
                * sin(0.5 * radius / MEAN_EARTH_RADIUS);
}

}