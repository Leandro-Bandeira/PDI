#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include "DataImg.h"
#include "Filters.h"
int main(){
    DataImg img;
    img.readImg("Trabalho1/Shapes.png");
    Filters allFilters(1);
    allFilters.showFilters();
    std::getchar();
    std::getchar();
    img.applyFilters(allFilters.count, allFilters.filters);
    return 0 ;

}
