#ifndef Atomo_h
#define Atomo_h

#define identificador 0           //identificadores ser�o reconhecidos pelo valor 0
#define palavra_reservada 1       //palavras reservadas ser�o reconhecidas pelo valor 1
#define numero 2                  //n�meros ser�o reconhecidos pelo valor 2
#define simb_especial 3           //s�mbolos especiais ser�o reconhecidos pelo valor 3
#define simb_especial_composto 4  //s�mbolos especiais compostos ser�o reconhecidos pelo valor 4

#define TRUE 1                    //define 1 como TRUE
#define FALSE 0                   //define 0 como false

#define max_tam_simb 25 //todo simbolo da linguagem tem no max 25 caracteres
#define max_palavras_reservadas 24 //n�mero m�ximo de palavras reservadas

#define max_letras 52 //m�ximo de letras
#define max_digitos 10 //m�ximo de d�gitos


/*
  Defini��o do �tomo
*/
typedef struct atomo
{
   char token[max_tam_simb];
   char classificacao[50];
   int linha;
   struct atomo * prox;
}atomo;
#endif
