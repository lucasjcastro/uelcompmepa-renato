#ifndef NoHash_VarSimples_h_
#define NoHash_VarSimples_h_

#include "NoHash.h"

class NoHash_VarSimples : public NoHash{

    public:
            NoHash_VarSimples(atomo, char *, int, char *, int);
            virtual void imprimirNo();
    private:
            char tipo[50];
            int deslocamento;
};
#endif
