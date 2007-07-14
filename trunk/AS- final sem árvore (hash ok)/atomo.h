#ifndef Atomo_h
#define Atomo_h

#define identificador 0           //identificadores serão reconhecidos pelo valor 0
#define palavra_reservada 1       //palavras reservadas serão reconhecidas pelo valor 1
#define numero 2                  //números serão reconhecidos pelo valor 2
#define simb_especial 3           //símbolos especiais serão reconhecidos pelo valor 3
#define simb_especial_composto 4  //símbolos especiais compostos serão reconhecidos pelo valor 4

#define TRUE 1                    //define 1 como TRUE
#define FALSE 0                   //define 0 como false

#define max_tam_simb 25 //todo simbolo da linguagem tem no max 25 caracteres
#define max_palavras_reservadas 24 //número máximo de palavras reservadas

#define max_letras 52 //máximo de letras
#define max_digitos 10 //máximo de dígitos


/*
  Definição do átomo
*/
typedef struct atomo
{
   char token[max_tam_simb];
   char classificacao[50];
   int linha;
   struct atomo * prox;
}atomo;
#endif
