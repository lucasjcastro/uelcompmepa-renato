#ifndef NoHash_h
#define NoHash_h
#include "Atomo.h"

class NoHash{
    
    public:
             NoHash(atomo, char *, int);
             virtual void imprimirNo();
             char * retornaToken(); 
    private:
             atomo ident;
             char categoria[30];
             int nivel;   
};
#endif
