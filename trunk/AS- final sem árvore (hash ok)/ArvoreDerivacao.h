#ifndef ArvoreDerivacao_h_
#define ArvoreDerivacao_h_
#include "NoArvore.h"
#include "TabelaHash.h"
#include <list>
class ArvoreDerivacao{
    public:
             ArvoreDerivacao();
             void criarNoRaiz(TabelaHash &);
             void criarListaFuncao(list<No_Funcao>);
             void criarListaProcedimentos(list<No_Procedimento>);
             void criarNoExpressao(No_Expr *, string);
    private:
            struct No_Programa * raiz;
};
#endif
