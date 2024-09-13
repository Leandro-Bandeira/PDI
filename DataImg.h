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
            void applyFilters(int count, std::vector<tMask>filters);

        private:
            cv::Mat imgOriginal;
            std::vector<cv::Mat>imgFilter;     
        
};

#endif

