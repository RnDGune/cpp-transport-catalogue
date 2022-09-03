#pragma once
#include "geo.h"          
#include "domain.h"       

#include <deque>
#include <map>             
#include <vector>
#include <string>
#include <string_view>
#include <set>
#include <ostream>         
#include <sstream>         
#include <iomanip>         
#include <unordered_set>
#include <unordered_map>
#include <algorithm>       
#include <utility>         
#include <cctype>          

namespace transport_catalogue
{
		
	struct StopStat
	{
		explicit StopStat(std::string_view, std::set<std::string_view>&);
		std::string_view name;
		std::set<std::string_view> buses;  // Должны быть отсортированными
	};
	using StopStatPtr = const StopStat*;



	struct RouteStat
	{
		explicit RouteStat(size_t, size_t, int64_t, double, std::string_view);
		size_t stops_on_route = 0;
		size_t unique_stops = 0;
		int64_t meters_route_length = 0;
		double curvature = 0L;
		std::string name;
	};
	using RouteStatPtr = const RouteStat*;



	class TransportCatalogue
	{
	public:
		TransportCatalogue();
		~TransportCatalogue();

		void AddStop(Stop&&);
		void AddRoute(Route&&);
		void AddDistance(const Stop*, const Stop*, size_t);

		size_t GetDistance(const Stop*, const Stop*);
		size_t GetDistanceDirectly(const Stop*, const Stop*);
		const Stop* GetStopByName(const std::string_view) const;
		const Route* GetRouteByName(const std::string_view) const;

		RouteStatPtr GetRouteInfo(const std::string_view) const;
		StopStatPtr GetBusesForStopInfo(const std::string_view) const;

		void GetAllRoutes(std::map<const std::string, RendererData>&) const; 

			size_t GetAllStopsCount() const;
		const std::vector<const Stop*> GetAllStopsPtr() const;
		const std::deque<const Route*> GetAllRoutesPtr() const;

		// SERIALIZER. Возвращает read-only словарь расстояний между всеми остановками
		const std::unordered_map<std::pair<const Stop*, const Stop*>, size_t, PairPointersHasher>& GetAllDistances() const;


	private:
		std::deque<Stop> all_stops_data_;                                     // Дек с информацией обо всех остановках 
		std::unordered_map<std::string_view, const Stop*> all_stops_map_;         // Словарь остановок (словарь с хэш-функцией)
		std::deque<Route> all_buses_data_;                                    // Дек с информацией обо всех маршрутах
		std::unordered_map<std::string_view, const Route*> all_buses_map_;        // Словарь маршрутов (автобусов) (словарь с хэш-функцией)
		std::unordered_map<std::pair<const Stop*, const Stop*>, size_t, PairPointersHasher> distances_map_;    // Словарь расстояний между остановками

		std::string_view GetStopName(const Stop* stop_ptr);
		std::string_view GetStopName(const Stop stop);
		std::string_view GetBusName(const Route* route_ptr);
		std::string_view GetBusName(const Route route);
	};
}
