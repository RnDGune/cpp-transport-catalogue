#pragma once

#include <cmath>


const double earth_radius = 6371000;

#pragma once

namespace geo {

    struct Coordinates {
        double lat; // Широта
        double lng; // Долгота
        bool operator==(const Coordinates& other) const {
            return lat == other.lat && lng == other.lng;
        }
        bool operator!=(const Coordinates& other) const {
            return !(*this == other);
        }
    };

    class CoordinatesHasher
    {
    public:
        std::size_t operator()(const Coordinates&) const;
    };

    double ComputeDistance(Coordinates from, Coordinates to);

}  // namespace geo