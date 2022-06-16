#define _USE_MATH_DEFINES
#include "geo.h"

#include <cmath>

namespace geo {

    std::size_t CoordinatesHasher::operator()(const Coordinates& coords) const
    {
        return static_cast<std::size_t>(coords.lat + 37 * coords.lng);
    }


    double ComputeDistance(Coordinates from, Coordinates to) {
        using namespace std;
        if (from == to) {
            return 0;
        }
        static const double dr = M_PI / 180.;
        return acos(sin(from.lat * dr) * sin(to.lat * dr)
            + cos(from.lat * dr) * cos(to.lat * dr) * cos(abs(from.lng - to.lng) * dr))
            * earth_radius;
    }

}  // namespace geo