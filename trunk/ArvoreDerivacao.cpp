#include "ArvoreDerivacao.h"
#include <string>
#include <iostream>
using namespace std;

ArvoreDerivacao::ArvoreDerivacao(){

}

void ArvoreDerivacao::criarNoRaiz(TabelaHash &tabela){
    raiz = (No_Programa *)malloc(sizeof(No_Programa));
    raiz->pontHash = &tabela;
}

//void ArvoreDerivacao::criarNoFuncao(list<No_Funcao> funcoes){
 //   raiz->funcoes = funcoes;
//}
void ArvoreDerivacao::criarNoExpressao(No_Expr *raiz, string expressao){
    No_Expr * aux;
    char opcode[2], operando[expressao.length()];
    for(int i = 0; i < expressao.length(); i++){
        if(expressao.at(i) == '+' || expressao.at(i) == '-' || expressao.at(i) == '*'){
            cout << "I : " << i;
           for(int j = 0; j < i; j++) operando[j] = expressao.at(j);
           raiz = (No_Expr *)malloc(sizeof(No_Expr));
           cout << "Ok0" << endl;
           opcode[0] = expressao.at(i);
           cout << "Ok1" << endl;
           opcode[1] = '\0';
           cout << "Ok2" << endl;
           strcpy(raiz->rotulo, opcode);
           cout << "Ok3" << endl;
           raiz->esq = (No_Expr *)malloc(sizeof(No_Expr));
           cout << "Ok4" << endl;
           strcpy(raiz->esq->rotulo,operando); 
           cout << "Ok5" << endl;
           raiz->dir = NULL;
           cout << "Ok6" << endl;    
        }    
    }
}
