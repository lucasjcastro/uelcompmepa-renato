#include "Sintatico.h"
#include "TabelaHash.h"
#include "NoHash_VarSimples.h"
#include "NoHash_ParForm.h"
#include "NoHash_Proc.h"
#include "Atomo.h"
#include <iostream>
#include <queue>
#include <string>
using namespace std;

int nivelLexico = 0;
/*
Construtor da classe Sintatico
Propósito: Inicializar um objeto Sintatico, que realiza a análise sintática do texto fonte
Parâmetros: atomo * lista - lista de análise léxica   
*/
Sintatico::Sintatico(atomo *lista)
{
    gera_fila(lista);
}


/*
Destrutor da classe Sintatico
Propósito: Desalocar memória do objeto Sintatico
Parâmetros: nenhum
*/
Sintatico::~Sintatico(){
    
}

/*
void gera_fila
Propósito: Gerar a fila utilizada para análise sintática
Parâmetros: atomo * lista - lista de análise léxica
Retorno: void
*/
void Sintatico::gera_fila(atomo * lista){
    atomo * aux;
    aux = lista;
    while(aux!=NULL){
        /*Ignora comentário*/
        if(strcmp(aux->token,"(*") && strcmp(aux->token,"*)") && 
        strcmp(aux->token,"{") && strcmp(aux->token,"}"))
        fila.push(*aux);
        aux = aux->prox;
    }
}

/*
void anasin
Propósito: Iniciar a análise sintática do código fonte
Parâmetros: nenhum
Retorno: void
*/
void Sintatico::anasin(){
    if(!programa()) erro("Analise Sintatica gerou erros");
    else cout << "\tAnalise Sintatica finalizada. Nenhum erro foi detectado." << endl;
    cout << "\n\t\t     Imprimindo Tabela Hash\n\n";
    //arvore.criarNoRaiz(hash);
    hash.imprimeHash();
}

/*
bool programa
Propósito: Faz a análise sintática do programa
Parâmetros: nenhum
Retorno: true - se não houver erros 
         false - se houver erros
*/
bool Sintatico::programa(){
    
    NoHash_VarSimples * nhvs;
    
    a = fila.front();
    if(!strcmp(a.token,"program")){
        fila.pop();
        a = fila.front();
        if(!strcmp(a.classificacao,"identificador")){
            nhvs = new NoHash_VarSimples(a,"variavel simples",nivelLexico,"palavra reservada", 0);
            hash.adicionarNo(nhvs); 
            fila.pop();
            a = fila.front();
            if(!strcmp(a.token,";")){
                fila.pop();
                if(!bloco())return false;
                a = fila.front();
                if(!strcmp(a.token,".")) return true;
                else return erro("Ponto final esperado para final de programa");
           }else return erro("Ponto-e-virgula esperado apos nome do programa");
        }else return erro("Identificador esperado como nome do programa");
    }else return erro("Palavra chave \"program\" esperada como inicio de programa");
}


/*
bool bloco
Propósito: Faz a análise sintática do bloco de comandos do programa
Parâmetros: nenhum
Retorno: true - se não houver erros 
         false - se houver erros
*/
bool Sintatico::bloco(){
    
    
    a = fila.front();
    if(!strcmp(a.token,"var")){
        fila.pop();
        if(!bloco_var()) return false;
        a = fila.front();
        while(!strcmp(a.token,"procedure") || !strcmp(a.token,"function")){
            if(!strcmp(a.token,"procedure")){
                fila.pop();
                if(!bloco_procedure()) return false;
                a = fila.front();
            }
            else if(!strcmp(a.token,"function")){
                fila.pop();
                if(!bloco_function()) return false;
                a = fila.front();
            }
        }
    }
    else if(!strcmp(a.token,"procedure")){
        fila.pop();
        if(!bloco_procedure()) return false;
        a = fila.front();
        while(!strcmp(a.token,"procedure") || !strcmp(a.token,"function")){
            if(!strcmp(a.token,"procedure")){
                fila.pop();
                if(!bloco_procedure()) return false;
                a = fila.front();
            }
            else if(!strcmp(a.token,"function")){
                fila.pop();
                if(!bloco_function()) return false;
                a = fila.front();
            }
        }
    }
    else if(!strcmp(a.token,"function")){
        fila.pop();
        if(!bloco_function()) return false;
        a = fila.front();
        while(!strcmp(a.token,"procedure") || !strcmp(a.token,"function")){
            if(!strcmp(a.token,"procedure")){
                fila.pop();
                if(!bloco_procedure()) return false;
                a = fila.front();
            }
            else if(!strcmp(a.token,"function")){
                fila.pop();
                if(!bloco_function()) return false;
                a = fila.front();
            }
        }
    }
    if(!strcmp(a.token,"begin")){
        fila.pop();
        if(!bloco_begin()) return false;
        return true;
    }
    else return erro("Declaracao de inicio de bloco esperado");
}

/*
bool bloco_var
Propósito: Faz a análise sintática da declaração de variáveis do programa
Parâmetros: nenhum
Retorno: true - se não houver erros
         false - se houver erros
*/
bool Sintatico::bloco_var(){
    
    NoHash_VarSimples * nhvs;
    int deslocamento = 0;
    list<atomo> listaVar;
    list<atomo>::iterator itVar;
    char tipoVar[50];
    
    a = fila.front();
    if(!strcmp(a.classificacao,"identificador")){
        listaVar.push_back(a);
        fila.pop();
        a = fila.front();
        while(!strcmp(a.token,",")){
            fila.pop();
            a = fila.front();
            if(!strcmp(a.classificacao,"identificador")){
                listaVar.push_back(a);
                fila.pop();
                a = fila.front();
            }else return erro("Identificador esperado apos virgula.");
        }
        if(!strcmp(a.token,":")){
            fila.pop();
            if(!tipo(tipoVar)) return false; 
            for(itVar = listaVar.begin(); itVar != listaVar.end(); itVar++){
                nhvs = new NoHash_VarSimples(*(itVar),"variavel simples",nivelLexico,tipoVar, deslocamento);
                deslocamento++;  
                hash.adicionarNo(nhvs);
            }
            listaVar.clear();
            a = fila.front();
            if(!strcmp(a.token,";")){
                fila.pop();
                a = fila.front();
                if(!strcmp(a.classificacao,"identificador")){
                    if(!bloco_var()) return false;
                }
                return true; 
            }else return erro("Ponto-e-virgula esperado apos definicao de tipo.");
        }else return erro ("Dois pontos esperado apos identificador.");
    }else return erro("Identificador esperado apos palavra-chave \"var\"");
}

/*
bool bloco_procedure
Propósito: Faz a análise sintática da declaração de procedimentos do programa
Parâmetros: nenhum
Retorno: true - se não houver erros
         false - se houver erros
*/
bool Sintatico::bloco_procedure(){

    NoHash_Proc * nhp;
    list<NoHash_ParForm*> parametros;
    atomo nomeProcedure;
    
    nivelLexico++;
        
    a = fila.front();
    if(!strcmp(a.classificacao,"identificador")){
        nomeProcedure = a;
        fila.pop();
        a = fila.front();
        if(!strcmp(a.token,"(")){
            fila.pop();
            if(!param_formais(parametros)) return false;
            a = fila.front();
            if(!strcmp(a.token,";")){
                fila.pop();
                if(!bloco())return false;
                a = fila.front();
                if(!strcmp(a.token,";")){
                    fila.pop();
                    nhp = new NoHash_Proc(nomeProcedure,"procedimento",nivelLexico, parametros.size(), parametros, (-3 - parametros.size()), "proc" );
                    hash.adicionarNo(nhp);
                    nivelLexico--;
                    return true;
                }else return erro("Ponto-e-virgula esperado apos declaracao de bloco");
            }else return erro("Ponto-e-virgula esperado apos declaracao de parametros");
        }
        else if(!strcmp(a.token,";")){
            nhp = new NoHash_Proc(nomeProcedure,"procedimento",nivelLexico, parametros.size(), parametros, (-3 - parametros.size()) , "proc" );
            hash.adicionarNo(nhp);
            fila.pop();
            if(!bloco()) return false;
            a = fila.front();
            if(!strcmp(a.token,";")){
                fila.pop();
                nivelLexico--;
                nhp = new NoHash_Proc(nomeProcedure,"procedimento",nivelLexico, parametros.size(), parametros, (-3 - parametros.size()), "proc" );
                hash.adicionarNo(nhp);
                return true;
            }else return erro("Ponto-e-virgula esperado apos declaracao de bloco");
        }else return erro("Declaracao de parametros ou ponto-e-virgula faltando em procedimento");
    }else return erro("Identificador esperado apos palavra chave \"procedure\"");
}


/*
bool bloco_function
Propósito: Faz a análise sintática da declaração de funções do programa
Parâmetros: nenhum
Retorno: true - se não houver erros
         false - se houver erros
*/
bool Sintatico::bloco_function(){
    
  NoHash_Proc * nhp;
  NoHash_VarSimples *nhvs;
  list<NoHash_ParForm*> parametros;
  atomo nomeFuncao, tipoFuncao;
    
    nivelLexico++;
    
    a = fila.front();
    if(!strcmp(a.classificacao,"identificador")){
        nomeFuncao = a;
        fila.pop();
        a = fila.front();
        if(!strcmp(a.token,"(")){
            fila.pop();
            if(!param_formais(parametros)) return false;
            a = fila.front();
            if(!strcmp(a.token,":")){
                fila.pop();
                a = fila.front();
                if(!strcmp(a.classificacao,"identificador")){
                    tipoFuncao = a;
                    nhvs = new NoHash_VarSimples(a,"variavel simples",nivelLexico,"tipo definido", 0);
                    hash.adicionarNo(nhvs);
                    fila.pop();
                    a = fila.front();
                    if(!strcmp(a.token,";")){
                        fila.pop();
                        if(!bloco()) return false;
                        a = fila.front();
                        if(!strcmp(a.token,";")){
                            fila.pop();                          
                            nhp = new NoHash_Proc(nomeFuncao,"funcao",nivelLexico, parametros.size(), parametros, (-3 - parametros.size()), tipoFuncao.token);
                            hash.adicionarNo(nhp); 
                            nivelLexico--;                           
                            return true;
                        }else return erro("Ponto e virgula esperado.");
                    }else return erro("Dois pontos esperado apos declaracao de tipo de retorno.");
                }else return erro("Identificador esperado como retorno da funcao.");
            }else return erro("Dois pontos esperado apos declaracao de parametros.");
        }
        else if(!strcmp(a.token,":")){
                fila.pop();
                a = fila.front();
                if(!strcmp(a.classificacao,"identificador")){
                    tipoFuncao = a;
                    nhvs = new NoHash_VarSimples(a,"variavel simples",nivelLexico,"tipo definido", 0);
                    hash.adicionarNo(nhvs);
                    fila.pop();
                    a = fila.front();
                    if(!strcmp(a.token,";")){
                        fila.pop();
                        if(!bloco()) return false;
                        a = fila.front();
                        if(!strcmp(a.token,";")){
                            fila.pop();                      
                            nhp = new NoHash_Proc(nomeFuncao,"funcao",nivelLexico, parametros.size(), parametros, (-3 - parametros.size()), tipoFuncao.token);
                            hash.adicionarNo(nhp);
                            nivelLexico--;
                            return true;
                        }else return erro("Ponto-e-virgula esperado apos bloco.");
                    }else return erro("Dois pontos esperado apos declaracao de tipo de retorno.");
                }else return erro("Identificador esperado como retorno da funcao.");
        }
        else return erro("Declaracao de parametros ou retorno de tipo esperado.");
    }else return erro("Identificador esperado apos palavra chave \"function\".");
}


/*
bool bloco_begin
Propósito: Faz a análise sintática de comandos do programa
Parâmetros: nenhum
Retorno: true - se não houver erros 
         false - se houver erros
*/
bool Sintatico::bloco_begin(){
    
    if(!comando()) return false;
    a = fila.front();
    while(!strcmp(a.token,";")){
        fila.pop();
        if(!comando()) return false;
        a = fila.front();
    }
    if(!strcmp(a.token,"end")){
         fila.pop();
         return true;
    }
    else return erro("Palavra-chave \"end\" esperada como fim de bloco");
}

/*
bool tipo
Propósito: Faz a análise sintática da declaração tipos de variávies do programa
Parâmetros: nenhum
Retorno: true - se não houver erros 
         false - se houver erros
*/
bool Sintatico::tipo(char * tipoEncontrado){
    
    NoHash_VarSimples * nhvs;
    
    a = fila.front();
    if(!strcmp(a.classificacao,"identificador")){
        nhvs = new NoHash_VarSimples(a,"variavel simples",nivelLexico,"tipo definido", 0);
        hash.adicionarNo(nhvs);
        strcpy(tipoEncontrado,a.token);
        fila.pop(); 
        return true;
    }
    else return erro("Identificador esperado como tipo");
}


/*
bool param_formais
Propósito: Faz a análise sintática da declaração de parâmetros formais do programa
Parâmetros: nenhum
Retorno: true - se não houver erros 
         false - se houver erros
*/
bool Sintatico::param_formais(list<NoHash_ParForm*> &parametros){
    
    NoHash_ParForm * nhpf;
    NoHash_VarSimples * nhvs;
    list<atomo>::iterator itVar;
    list<atomo> listaAuxiliar;
    int deslocamento = -3,cont2 = 0;
    
    a = fila.front();
    if(!strcmp(a.token,"var")){
        fila.pop();
        a = fila.front();
        if(!strcmp(a.classificacao,"identificador")){
            listaAuxiliar.push_front(a);
            fila.pop();
            a = fila.front();
            while(!strcmp(a.token,",")){
                fila.pop();
                a = fila.front();
                if(!strcmp(a.classificacao,"identificador")){
                    listaAuxiliar.push_front(a);
                    fila.pop();
                    a = fila.front();
                }else return erro("Identificador esperado apos virgula.");
            }
            if(!strcmp(a.token,":")){
                fila.pop();
                a = fila.front();
                if(!strcmp(a.classificacao,"identificador")){
                    for(itVar = listaAuxiliar.begin(); itVar!= listaAuxiliar.end(); itVar++){
                       nhpf = new NoHash_ParForm(*(itVar),"parametro formal", nivelLexico, a.token, (-3 - cont2), true);
                       cont2++;
                       hash.adicionarNo(nhpf);
                       parametros.push_back(nhpf);     
                    }
                    nhvs = new NoHash_VarSimples(a,"variavel simples",nivelLexico,"tipo definido", 0);
                    hash.adicionarNo(nhvs);
                    fila.pop();
                    a = fila.front();
                    if(!strcmp(a.token,")")){ 
                        fila.pop(); 
                        return true;
                    }
                    else return erro("Fecha parenteses esperado para fim de lista de parametros");
                }else return erro("Identificador esperado como tipo de parametro");
            }else return erro ("Dois pontos esperado apos identificador.");
        }else erro("Identificador esperado apos palavra-chave \"var\".");
        
    }else if(!strcmp(a.classificacao,"identificador")){
        listaAuxiliar.push_front(a);
        fila.pop();
        a = fila.front();
        while(!strcmp(a.token,",")){
                fila.pop();
                a = fila.front();
                if(!strcmp(a.classificacao,"identificador")){
                    listaAuxiliar.push_front(a);
                    fila.pop();
                    a = fila.front();
                }else return erro("Identificador esperado apos virgula.");
        }
        if(!strcmp(a.token,":")){
                fila.pop();
                a = fila.front();
                if(!strcmp(a.classificacao,"identificador")){
                    for(itVar = listaAuxiliar.begin(); itVar!= listaAuxiliar.end(); itVar++){
                       nhpf = new NoHash_ParForm(*(itVar),"parametro formal", nivelLexico, a.token,(-3 - cont2) , false);
                       cont2++;
                       hash.adicionarNo(nhpf);
                       parametros.push_back(nhpf); 
                    }
                    nhvs = new NoHash_VarSimples(a,"variavel simples",nivelLexico,"tipo definido", 0);
                    hash.adicionarNo(nhvs);
                    fila.pop();
                    a = fila.front();
                    if(!strcmp(a.token,")")){
                        fila.pop();
                        return true;
                    }
                    else return erro("Fecha parenteses esperado para fim de lista de parametros");
                }else return erro("Identificador esperado como tipo de parametro");
            }else return erro ("Dois pontos esperado apos identificador.");    
    }else return erro("Declaracao de parametros formais.");
}

/*
bool comando
Propósito: Faz a análise sintática de comandos do programa
Parâmetros: nenhum
Retorno: true - se não houver erros 
         false - se houver erros
*/
bool Sintatico::comando(){
    
    a = fila.front();
    if(!strcmp(a.classificacao,"identificador")){
        fila.pop();
        a = fila.front();
        if(!strcmp(a.token,":=")){
            fila.pop();
            if(!comando_atrib()) return false;
        }
        else if(!strcmp(a.token,"(")){
            fila.pop();
            if(!comando_id()) return false;
        }
        return true;
    }
    else if(!strcmp(a.token,"begin")){
        fila.pop();
        if(!comando_begin()) return false;
        return true;
    }
    else if(!strcmp(a.token,"if")){
        fila.pop();
        if(!comando_if()) return false;
        return true;
    }
    else if(!strcmp(a.token,"while")){
        fila.pop();
        if(!comando_while()) return false;
        return true;
    }
    else if(!strcmp(a.token,"read")){
        fila.pop();
        if(!comando_read()) return false;
        return true;
    }
    else if(!strcmp(a.token,"write")){
        fila.pop();
        if(!comando_write()) return false;
        return true;
    }
    else return erro("Declaracao de inicio de comando esperado");
        
}


/*
bool comando_atrib
Propósito: Faz a análise sintática de comandos de atribuição do programa
Parâmetros: nenhum
Retorno: true - se não houver erros 
         false - se houver erros
*/
bool Sintatico::comando_atrib(){
    
    if(!expressao()) return false;
    return true;
}


/*
bool comando_id
Propósito: Faz a análise sintática de comandos de chamada de função (que iniciam
com um identificador) do programa
Parâmetros: nenhum
Retorno: true - se não houver erros 
         false - se houver erros
*/
bool Sintatico::comando_id(){
    
    if(!expressao()) return false;
    a = fila.front();
    while(!strcmp(a.token,",")){
        fila.pop();
        if(!expressao()) return false;
        a = fila.front();
    }
    if(!strcmp(a.token,")")){
        fila.pop();
        return true;
    }
    return true;
}


/*
bool comando_begin
Propósito: Faz a análise sintática de comandos que iniciam com begin (que incluem
mais comandos) do programa
Parâmetros: nenhum
Retorno: true - se não houver erros 
         false - se houver erros
*/
bool Sintatico::comando_begin(){
    
    if(!comando()) return false;
    a = fila.front();
    while(!strcmp(a.token,";")){
        fila.pop();
        if(!comando()) return false;
        a = fila.front();
    }
    if(!strcmp(a.token,"end")){
           fila.pop();
           return true; 
    }else return erro("Palavra-chave \"end\" esperada para finalizar comando");
}


/*
bool comando_if
Propósito: Faz a análise sintática de comandos "if-then-else" do programa
Parâmetros: nenhum
Retorno: true - se não houver erros 
         false - se houver erros
*/
bool Sintatico::comando_if(){
    

    if(!expressao()) return false;
    a = fila.front();
    if(!strcmp(a.token,"then")){
        fila.pop();

        if(!comando()) return false;
        a = fila.front();
        if(!strcmp(a.token,"else")){
            fila.pop();
            if(!comando()) return false;
            return true;
        }
        return true;
    }else return erro("Palavra-chave \"then\" esperada apos expressao");
}


/*
bool comando_while
Propósito: Faz a análise sintática de comandos "while-do" do programa
Parâmetros: nenhum
Retorno: true - se não houver erros
         false - se houver erros
*/
bool Sintatico::comando_while(){
    
    if(!expressao()) return false;
    a = fila.front();
    if(!strcmp(a.token,"do")){
        fila.pop();
        if(!comando()) return false;
        return true;
    }else return erro("Palavra-chave \"do\" esperada apos expressao");
}


/*
bool comando_read
Propósito: Faz a análise sintática de comandos "read" do programa
Parâmetros: nenhum
Retorno: true - se não houver erros no bloco de comandos
         false - se houver erros
*/
bool Sintatico::comando_read(){
    
    a = fila.front();
    if(!strcmp(a.token,"(")){
        fila.pop();
        a = fila.front();
        if(!strcmp(a.classificacao,"identificador")){
            fila.pop();
            a = fila.front();
            while(!strcmp(a.token,",")){
                fila.pop();
                a = fila.front();
                if(!strcmp(a.classificacao,"identificador")){
                    fila.pop();
                    a = fila.front();
                }else return erro("Identificador esperado apos virgula");
            }
            if(!strcmp(a.token,")")){
                fila.pop();
                return true;
            }else return erro("Fecha parenteses esperado apos identificador");
        }else return erro("Identificador esperado apos abre parenteses");
    }else return erro("Abre parenteses esperado apos palavra-chave \"read\"");
}


/*
bool comando_write
Propósito: Faz a análise sintática de comandos "write" do programa
Parâmetros: nenhum
Retorno: true - se não houver erros 
         false - se houver erros
*/
bool Sintatico::comando_write(){
    
    No_Expr * raiz;
    a = fila.front();
    if(!strcmp(a.token,"(")){
        fila.pop();
        if(expressao()){
            a = fila.front();
            while(!strcmp(a.token,",")){
                fila.pop();
                if(expressao()){
                    a = fila.front();
                }else return erro("Expressao esperada apos virgula");
            }
            if(!strcmp(a.token,")")){
                fila.pop();
                return true;
            }else return erro("Fecha parenteses esperado apos identificador");
        }else return erro("Expressao esperada apos abre parenteses");
    }else return erro("Abre parenteses esperado apos palavra-chave \"write\"");
}


/*
bool expressao
Propósito: Faz a análise sintática de expressões do programa
Parâmetros: nenhum
Retorno: true - se não houver erros
         false - se houver erros
*/
bool Sintatico::expressao(){
    
    if(!expressao_simples()) return false;
    a = fila.front();
    if(!strcmp(a.token,"=") || !strcmp(a.token,"<>") || !strcmp(a.token,"<")
    || !strcmp(a.token,"<=") || !strcmp(a.token,">=") || !strcmp(a.token,">")){
        fila.pop();    
        if(!expressao_simples()) return false;
        return true;
    }
    return true;
}

/*
bool expressao_simples
Propósito: Faz a análise sintática de expressões do programa
Parâmetros: nenhum
Retorno: true - se não houver erros
         false - se houver erros
*/
bool Sintatico::expressao_simples(){
    
    a = fila.front();
    if(!strcmp(a.token,"+") || !strcmp(a.token,"-")){
        fila.pop();
        a = fila.front();   
    }
    if(!termo()) return false;
    a = fila.front();
    while(!strcmp(a.token,"+") || !strcmp(a.token,"-") || !strcmp(a.token,"or")){
        fila.pop();
        a = fila.front();
        if(!termo()) return false;
        a = fila.front(); 
    }
    return true;
}

/*
bool termo
Propósito: Faz a análise sintática de termos do programa
Parâmetros: nenhum
Retorno: true - se não houver erros
         false - se houver erros
*/
bool Sintatico::termo(){
    
    if(!fator()) return false;
    a = fila.front();
    while(!strcmp(a.token,"*") || !strcmp(a.token,"div") || !strcmp(a.token,"and")){
        fila.pop();
        if(!fator()) return false;
        a = fila.front();
    }
    return true;
}

/*
bool fator
Propósito: Faz a análise sintática de fatores do programa
Parâmetros: nenhum
Retorno: true - se não houver erros
         false - se houver erros
*/
bool Sintatico::fator(){
    
    a = fila.front();
    if(!strcmp(a.classificacao,"numero")){
        fila.pop();
        return true;
    }
    else if(!strcmp(a.classificacao,"identificador")){
        fila.pop();
        a = fila.front();
        if(!strcmp(a.token,"(")){
            fila.pop();
            if(!expressao()) return false;
            a = fila.front();
            while(!strcmp(a.token,",")){
                fila.pop();
                if(!expressao()) return false;
                a = fila.front();
            }
            if(!strcmp(a.token,")")){
                fila.pop();
                return true;
            }else return erro("Fecha parenteses esperado apos definicao de fator");
        }
        return true;
    }
    else if(!strcmp(a.token,"(")){
        fila.pop();
        if(!expressao()) return false;
        a = fila.front();
        if(!strcmp(a.token,")")){
            fila.pop();
            return true;
        }else return erro("Fecha parenteses esperado apos definicao de fator");
    }
    else if(!strcmp(a.token,"not")){
        fila.pop();
        if(!fator()) return false;
        return true;
    }
    else return erro("Declaracao de inicio de fator esperado");
}

/*
bool erro
Propósito: Indica que a análise detectou erro
Parâmetros: char * mensagem - mensagem que indica a natureza do erro gerado
Retorno: false - erro
*/
bool Sintatico::erro(char *mensagem){
    cout << "\t\t" <<mensagem;
    cout << ". Linha: " << a.linha << endl;
    return false;
}
