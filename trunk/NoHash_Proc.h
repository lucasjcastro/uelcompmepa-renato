#ifndef NoHash_Proc_h_
#define NoHash_Proc_h_

#include "NoHash.h"
#include "NoHash_ParForm.h"
#include <list>
using namespace std;

class NoHash_Proc : public NoHash{

    public:
            NoHash_Proc(atomo, char *, int, int, list<NoHash_ParForm*>, int, char *);
            virtual void imprimirNo();
    private:
            int deslocamento;
            int numParam;
            std::list<NoHash_ParForm*> parametros;
            char tipoRetorno[30];
};
#endif
