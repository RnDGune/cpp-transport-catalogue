#pragma once

#include <iostream>                 
#include <sstream>  
#include <vector> 

#include "request_handler.h"        
#include "json.h"
#include "map_renderer.h"
#include "json_builder.h"

namespace json_reader 
{
	void ProcessJSON(transport_catalogue::TransportCatalogue&,
		map_renderer::MapRenderer&, std::istream&, std::ostream&);

	void AddToDataBase(transport_catalogue::TransportCatalogue&, const json::Array&);

	void AddStopData(transport_catalogue::TransportCatalogue&, const json::Dict&);
	
	void AddStopDistance(transport_catalogue::TransportCatalogue&, const json::Dict&);
	
	void AddRouteData(transport_catalogue::TransportCatalogue&, const json::Dict&);

	const svg::Color ConvertColor_JSONToSVG(const json::Node&);

	void ReadRendererSettings(map_renderer::MapRenderer&, const json::Dict&);

	//void ProcessQueriesJSON(transport_catalogue::RequestHandler&, const json::Array&, std::ostream&);
	void ParseJSONQueries(transport_catalogue::RequestHandler&, const json::Array&, std::ostream&);

	const json::Node ProcessStopQuery(transport_catalogue::RequestHandler&, const json::Dict&);
	
	const json::Node ProcessRouteQuery(transport_catalogue::RequestHandler&, const json::Dict&);
	
	const json::Node ProcessMapQuery(transport_catalogue::RequestHandler&, const json::Dict&);
}

