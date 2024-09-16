#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include "DataImg.h"
#include "Filters.h"
int main(){
    DataImg img;
    std::string pathImg;
    std::string saveImg;

    while(true){
        int input = -1;
        int isSobel = 0;
        std::cout << "Digite o path da imagem: ";
        std::cin >> pathImg;
        std::cout << "Digite\n1: Aplicar alguma máscara\n2:Aplicar Filtro Pontual no RGB\n3:Aplicar Filtro no Y e Voltar para o RGB\n0: Sair\n";
        std::cin >> input;
        if(input == 0) break;
        std::cout << "Digite o nome que vai ser salvo a imagem (imagem.png): ";
        std::cin >> saveImg;
        img.readImg(pathImg);
        if(input == 0) break;
        else if(input == 1){
            Filters allFilters;
            allFilters.showFilters();
            std::cout << "A máscara é Sobel?\n1: Caso sim\n0: Caso não\n";
            std::cin >> isSobel;
        
            img.applyFilters(allFilters.filters, isSobel, saveImg);
        }
        else if(input == 2){
            img.applyPontRGB(saveImg);
        }else if(input == 3){
            img.applyPontYIQRGB(saveImg);
        }
        

    }
    

    return 0 ;

}
