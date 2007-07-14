#ifndef TabelaHash_h_
#define TabelaHash_h_
#include "NoHash.h"
#include <list>

class TabelaHash{
        
        public:
               TabelaHash();
               void adicionarNo(NoHash *noAtual);
               void imprimeHash();
                
        private:
                std::list<NoHash*> * tabela;
                int calculaPosicao(NoHash *noAtual);
                bool estaInserido(NoHash *noAtual);
                
};
#endif
