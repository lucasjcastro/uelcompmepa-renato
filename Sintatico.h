#ifndef Sintatico_h_
#define Sintatico_h_
#include "Atomo.h"
#include "TabelaHash.h"
#include "ArvoreDerivacao.h"
#include "NoHash_ParForm.h"
#include <queue>
#include <list>
#include <string>
class Sintatico{
    
    public:
            Sintatico(atomo *);
            ~Sintatico();
            void anasin();
            
    private:
            atomo a;
            std::queue<atomo> fila;
            TabelaHash hash;
            ArvoreDerivacao arvore;
            void gera_fila(atomo *);
            bool programa();
            bool bloco();
            bool bloco_var();
            bool bloco_procedure();
            bool bloco_function();
            bool bloco_begin();
            bool tipo(char *);
            bool param_formais(list<NoHash_ParForm*> &);
            bool comando();
            bool comando_id();
            bool comando_atrib();
            bool comando_begin();
            bool comando_if();
            bool comando_while();
            bool comando_read();
            bool comando_write();
            bool expressao();
            bool expressao_simples();
            bool fator();
            bool termo();
            bool erro(char *);
};
#endif
