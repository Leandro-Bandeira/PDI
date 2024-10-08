#include "Filters.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

Filters::Filters()
{

    /* Para cada filtro vamos realizar a sua leitura do arquivo */

    std::string path;
    std::cout << "Digite o path do filtro:";
    std::cin >> path;
    std::fstream file(path, std::ios::in);
    if (!file.is_open())
    {
        std::cout << "Erro ao abrir o arquivo\n";
        exit(1);
    }
    int m = 0;
    int n = 0;
    int offset = 0;
    file >> m >> n >> offset;
    tMask currentMask;
    currentMask.mask.clear();
    std::string line;
    while (std::getline(file, line))
    {

        std::stringstream ss(line);

        std::vector<double> row;
        double value;
        while (ss >> value)
        {
            row.push_back(value);
        }
        if (row.empty())
            continue;
        currentMask.mask.push_back(row);
    }

    currentMask.m = m;
    currentMask.n = n;
    currentMask.offset = offset;

    file.close();

    this->filters = currentMask;
}

void Filters::showFilters()
{

    tMask currentFilter = this->filters;
    std::cout << currentFilter.m << " " << currentFilter.n << " " << currentFilter.offset << "\n";
    for (int j = 0; j < currentFilter.mask.size(); j++)
    {
        for (int l = 0; l < currentFilter.mask[j].size(); l++)
        {
            std::cout << currentFilter.mask[j][l] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "-----------------------------\n";
}
