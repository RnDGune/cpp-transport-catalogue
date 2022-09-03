
#pragma once

#include "geo.h"

#include <string>
#include <string_view>
#include <vector>
#include <functional>      

namespace transport_catalogue
{

struct Stop;      
struct Route;     


struct Stop
{
	Stop() = default;
	Stop(const std::string_view stop_name, const double lat, const double lng);
	Stop(const Stop* other_stop_ptr);

	std::string name;                   // Название остановки
	geo::Coordinates coords{ 0L,0L };   // Координаты
};


struct Route
{
	Route() = default;
	Route(const Route* other_stop_ptr);

	std::string route_name;            // Номер маршрута (название)
	std::vector<const Stop*> stops;        // Контейнер указателей на остановки маршрута
	size_t unique_stops_qty = 0U;      // Количество уникальных остановок на маршруте 
	double geo_route_length = 0L;      // Длина маршрута по прямой между координатами 
	size_t meters_route_length = 0U;   // Длина маршрута с учетом заданных расстояний между точками (метры) 
	double curvature = 0L;             // Извилистость маршрута
	bool is_circular = false;          // Является ли маршрут кольцевым
};


struct RendererData
{
	std::vector<geo::Coordinates> stop_coords;    // Контейнер координат остановок
	std::vector<std::string_view> stop_names;     // Контейнер указателей на названия остановок
	bool is_circular = false;                     // Является ли маршрут кольцевым
};

// Класс хэшера для unordered_map с ключом типа pair<const Stop*, const Stop*>
class PairPointersHasher
{
public:
	std::size_t operator()(const std::pair<const Stop*, const Stop*> pair_of_pointers) const noexcept;

private:
	std::hash<const void*> hasher_;
};

} // namespace transport_catalogue
