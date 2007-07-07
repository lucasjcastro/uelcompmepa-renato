#ifndef NoArvore_h_
#define NoArvore_h_
#include "Atomo.h"
#include "TabelaHash.h"
#include <list>
using namespace std;

typedef struct No_If{
	struct No_Expr *no_cond;
	struct No_Comando *no_then;
	struct No_Comando *no_else;
}No_If;


typedef struct No_While{
	struct No_Expr *no_cond;
    list<No_Comando> comandos;
}No_While;

typedef struct No_Begin{
	list<No_Comando> comandos;
}No_Begin;

typedef struct No_Write{
	list<No_Expr> expressoes;
}No_Write;

typedef struct No_Atribuicao{
	atomo *dest;
	list<No_Expr> expressoes;
}No_Atribuicao;

typedef struct No_Read{
	list<atomo> atomos;
}No_Read;

typedef struct No_Call{
    atomo *ident;
    list<No_Expr> expressoes;
}No_Call;

typedef struct No_Comando{
    No_If *no_if;
	No_While *no_while;
	No_Begin *no_begin;
	No_Read *no_read;
	No_Write *no_write;
	No_Atribuicao *no_atr;
	No_Call *no_call;
}No_Comando;

typedef struct No_Funcao{
    TabelaHash * pontHash;
    list<No_Comando> comandos;
    //list<No_Procedimento> procedimentos;
    list<No_Funcao> funcoes;
    char retorno[25];
}No_Funcao;

typedef struct No_Procedimento{
    TabelaHash * pontHash;
    list<No_Comando> comandos;
    //list<No_Procedimento> procedimentos;
    list<No_Funcao> funcoes;
}No_Procedimento;

typedef struct No_Programa{
    TabelaHash *pontHash;
    list<No_Comando> comandos;
    list<No_Funcao> funcoes;
}No_Programa;

typedef struct No_Expr{
    char rotulo[25];
    struct No_Expr * dir;
    struct No_Expr * esq;
}No_Expr;    
#endif
