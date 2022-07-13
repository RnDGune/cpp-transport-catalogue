#include "json_reader.h"

namespace json_reader
{
	void JSONReader::ProcessJSON(transport_catalogue::TransportCatalogue& tc,
		map_renderer::MapRenderer& mr, std::istream& input, std::ostream& output)
	{
		using namespace std::literals;

		transport_catalogue::RequestHandler rh(tc, mr);
		const json::Document j_doc = json::Load(input);
		const json::Dict j_dict = j_doc.GetRoot().AsDict();
		const auto base_requests_it = j_dict.find("base_requests"s);

		if (base_requests_it != j_dict.cend())
		{
			AddToDataBase(tc, base_requests_it->second.AsArray());
		}
		const auto renderer_settings_it = j_dict.find("render_settings"s);
		if (renderer_settings_it != j_dict.cend())
		{
			ReadRendererSettings(mr, renderer_settings_it->second.AsDict());
		}

		auto stat_requests_it = j_dict.find("stat_requests"s);
		if (stat_requests_it != j_dict.cend())
		{
			ParseJSONQueries(rh, stat_requests_it->second.AsArray(), output);
		}
	}

	void JSONReader::AddToDataBase(transport_catalogue::TransportCatalogue& tc, const json::Array& j_arr)
	{
		using namespace std::literals;
		static std::vector<std::string> stages = { "Stop"s, "Stop"s, "Bus"s };
		for (size_t i = 0; i < stages.size(); ++i)
		{
			for (const auto& element : j_arr)
			{
				const auto request_type = element.AsDict().find("type"s);
				if (request_type != element.AsDict().end())
				{

					if (request_type->second.AsString() == stages[i])
					{
						switch (i)
						{
						case 0:
							AddStopData(tc, element.AsDict());
							break;
						case 1:
							AddStopDistance(tc, element.AsDict());
							break;
						case 2:
							AddRouteData(tc, element.AsDict());
							break;
						}
					}
				}
			}
		}
	}

	void JSONReader::AddStopData(transport_catalogue::TransportCatalogue& tc, const json::Dict& j_dict)
	{
		using namespace std::literals;

		const std::string stop_name = j_dict.at("name"s).AsString();
		const double latitude = j_dict.at("latitude"s).AsDouble();
		const double longitude = j_dict.at("longitude"s).AsDouble();
		tc.AddStop(transport_catalogue::Stop{ stop_name, latitude, longitude });
	}

	void JSONReader::AddStopDistance(transport_catalogue::TransportCatalogue& tc, const json::Dict& j_dict)
	{
		using namespace std::literals;

		const std::string from_stop_name = j_dict.at("name"s).AsString();
		transport_catalogue::Stop* from_ptr = tc.GetStopByName(from_stop_name);
		if (from_ptr != nullptr)
		{
			const json::Dict stops = j_dict.at("road_distances"s).AsDict();
			for (const auto& [to_stop_name, distance] : stops)
			{
				tc.AddDistance(from_ptr, tc.GetStopByName(to_stop_name), static_cast<size_t>(distance.AsInt()));
			}
		}
	}

	void JSONReader::AddRouteData(transport_catalogue::TransportCatalogue& tc, const json::Dict& j_dict)
	{
		using namespace std::literals;

		transport_catalogue::Route new_route;
		new_route.route_name = j_dict.at("name"s).AsString();
		new_route.is_circular = j_dict.at("is_roundtrip"s).AsBool();

		for (auto& element : j_dict.at("stops"s).AsArray())
		{
			transport_catalogue::Stop* tmp_ptr = tc.GetStopByName(element.AsString());
			if (tmp_ptr != nullptr)
			{
				new_route.stops.push_back(tmp_ptr);
			}
		}
		tc.AddRoute(std::move(new_route));
	}


	const svg::Color JSONReader::ConvertColor_JSONToSVG(const json::Node& color)
	{
		if (color.IsString())
		{
			return svg::Color{ color.AsString() };
		}
		else if (color.IsArray())
		{
			if (color.AsArray().size() == 3)
			{
				return svg::Rgb
				{
					static_cast<uint8_t>(color.AsArray()[0].AsInt()),
					static_cast<uint8_t>(color.AsArray()[1].AsInt()),
					static_cast<uint8_t>(color.AsArray()[2].AsInt())
				};
			}
			else if (color.AsArray().size() == 4)
			{
				return svg::Rgba
				{
					static_cast<uint8_t>(color.AsArray()[0].AsInt()),
					static_cast<uint8_t>(color.AsArray()[1].AsInt()),
					static_cast<uint8_t>(color.AsArray()[2].AsInt()),
					color.AsArray()[3].AsDouble()
				};
			}
		}
		return svg::Color();
	}


	void JSONReader::ReadRendererSettings(map_renderer::MapRenderer& mr, const json::Dict& j_dict)
	{
		map_renderer::RendererSettings new_settings;

		new_settings.width = j_dict.at("width").AsDouble();
		new_settings.height = j_dict.at("height").AsDouble();
		new_settings.padding = j_dict.at("padding").AsDouble();
		new_settings.line_width = j_dict.at("line_width").AsDouble();
		new_settings.stop_radius = j_dict.at("stop_radius").AsDouble();
		new_settings.bus_label_font_size = j_dict.at("bus_label_font_size").AsInt();
		new_settings.bus_label_offset = { j_dict.at("bus_label_offset").AsArray()[0].AsDouble(), j_dict.at("bus_label_offset").AsArray()[1].AsDouble() };
		new_settings.stop_label_font_size = j_dict.at("stop_label_font_size").AsInt();
		new_settings.stop_label_offset = { j_dict.at("stop_label_offset").AsArray()[0].AsDouble(), j_dict.at("stop_label_offset").AsArray()[1].AsDouble() };
		new_settings.underlayer_color = ConvertColor_JSONToSVG(j_dict.at("underlayer_color"));
		new_settings.underlayer_width = j_dict.at("underlayer_width").AsDouble();
		new_settings.color_palette.clear();
		for (const auto& color : j_dict.at("color_palette").AsArray())
		{
			new_settings.color_palette.emplace_back(ConvertColor_JSONToSVG(color));
		}

		mr.ApplyRenderSettings(new_settings);
	}

	/*void ProcessQueriesJSON(transport_catalogue::RequestHandler& rh, const json::Array& j_arr, std::ostream& output)
	{
		using namespace std::literals;

		json::Array processed_queries;
		for (const auto& query : j_arr)
		{
			const auto request_type = query.AsMap().find("type"s);
			if (request_type != query.AsMap().cend())
			{
				if (request_type->second.AsString() == "Stop"s)
				{
					processed_queries.emplace_back(ProcessStopQuery(rh, query.AsMap()));
				}
				else if (request_type->second.AsString() == "Bus"s)
				{
					processed_queries.emplace_back(ProcessRouteQuery(rh, query.AsMap()));
				}
				else if (request_type->second.AsString() == "Map"s)
				{
					processed_queries.emplace_back(ProcessMapQuery(rh, query.AsMap()));
				}
			}
		}
		json::Print(json::Document{ processed_queries }, output);
	}*/

	void JSONReader::ParseJSONQueries(transport_catalogue::RequestHandler& rh, const json::Array& j_arr, std::ostream& output) {
		using namespace std::literals;

		json::Array processed_queries;

		for (const auto& query : j_arr)
		{
			const auto request_type = query.AsDict().find("type"s);
			if (request_type != query.AsDict().cend())
			{

				if (request_type->second.AsString() == "Stop"s)
				{
					processed_queries.emplace_back(ProcessStopQuery(rh, query.AsDict()));
				}
				else if (request_type->second.AsString() == "Bus"s)
				{
					processed_queries.emplace_back(ProcessRouteQuery(rh, query.AsDict()));
				}
				else if (request_type->second.AsString() == "Map"s)
				{
					processed_queries.emplace_back(ProcessMapQuery(rh, query.AsDict()));
				}
			}
		}
		json::Print(json::Document{ processed_queries }, output);
	}

	const json::Node JSONReader::ProcessStopQuery(transport_catalogue::RequestHandler& rh, const json::Dict& j_dict)
	{
		using namespace std::literals;

		const std::string stop_name = j_dict.at("name"s).AsString();
		const auto stop_query_ptr = rh.GetBusesForStop(stop_name);

		if (stop_query_ptr == nullptr)
		{
			return json::Builder{}.StartDict().Key("request_id"s).Value(j_dict.at("id"s).AsInt()).
				Key("error_message"s).Value("not found"s).EndDict().Build();
		}

		json::Array routes;
		for (auto& bus : stop_query_ptr.value()->buses)
		{
			routes.push_back(std::string(bus));
		}
		return json::Builder{}.StartDict().Key("buses"s).Value(routes).Key("request_id"s).
			Value(j_dict.at("id"s).AsInt()).EndDict().Build();
	}

	const json::Node JSONReader::ProcessRouteQuery(transport_catalogue::RequestHandler& rh, const json::Dict& j_dict)
	{
		using namespace std::literals;

		const std::string route_name = j_dict.at("name"s).AsString();
		const auto route_query_ptr = rh.GetRouteInfo(route_name);

		if (route_query_ptr == nullptr)
		{
			return json::Builder{}.StartDict().Key("request_id"s).Value(j_dict.at("id"s).AsInt()).
				Key("error_message"s).Value("not found"s).EndDict().Build();
		}
		return json::Builder{}.StartDict().Key("curvature"s).Value(route_query_ptr.value()->curvature).
			Key("request_id"s).Value(j_dict.at("id"s).AsInt()).Key("route_length"s).
			Value(static_cast<int>(route_query_ptr.value()->meters_route_length)).
			Key("stop_count"s).Value(static_cast<int>(route_query_ptr.value()->stops_on_route)).
			Key("unique_stop_count"s).Value(static_cast<int>(route_query_ptr.value()->unique_stops)).
			EndDict().Build();
	}


	const json::Node JSONReader::ProcessMapQuery(transport_catalogue::RequestHandler& rh, const json::Dict& j_dict)
	{
		using namespace std::literals;

		svg::Document svg_map = rh.GetMapRender();
		std::ostringstream os_stream;
		svg_map.Render(os_stream);
		return json::Builder{}.StartDict().Key("map"s).Value(os_stream.str()).Key("request_id"s).
			Value(j_dict.at("id"s).AsInt()).EndDict().Build();
	}


}