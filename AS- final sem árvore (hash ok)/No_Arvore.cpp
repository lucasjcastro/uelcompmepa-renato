#include "No_Arvore.h"
#include <iostream>
using namespace std;

No_Arvore::No_Arvore(){
    nome = tipo = retorno = NULL;
}
No_Arvore::~No_Arvore(){}

void No_Arvore::setNome(char * n){
    nome = new char[25];
    strcpy(nome,n);
}

void No_Arvore::setTipo(char * t){
    tipo = new char[50];
    strcpy(tipo,t);    
}

void No_Arvore::setHash(TabelaHash * hash){
    refHash = hash;    
}

void No_Arvore::setRetorno(char * r){
    retorno = new char[25];
    strcpy(retorno,r);
}

void No_Arvore::addProcedimento(No_Arvore *no){   
    procedimentos.push_back(no);
}

void No_Arvore::addFuncao(No_Arvore *no){
    funcoes.push_back(no);
}

void No_Arvore::addComando(No_Comando *no){

    comandos.push_back(no);
}

void No_Arvore::imprimeNo(){
    list<No_Arvore *>::iterator it;
    list<No_Comando *>::iterator it2;
    
    cout << "Tipo do no: " << tipo << endl;
    cout << "Nome: " << nome << endl;
    cout << "Procedimentos de " << nome << endl;
    
    it = procedimentos.begin();
    while(it!= procedimentos.end()){
        (*it)->imprimeNo();
        it++;
    }
    cout << "Funcoes de " << nome << endl;
    it = funcoes.begin();
    while(it!= funcoes.end()){
        (*it)->imprimeNo();
        it++;
    }
        
    cout << "Comandos de " << nome << endl;
    it2 = comandos.begin();
    while(it2!= comandos.end()){
        (*it2)->imprimeNo();
        it2++;
    }
}
