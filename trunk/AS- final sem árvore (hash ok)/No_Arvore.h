#ifndef No_Arvore_h_
#define No_Arvore_h_

#include "Atomo.h"
#include "TabelaHash.h"
#include "No_Comando.h"

#include <list>
using namespace std;

class No_Arvore{
    public:
            No_Arvore();
            ~No_Arvore();
            void setNome(char *);
            void setTipo(char *);
            void setRetorno(char *);
            void addProcedimento(No_Arvore *);
            void addFuncao(No_Arvore *);
            void addComando(No_Comando *);
            void setHash(TabelaHash *);
            void imprimeNo();
    private:
            list<No_Arvore*> procedimentos;
            list<No_Comando*> comandos;
            list<No_Arvore*> funcoes;
            char * tipo;
            char * retorno;
            TabelaHash * refHash;
            char * nome;
};   
#endif
