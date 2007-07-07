#include "NoHash_VarSimples.h"
#include <iostream>
using namespace std;

NoHash_VarSimples::NoHash_VarSimples(atomo id, char * cat, int niv, char * tip, int desl)
                   :NoHash(id,cat,niv)
{
  strcpy(tipo,tip);
  deslocamento = desl;      
}

void NoHash_VarSimples::imprimirNo(){
    NoHash::imprimirNo();
    cout << "Tipo: " << tipo << "\tDeslocamento: " << deslocamento << endl;
}
