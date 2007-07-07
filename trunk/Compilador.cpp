#include <cstdlib>
#include <iostream>
#include "Lexico.h"
#include "Sintatico.h"
#include "ArvoreDerivacao.h"
using namespace std;

int main(int argc, char *argv[])
{
    analex();
    Sintatico sintatico(lista_atomos);
    cout << "\n\n\t\t\tAnalise Sintatica\n" << endl;
    sintatico.anasin();
    
    system("PAUSE");
    return EXIT_SUCCESS;
}
