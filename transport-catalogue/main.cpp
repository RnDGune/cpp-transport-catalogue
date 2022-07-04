// напишите решение с нуля
// код сохраните в свой git-репозиторий


#include "request_handler.h"    
#include "json_reader.h"        
#include "map_renderer.h"
#include "json_builder.h"

#include <iostream>         
#include <sstream>


int main()
{
    transport_catalogue::TransportCatalogue tc;
    map_renderer::MapRenderer mr;
   // transport_catalogue::RequestHandler rh(tc, mr);
    json_reader::ProcessJSON(tc, mr, std::cin, std::cout);

}

