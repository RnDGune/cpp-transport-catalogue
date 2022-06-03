// �������� ������� � ����
// ��� ��������� � ���� git-�����������


#include "input_reader.h"
#include "transport_catalogue.h"
#include <iostream>         
#include <sstream>

#include "geo.h"
#include "stat_reader.h"

int main()
{
    transport_catalogue::TransportCatalogue  transport_catalog;
    transport_catalogue::input_reader::ProcessInput(transport_catalog,std::cin);
    transport_catalogue::stat_reader::ProcessRequests(transport_catalog,std::cout,std::cin);
    return 0;
}