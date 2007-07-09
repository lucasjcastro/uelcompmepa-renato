#include <iostream>
using namespace std;
#include <list>
#include <typeinfo>
#include <exception>
#include "TabelaHash.h"
#include "NoHash.h"
#include "NoHash_VarSimples.h"

TabelaHash::TabelaHash(){
    
    tabela = new list<NoHash*>[37];   
}

void TabelaHash::adicionarNo(NoHash *noAtual){
      
      if(!estaInserido(noAtual)){
            int indice = calculaPosicao(noAtual);
            tabela[indice].push_back(noAtual);  
      }
}

int TabelaHash::calculaPosicao(NoHash *noAtual){
    
    int alpha = 6;
	unsigned int h = 0;
    char token[25];
    
    strcpy(token,(*noAtual).retornaToken());
    
    for (unsigned int i = 0 ; i < strlen(token) ; i++ ) {
			h = alpha*h + (int) token[i];
	}
	 
    return h % 37;
}

bool TabelaHash::estaInserido(NoHash *noAtual){
    
    int indice = calculaPosicao(noAtual);
	list<NoHash*>::iterator itNo;

	itNo = tabela[indice].begin();

	while(itNo!= tabela[indice].end()) {
          if(!strcmp((*itNo)->retornaToken(), noAtual->retornaToken())){
          	return true;
          }
          itNo++;
    }
    return false;
}

void TabelaHash::imprimeHash(){
    
    list<NoHash*>::iterator itNo;
    for(int i = 0; i < 37; i++){ 
        cout << "---------------------------------- Entrada " << i << " ----------------------------------" << endl;
        itNo = tabela[i].begin();   
        int cont = 1;
        if(itNo!= tabela[i].end()){
            while(itNo != tabela[i].end()){
                cout << "************************************ Item " << cont << " ************************************"<<  endl;
                (*itNo)->imprimirNo();
                cout << endl;
                cout << "********************************************************************************" << endl;
                cont++; itNo++; 
            }
        } else cout << "A entrada " << i << " encontra-se vazia" << endl;
    }
}
