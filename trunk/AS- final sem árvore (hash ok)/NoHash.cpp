#include "NoHash.h"
#include "Atomo.h"
#include <iostream>
using namespace std;

NoHash::NoHash(atomo id, char * cat, int niv){
      ident = id;
      strcpy(categoria,cat);
      nivel = niv;
}

void NoHash::imprimirNo(){
    cout << "Token: " << ident.token << "\tLinha: " << ident.linha << "\tCategoria: " << categoria << endl << "\nNivel: " << nivel << "\t";
}

char * NoHash::retornaToken(){
    return ident.token;
}
