#include <iostream>             
#include <fstream>              
#include <string_view>

#include "request_handler.h"    
#include "json_reader.h"        
#include "json_builder.h"
#include "map_renderer.h"


using namespace std;

/*
int main()
{
    transport_catalogue::TransportCatalogue tc;
    map_renderer::MapRenderer mr;
    json_reader::ProcessJSON(tc, mr, std::cin, std::cout);
}
*/

void PrintUsage(std::ostream& stream = std::cerr)
{
    stream << "Usage: transport_catalogue [make_base|process_requests]\n"sv;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        PrintUsage();
        return 1;
    }

    const std::string_view mode(argv[1]);

    if (mode == "make_base"sv)
    {
        transport_catalogue::TransportCatalogue tc;
        map_renderer::MapRenderer mr;
        json_reader::ProcessBaseJSON(tc, mr, std::cin);
    }
    else if (mode == "process_requests"sv)
    {
        transport_catalogue::TransportCatalogue tc;
        map_renderer::MapRenderer mr;
        json_reader::ProcessRequestJSON(tc, mr, std::cin, std::cout);
    }
    else
    {
        PrintUsage();
        return 1;
    }
}