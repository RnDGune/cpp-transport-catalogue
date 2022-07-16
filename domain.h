#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <functional> 

#include "geo.h"

namespace transport_catalogue
{
	struct Stop
	{
		//методы
		Stop() = default;
		Stop(const std::string_view, const double, const double);
		Stop(Stop*);
		//данные
		std::string name;
		geo::Coordinates coordinates{ 0L,0L };
	};

	struct Route
	{
		//методы
		Route() = default;
		Route(Route*);
		//данные
		std::string route_name;
		std::vector<Stop*> stops;
		size_t unique_stops_qty = 0U;
		double geo_route_length = 0L;
		size_t meters_route_length = 0U;
		double curvature = 0L;
		bool is_circular = false;
	};

	class PairPointersHasher
	{
	public:
		std::size_t operator()(const std::pair<const Stop*, const Stop*>) const noexcept;

	private:
		std::hash<const void*> hasher_;
	};

	struct RendererData
	{
		std::vector<geo::Coordinates> stop_coords;   
		std::vector<std::string_view> stop_names;     
		bool is_circular = false;                    
	};

}