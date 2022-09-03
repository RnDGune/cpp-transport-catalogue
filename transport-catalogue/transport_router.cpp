#include "transport_router.h"

namespace router
{

	TransportRouter::TransportRouter(transport_catalogue::TransportCatalogue& tc)
		: tc_(tc), dw_graph_(tc.GetAllStopsCount() * 2)
	{}


	void TransportRouter::ApplyRouterSettings(RouterSettings& settings)
	{
		settings_ = std::move(settings);
	}


	RouterSettings TransportRouter::GetRouterSettings() const
	{
		return settings_;
	}


	const RouteData TransportRouter::CalculateRoute(const std::string_view from, const std::string_view to)
	{
		if (!router_)
		{
			BuildGraph();
		}

		RouteData result;
		auto calculated_route = router_->BuildRoute(vertexes_wait_.at(from), vertexes_wait_.at(to));

		if (calculated_route)
		{
			result.founded = true;
			for (const auto& element_id : calculated_route->edges)
			{
				auto edge_details = dw_graph_.GetEdge(element_id);
				result.total_time += edge_details.weight;
				result.items.emplace_back(RouteItem{
					edge_details.edge_name,
					(edge_details.type == graph::EdgeType::TRAVEL) ? edge_details.span_count : 0,
					edge_details.weight,
					edge_details.type });
			}
		}
		return result;
	}

	void TransportRouter::BuildGraph()
	{
		int vertex_id = 0;

		// 1. Ребра ожидания. Проходим по вектору указателей на все остановки
		for (const auto& stop : tc_.GetAllStopsPtr())
		{
			// Добавляем вершину в словарь вершин ожидания
			vertexes_wait_.insert({ stop->name, vertex_id });
			// Добавляем ее же в словарь обычных вершин
			vertexes_travel_.insert({ stop->name, ++vertex_id });
			// Создаем ребро ожидания
			dw_graph_.AddEdge({
					vertexes_wait_.at(stop->name),    // id
					vertexes_travel_.at(stop->name),  // id
					settings_.bus_wait_time * 1.0,    // вес == времени ожидания (double)
					stop->name,                       // наименование ребра == имени остановки
					graph::EdgeType::WAIT,            // тип ребра
					0                                 // span == 0 для ребра ожидания
				});
			++vertex_id;
		}

		// 2. Ребра передвижения. Проходим по указателям на все маршруты
		for (const auto& route : tc_.GetAllRoutesPtr())
		{
			// Проходим по всем остановкам (кроме последней) каждого маршрута и для каждой текущей...
			for (size_t it_from = 0; it_from < route->stops.size() - 1; ++it_from)
			{
				int span_count = 0;
				// ...до каждой из оставшихся остановок маршрута
				for (size_t it_to = it_from + 1; it_to < route->stops.size(); ++it_to)
				{
					double road_distance = 0.0;
					// Считаем дистанцию
					for (size_t it = it_from + 1; it <= it_to; ++it)
					{
						road_distance += static_cast<double>(tc_.GetDistance(route->stops[it - 1], route->stops[it]));
					}
					// Создаем ребро передвижения с весом, равным времени на проезд от вершины передвижения
					// исходной остановки до вершины ожидания другой остановки
					dw_graph_.AddEdge({
							vertexes_travel_.at(route->stops[it_from]->name),
							vertexes_wait_.at(route->stops[it_to]->name),
							road_distance / (settings_.bus_velocity * 1000.0 / 60.0),    // вес (== времени движения)
							route->route_name,
							graph::EdgeType::TRAVEL,
							++span_count     // Счетчик остановок в ребре
						});
				}
			}
		}

		router_ = std::make_unique<graph::Router<double>>(dw_graph_);
	}

}
