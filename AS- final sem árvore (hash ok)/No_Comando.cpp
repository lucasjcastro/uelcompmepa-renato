#include "No_Comando.h"
#include <iostream>
using namespace std;

No_Comando::No_Comando(){
     tipo = NULL;    
}
No_Comando::~No_Comando(){}

void No_Comando::setTipo(char * t){
    tipo = new char[25];
    strcpy(tipo,t);    
}

void No_Comando::setComandoFilho(No_Comando *no){
    comandosFilhos.push_back(no);
}

void No_Comando::imprimeNo(){
    cout << "Tipo do comando: " << tipo << endl;
    cout << "Comandos filhos" << endl;
    list<No_Comando*>::iterator it;
    it = comandosFilhos.begin();
    while(it != comandosFilhos.end()){
        (*it)->imprimeNo();
        it++;
    }   
}
