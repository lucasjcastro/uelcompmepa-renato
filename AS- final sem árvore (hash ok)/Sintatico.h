#ifndef Sintatico_h_
#define Sintatico_h_
#include "Atomo.h"
#include "TabelaHash.h"
#include "NoHash_ParForm.h"
#include "No_Arvore.h"
#include "No_Comando.h"
#include <queue>
#include <list>
#include <string>
using namespace std;

class Sintatico{
    
    public:
            Sintatico(atomo *);
            ~Sintatico();
            void anasin();
            
    private:
            atomo a;
            queue<atomo> fila;
            TabelaHash hash;
            void gera_fila(atomo *);
            bool programa();
            bool bloco(No_Arvore *);
            bool bloco_var();
            bool bloco_procedure(No_Arvore *);
            bool bloco_function(No_Arvore *);
            bool bloco_begin(No_Arvore *);
            bool tipo(char *);
            bool param_formais(list<NoHash_ParForm*> &);
            bool comando(No_Comando *);
            bool comando_id(No_Comando*);
            bool comando_atrib(No_Comando *);
            bool comando_begin(No_Comando *);
            bool comando_if(No_Comando*);
            bool comando_while(No_Comando*);
            bool comando_read(No_Comando*);
            bool comando_write(No_Comando*);
            bool expressao();
            bool expressao_simples();
            bool fator();
            bool termo();
            bool erro(char *);
};
#endif
