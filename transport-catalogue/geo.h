#pragma once

#include <cmath>


const double earth_radius = 6371000;

#pragma once

namespace geo {

    struct Coordinates {
        double lat; // ������
        double lng; // �������
        bool operator==(const Coordinates& other) const;
        bool operator!=(const Coordinates& other) const;

    };

    class CoordinatesHasher
    {
    public:
        std::size_t operator()(const Coordinates&) const;
    };

    double ComputeDistance(Coordinates from, Coordinates to);

}  // namespace geo