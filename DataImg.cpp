#include "DataImg.h"
#include "Filters.h"

DataImg::DataImg()
{
}

/* Função responsável pela leitura de imagem e salvamento dos seus dados */
void DataImg::readImg(std::string path)
{
    cv::Mat img = cv::imread(path, cv::IMREAD_COLOR);
    this->imgOriginal = img;
    this->imgFilter.clear();
}

cv::Vec3d calculatePontual(std::vector<double>& pixels)
{
    /* Calcula o filtro pontual, caso for menor do que 128, multiplica por 2
    e caso seja maior, vamos fazer a conta que está representada abaixo*/

    cv::Vec3d resultPixel = {0.0, 0.0, 0.0};

    for(int i = 0; i < pixels.size(); i++){
        
        if (pixels[i] < 128){
            resultPixel[i] = 2.0 * pixels[i];
        }
        else{
            resultPixel[i] = 510.0 - (2.0 * pixels[i]);
        }
    }
    
    return resultPixel;
}


void DataImg::applyPontRGB(std::string saveImg){

    cv::Mat newImg = this->imgOriginal.clone();
    cv::Mat resultImg = newImg.clone();

    int rows = newImg.rows;
    int cols = newImg.cols;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {   cv::Vec3b pixel  = newImg.at<cv::Vec3b>(i,j);
            /* Envia os pixels na ordem correta, para realizar o cálculo */
            std::vector<double>pixelsValues = {static_cast<double>(pixel[0]), static_cast<double>(pixel[1]), static_cast<double>(pixel[2])};
            cv::Vec3d resultPixel = calculatePontual(pixelsValues);
            cv::Vec3b &pixelCurrent = resultImg.at<cv::Vec3b>(i, j);
            pixelCurrent = cv::Vec3b(static_cast<uchar>(resultPixel[0]),
                                     static_cast<uchar>(resultPixel[1]),
                                     static_cast<uchar>(resultPixel[2]));
        }
    }

    cv::imshow("Display Window", resultImg);
    int k = cv::waitKey(0);
    cv::imwrite(saveImg, resultImg);
}

void DataImg::applyPontYIQRGB(std::string saveImg){
    cv::Mat newImg = this->imgOriginal.clone();
    cv::Mat resultImg = newImg.clone();

    int rows = newImg.rows;
    int cols = newImg.cols;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {   cv::Vec3b pixel  = newImg.at<cv::Vec3b>(i,j);
            
            /* Pega os valores BGR do openCV*/
            double B = static_cast<double>(pixel[0]);
            double G = static_cast<double>(pixel[1]);
            double R = static_cast<double>(pixel[2]);

            /* Realiza a conversão */
            double Y = 0.299 * R  + 0.587 * G + 0.114 * B;
            double I = 0.596 * R - 0.274 * G - 0.322 * B;
            double Q = 0.211 * R - 0.523 * G + 0.312 * B;

            /* Aplica o pontual apenas em Y*/
            std::vector < double > pixelsValues = {Y};
            cv::Vec3d tempPixel = calculatePontual(pixelsValues);
            
            /* Volta para o BGR e salva os valores*/
            double new_B = 1.0 * tempPixel[0] - 1.106 * I + 1.703 * Q;
            double new_G = 1.0 * tempPixel[0] - 0.272 * I - 0.647 * Q;
            double new_R = 1.0 * tempPixel[0] + 0.956 * I + 0.621 * Q;

            cv::Vec3d resultPixel = {new_B, new_G, new_R};
            cv::Vec3b &pixelCurrent = resultImg.at<cv::Vec3b>(i, j);
            pixelCurrent = cv::Vec3b(static_cast<uchar>(resultPixel[0]),
                                     static_cast<uchar>(resultPixel[1]),
                                     static_cast<uchar>(resultPixel[2]));
        }
    }

    cv::imshow("Display Window", resultImg);
    int k = cv::waitKey(0);
    cv::imwrite(saveImg, resultImg);
}


/* Função responsável por aplicar a máscara na imagem */
void DataImg::applyFilters(int count, std::vector<tMask> filters, int isSobel, std::string saveImg){
    /* Para cada filtro vamos aplicar na imagem original */
    cv::Mat newImg = this->imgOriginal;
    cv::Mat resultImg = newImg.clone();
    cv::Mat histoImg = newImg.clone();


    for (int m = 0; m < count; m++)
    {

        std::vector<std::vector<double>> mask = filters[m].mask;
        int totalLineMask = filters[m].m;
        int totalColsMask = filters[m].n;
        /* O pivo da mascara, caso ela seja de tamanho impar, será o centro dela */
        int iPivor = totalLineMask / 2;
        int jPivor = totalColsMask / 2;

        int rows = newImg.rows;
        int cols = newImg.cols;

        /* Vetores responsáveis por armazena os mínimos e máximos de cada canal*/
        std::vector<double> minVals(3, std::numeric_limits<double>::max());
        std::vector<double> maxVals(3, std::numeric_limits<double>::min());

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {

                cv::Vec3d sumPixel = {0.0, 0.0, 0.0};

                for (int k = 0; k < totalLineMask; k++)
                {

                    for (int l = 0; l < totalColsMask; l++)
                    {
                        
                        /* Cálculo para pegar o pixel equivalente na imagem original*/
                        int x = i + (k - iPivor);
                        int y = j + (l - jPivor);

                        if (x >= 0 and x < rows and y >= 0 and y < cols)
                        {

                            cv::Vec3b pixelIm = newImg.at<cv::Vec3b>(x, y);
                            double cor = mask[k][l];

                            /* Aplica a máscara em cada canal */
                            sumPixel[0] += static_cast<double>(pixelIm[0]) * cor;
                            sumPixel[1] += static_cast<double>(pixelIm[1]) * cor;
                            sumPixel[2] += static_cast<double>(pixelIm[2]) * cor;
                        }
                    }
                }
                /* Se for o Sobel, vamos pegar o valor absoluto */
                if (isSobel == 1)
                {
                    sumPixel[0] = std::abs(sumPixel[0]);
                    sumPixel[1] = std::abs(sumPixel[1]);
                    sumPixel[2] = std::abs(sumPixel[2]);

                    /* Descobrindo quem é o minimo e o maximo valor*/
                    minVals[0] = std::min(minVals[0], sumPixel[0]);
                    maxVals[0] = std::max(maxVals[0], sumPixel[0]);

                    minVals[1] = std::min(minVals[1], sumPixel[1]);
                    maxVals[1] = std::max(maxVals[1], sumPixel[1]);

                    minVals[2] = std::min(minVals[2], sumPixel[2]);
                    maxVals[2] = std::max(maxVals[2], sumPixel[2]);
                }
                else
                {
                    /* Caso não for sobel, vamos apenas normlizar os valores entre 0 e 255 */
                    sumPixel[0] = std::min(std::max(sumPixel[0], 0.0), 255.0) + filters[m].offset;
                    sumPixel[1] = std::min(std::max(sumPixel[1], 0.0), 255.0) + filters[m].offset;
                    sumPixel[2] = std::min(std::max(sumPixel[2], 0.0), 255.0) + filters[m].offset;
                }

                cv::Vec3b &pixelCurrent = resultImg.at<cv::Vec3b>(i, j);
                pixelCurrent = cv::Vec3b(static_cast<uchar>(sumPixel[0]),
                                         static_cast<uchar>(sumPixel[1]),
                                         static_cast<uchar>(sumPixel[2]));
            }
        }

        if (isSobel == 1)
        {

            for (int i = 0; i < rows; i++)
            {

                for (int j = 0; j < cols; j++)
                {

                    cv::Vec3b sumPixel = resultImg.at<cv::Vec3b>(i, j);

                    /* Caso for sobel, vamos aplicar o histograma para cada canal */
                    sumPixel[0] = std::round(255 * ((static_cast<double>(sumPixel[0]) - minVals[0]) / (maxVals[0] - minVals[0]))) + filters[m].offset;
                    sumPixel[1] = std::round(255 * ((static_cast<double>(sumPixel[1]) - minVals[1]) / (maxVals[1] - minVals[1]))) + filters[m].offset;
                    sumPixel[2] = std::round(255 * ((static_cast<double>(sumPixel[2]) - minVals[2]) / (maxVals[2] - minVals[2]))) + filters[m].offset;

                    cv::Vec3b &pixelCurrent = histoImg.at<cv::Vec3b>(i, j);
                    pixelCurrent = cv::Vec3b(static_cast<uchar>(sumPixel[0]),
                                             static_cast<uchar>(sumPixel[1]),
                                             static_cast<uchar>(sumPixel[2]));
                }
            }
        }
    }

    if (isSobel)
    {
        cv::imshow("Display Window", histoImg);
        int k = cv::waitKey(0);
        cv::imwrite(saveImg, histoImg);
        
    }
    else
    {
        cv::imshow("Display Window", resultImg);
        int k = cv::waitKey(0);
        cv::imwrite(saveImg, resultImg);
    }
}
