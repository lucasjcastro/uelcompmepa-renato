#include "NoHash_ParForm.h"
#include <iostream>
using namespace std;

NoHash_ParForm::NoHash_ParForm(atomo id, char * cat, int niv, char * tip, int desl, bool pr)
                   :NoHash(id,cat,niv)
{
  strcpy(tipo,tip);
  deslocamento = desl;
  porReferencia = pr;
}

void NoHash_ParForm::imprimirNo(){
    NoHash::imprimirNo();
    cout << "Tipo: " << tipo << "\tDeslocamento: " << deslocamento; 
    if(porReferencia) cout << "\tPassado por referencia." << endl;
    else cout << "\tPassado por valor." << endl;
}
