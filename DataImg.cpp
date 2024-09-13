#include "DataImg.h"
#include "Filters.h"


DataImg::DataImg(){
}


/* Função responsável pela leitura de imagem e salvamento dos seus dados */
void DataImg::readImg(std::string path){
    cv::Mat img = cv::imread(path, cv::IMREAD_COLOR);
        this->imgOriginal = img;
    this->imgFilter.clear();
}

void DataImg::applyFilters(int count, std::vector<tMask>filters){
    /* Para cada filtro vamos aplicar na imagem original */
    cv::Mat newImg = this->imgOriginal;
    cv::Mat resultImg = newImg.clone();         
    for(int m = 0; m < count; m++){
        
        std::vector<std::vector<double>> mask = filters[m].mask;
        int totalLineMask = filters[m].m;
        int totalColsMask = filters[m].n;
        /* O pivo da mascara, caso ela seja de tamanho impar, será o centro dela */
        int iPivor = totalLineMask / 2; 
        int jPivor = totalColsMask / 2;

        int rows = newImg.rows;
        int cols = newImg.cols;
        
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                
             
                cv::Vec3d sumPixel = {0.0, 0.0, 0.0}; 
                
                for(int k = 0; k < totalLineMask; k++){

                    for(int l = 0; l < totalColsMask; l++){
                            
                        int x = i + (k-iPivor);
                        int y = j + (l-jPivor);

                        if(x >= 0 and x < rows and y >= 0 and y < cols){
                            
                            cv::Vec3b pixelIm = newImg.at<cv::Vec3b>(x, y);
                            double cor = mask[k][l];
                       
                            sumPixel[0] += static_cast<double>(pixelIm[0]) * cor; 
                            sumPixel[1] += static_cast<double> (pixelIm[1]) * cor ;
                            sumPixel[2] += static_cast<double> (pixelIm[2]) * cor ;
                         
                 
                        }
                    
                            
                    }
                    
                }
                sumPixel[0] = std::min(std::max(sumPixel[0], 0.0), 255.0);
                sumPixel[1] = std::min(std::max(sumPixel[1], 0.0), 255.0);
                sumPixel[2] = std::min(std::max(sumPixel[2], 0.0), 255.0);
            
                cv::Vec3b& pixelCurrent = resultImg.at<cv::Vec3b>(i, j);
                pixelCurrent = cv::Vec3b(static_cast<uchar>(sumPixel[0]),
                                        static_cast<uchar>(sumPixel[1]),
                                        static_cast<uchar>(sumPixel[2]));

            } 

        }
    }
    cv::imshow("Display Window", resultImg);
    int k = cv::waitKey(0);
}
