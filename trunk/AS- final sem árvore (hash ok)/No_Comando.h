#ifndef No_Comando_h_
#define No_Comando_h_
#include <list>
using namespace std;
class No_Comando{
    
    public:
            No_Comando();
            ~No_Comando();
            void setTipo(char *);
            void setComandoFilho(No_Comando *);
            //void setExpressoes(
            void imprimeNo();
    private:
            char * tipo;
            list<No_Comando*> comandosFilhos;  
};

#endif
