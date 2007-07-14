#include "NoHash_Proc.h"
#include <iostream>
#include <list>
using namespace std;

NoHash_Proc::NoHash_Proc(atomo id, char * cat, int niv, int nP, list<NoHash_ParForm*> param, int desl, char * retorno)
                   :NoHash(id,cat,niv)
{   
  numParam = nP;
  parametros = param;
  deslocamento = desl;
  strcpy(tipoRetorno, retorno);
}

void NoHash_Proc::imprimirNo(){
    
    NoHash::imprimirNo();
    if(strcmp(tipoRetorno,"proc")) cout << "Tipo de Retorno: " << tipoRetorno << endl;
    cout << "\nDeslocamento: " << deslocamento << "\tNº de parametros: " << numParam << endl;
    list<NoHash_ParForm*>::iterator itParam;
    cout << "\nParametros:" << endl;
    
    for(itParam = parametros.begin(); itParam!= parametros.end(); itParam++)
    {
        (*itParam)->imprimirNo();
        cout << endl;
    }
    
    
}
