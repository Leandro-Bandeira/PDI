#ifndef FILTERS_H
#define FILTERS_h

#include <vector>
#include "Definitions.h"

/* Classe responsável por ler os filtros de entrada e armazená-los */
class Filters{
        
    public:
        Filters(int count);
        void showFilters();
        
        /* Temos um vetor tridimensional, onde representa os filtros em sequência
         * e por fim o tamanho deles definidos */
        std::vector<tMask>filters;
        int count;

    private:
};







#endif
