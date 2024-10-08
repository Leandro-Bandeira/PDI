#ifndef DATA_IMG_H
#define DATA_IMG_H

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include "Definitions.h"

/* A classe é responsável por armazenar os dados da imagem
 * Como sua matriz RGB e sua matriz modificada a partir dos filtros */
class DataImg{
    
        public:
            
            DataImg();
            void readImg(std::string path);
            void applyFilters(tMask filters, int isSobel, std::string nameSave);
            void applyPontRGB(std::string nameSave);
            void applyPontYIQRGB(std::string nameSave);

        private:
            cv::Mat imgOriginal;
            std::vector<cv::Mat>imgFilter;     
        
};

#endif

