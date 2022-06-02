#pragma once

#include <utility>          
#include <string>
#include <string_view>     
#include <istream>          
#include <iostream>         
#include <vector>

#include "transport_catalogue.h"    

// напишите решение с нуля
// код сохраните в свой git-репозиторий

namespace detail
{
	std::pair<std::string_view, std::string_view> Split(std::string_view, char, int count = 1);
	std::string_view Lstrip(std::string_view);
	std::string_view Rstrip(std::string_view);
	std::string_view TrimString(std::string_view);
}

namespace transport_catalogue::input_reader
{
	void ProcessInput(TransportCatalogue&);     
	void ProcessInputQueries(TransportCatalogue&, std::vector<InputQuery>&); 
	Stop ProcessQueryAddStop(std::string&);           
	void ProcessQueryAddStopsDistance(TransportCatalogue&, std::string&);      
	Route ProcessQueryAddRoute(TransportCatalogue&, std::string&);             
}