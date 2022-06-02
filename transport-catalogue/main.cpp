// напишите решение с нуля
// код сохраните в свой git-репозиторий


#include "input_reader.h"
#include "transport_catalogue.h"
#include <iostream>         
#include <sstream>

#include "geo.h"
#include "stat_reader.h"

int main()
{
    transport_catalogue::TransportCatalogue  transport_catalog;
    transport_catalogue::input_reader::ProcessInput(transport_catalog);
    transport_catalogue::stat_reader::ProcessRequests(transport_catalog);
    return 0;
}
