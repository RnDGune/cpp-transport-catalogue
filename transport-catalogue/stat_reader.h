#pragma once
        

#include <utility>          
#include <string>
#include <string_view>      
#include <sstream>          
#include <iomanip>         
#include <istream>          
#include <iostream>      

#include "transport_catalogue.h"   
#include "input_reader.h"  


namespace transport_catalogue::stat_reader
{

	std::ostream& operator<<(std::ostream&, const Stop*);
	std::ostream& operator<<(std::ostream& os, const Route* route);
	std::ostream& ProcessRequests(TransportCatalogue&, std::ostream&, std::istream&);
	std::ostream& ExecuteRequest(TransportCatalogue&, RequestQuery&, std::ostream&);

}