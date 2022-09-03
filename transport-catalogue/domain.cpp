/*
 * В этом файле вы можете размещены классы/структуры, которые являются частью предметной области
 * (domain) приложения и не зависят от транспортного справочника. Например Автобусные
 * маршруты и Остановки.
 */

#include "domain.h"

namespace transport_catalogue
{

Stop::Stop(const Stop* other_stop_ptr) :
	name(other_stop_ptr->name),
	coords(other_stop_ptr->coords)
{}

Stop::Stop(const std::string_view stop_name, const double lat, const double lng) : 
	name(stop_name), 
	coords(geo::Coordinates{ lat, lng })
{}


Route::Route(const Route* other_stop_ptr) :
    route_name(other_stop_ptr->route_name),
	stops(other_stop_ptr->stops),
	unique_stops_qty(other_stop_ptr->unique_stops_qty),
	geo_route_length(other_stop_ptr->geo_route_length),
	meters_route_length(other_stop_ptr->meters_route_length),
	curvature(other_stop_ptr->curvature),
	is_circular(other_stop_ptr->is_circular)
{}


std::size_t  PairPointersHasher::operator()(const std::pair<const Stop*, const Stop*> pair_of_pointers) const noexcept
{
	return hasher_(static_cast<const void*>(pair_of_pointers.first)) * 37
		+ hasher_(static_cast<const void*>(pair_of_pointers.second));
}

} // namespace transport_catalogue
