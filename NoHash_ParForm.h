#ifndef NoHash_ParForm_h_
#define NoHash_ParForm_h_

#include "NoHash.h"

class NoHash_ParForm : public NoHash{
    
    public:
            NoHash_ParForm(atomo, char *, int, char *, int, bool);
            virtual void imprimirNo();
    private:
            char tipo[50];
            int deslocamento;
            bool porReferencia;
};
#endif
