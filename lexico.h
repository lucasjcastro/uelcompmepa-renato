/*
            Analisador L�xico para Linguagem Pascal
            Trabalho em Compiladores - 1� Bimestre - 2007

         Componentes do Grupo: Renato Euclides da Silva [renatoes@gmail.com]
                               Mateus Ruiz Balconi      [mateus.balconi@gmail.com]

   Breve Descri��o do Trabalho: A Linguagem de Programa��o Pascal Simplificado
   � utilizada para a implementa��o de um Compilador, visto que oferece um conjunto
   reduzido de s�mbolos da linguagem Pascal em sua forma normal. Este analisador
   realiza o parsing (i.e, "quebra") do arquivo de entrada Pascal em �tomos. Cada �tomo
   cont�m um s�mbolo da linguagem, chamado token. Os token s�o classificados em:
   1-) identificador, 2-) palavra reservada, 3-) n�mero, 4-) s�mbolo especial ou
   5-) s�mbolo especial composto, al�m de uma vari�vel que armazena a linha em que se
   encontra o �tomo, a classifica��o do token (dentre os valores 1 a 5 citados acima),
   e um ponteiro para um pr�ximo �tomo. Os �tomos s�o ent�o adicionados a uma lista
   de �tomos, e esta lista � ent�o o resultado final da an�lise l�xica.


  Iniciado em 26/03/2007
  �ltima revis�o em 11/04/2007
*/
#ifndef Lexico_h
#define Lexico_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "atomo.h"
//------------------------[ Defini��es ]------------------------
//Conjunto de letras v�lidas da Linguagem
char *letras = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
//Conjunto de d�gitos v�lidos da Linguagem
char *digitos = "0123456789";

/*
  Defini��o da estrutura de armazenamento de palavras reservadas
  Cada palavra reservada, assim como todo s�mbolo v�lido da linguagem,
  possui no m�ximo 24 caracteres. Estas palavras s�o inicializadas pela fun��o
  void inicializa_palavras().
*/
struct
{
	char descricao[max_tam_simb];
} palavras_reservadas[max_palavras_reservadas];


//------------------------[ Vari�veis globais ]------------------------
FILE* arquivoPascal;             //arquivo que ser� analisado
char simbolo[max_tam_simb];      //s�mbolo extra�do do arquivo
int class_simb = - 1;            //classifica��o do simbolo atual
int numLinha = 1;                //n�mero da linha atual do arquivo
char ch[2];                      //caracter atual lido. N�o foi poss�vel utilizar apenas char
atomo * lista_atomos;            //lista de �tomos
int cmdul = FALSE;               //Vari�vel dummy que verifica coment�rios em mais de uma linha
int numLinhadump;                //Vari�vel dummy que armazena a contagem de linhas quando ocorre
                                 //coment�rio em mais de uma linha
fpos_t anterior; //armazena a �ltima posi��o v�lida do arquivo fonte
//------------------------[ Prot�tipo das Fun��es ]------------------------
void abre_arquivo(char[40]);             //abre o arquivo fonte
void adiciona_atomo(atomo *);    //adiciona o �tomo � lista
void analex();                   //scanner do texto fonte
void cria_atomo();               //cria �tomo ap�s classifica��o do token
void cria_lista();               //cria a lista de s�mbolos
int eh_palavra_reservada();      //classifica o token em palavra reservada
void fecha_arquivo();            //fecha o arquivo fonte
void identificadores();          //procura classificar o token como identificador
void imprime_lista();            //imprime a lista resultante da an�lise l�xica
void inicializa_palavras();      //inicializa as palavras reservadas
int letra_eh_caracter();         //retorna TRUE se a ultima letra lida eh caracter
int letra_eh_numero();           //retorna TRUE se a ultima letra lida eh numero
void numeros();                  //procura classificar o token como n�mero
void pega_letra();               //l� uma letra do arquivo
void pega_letra_nao_branco();    //l� uma letra do arquivo, mas ignora os caracteres ' ', '\n' e '\t'
void simb_especiais();           //procura classificar o token como s�mbolo especial

//------------------------[ Fun��o analex ]------------------------
void analex()
{
    char nomeArquivo[40];

    printf("Digite o nome do arquivo fonte de entrada. O arquivo deve estar\n");
    printf("no mesmo diret�rio que o arquivo execut�vel deste programa\n");
    printf("Nome: ");
    scanf("%s",&nomeArquivo);
     //Inicializa o s�mbolo que ser� formado
     strcpy(simbolo,"");
     //Inicializa o vetor de palavras reservadas
     inicializa_palavras();
     //Abre o arquivo fonte
     abre_arquivo(nomeArquivo);
     //Cria a lista que conter� os �tomos formados
     cria_lista();

     printf("\n\nInicializando a Analise Lexica...\n\n");
     //Loop principal
     //Enquanto n�o for fim do arquivo
     while(!feof(arquivoPascal))
     {
         //Ignora espa�o em branco e busca a primeira letra do arquivo
         //que n�o seja o espa�o em branco
         pega_letra_nao_branco();

         //Se a letra for um caracter, verifica-se a possibilidade de que
         //a letra pertence a um identificador
         if(letra_eh_caracter()){
            identificadores();
         }

         //Se a letra for um n�mero, verifica-se a possibilidade de que
         //a letra pertence a um n�mero
         else if(letra_eh_numero()){
              numeros();
         }

         //Se n�o for letra ou n�mero, verifica-se a possibilidade de que
         //a letra pertence a um s�mbolo especial
         else {
              simb_especiais();
         }

         cria_atomo();

     }

     //Fecha o arquivo fonte
     fecha_arquivo();

     printf("\n\nAnalise Lexica Concluida.\n");

     //Imprime a lista de �tomos
     printf("\n\nImprimindo Resultados\n\n");
     imprime_lista();


}

//------------------------[ Fun��o inicializa_palavras ]------------------------
void inicializa_palavras()
{
  //Copia as 24 palavras reservadas determinadas pela linguagem
  //Pascal Simplificado
  strcpy(palavras_reservadas[0].descricao , "program");
  strcpy(palavras_reservadas[1].descricao , "label");
  strcpy(palavras_reservadas[2].descricao , "type");
  strcpy(palavras_reservadas[3].descricao , "array");
  strcpy(palavras_reservadas[4].descricao , "of");
  strcpy(palavras_reservadas[5].descricao , "var");
  strcpy(palavras_reservadas[6].descricao , "procedure");
  strcpy(palavras_reservadas[7].descricao , "function");
  strcpy(palavras_reservadas[8].descricao , "begin");
  strcpy(palavras_reservadas[9].descricao , "end");
  strcpy(palavras_reservadas[10].descricao , "if");
  strcpy(palavras_reservadas[11].descricao , "then");
  strcpy(palavras_reservadas[12].descricao , "else");
  strcpy(palavras_reservadas[13].descricao , "while");
  strcpy(palavras_reservadas[14].descricao , "do");
  strcpy(palavras_reservadas[15].descricao , "or");
  strcpy(palavras_reservadas[16].descricao , "and");
  strcpy(palavras_reservadas[17].descricao , "div");
  strcpy(palavras_reservadas[18].descricao , "not");
  strcpy(palavras_reservadas[19].descricao , "read");
  strcpy(palavras_reservadas[20].descricao , "write");
  strcpy(palavras_reservadas[21].descricao , "true");
  strcpy(palavras_reservadas[22].descricao , "false");
}

//------------------------[ Fun��o abre_arquivo ]------------------------
void abre_arquivo(char nomeArq[40])
{
     //Tenta abrir o arquivo fonte
     //Caso n�o consiga, o programa � fechado
     if(!(arquivoPascal = fopen(nomeArq,"r")))
     {
         printf("O arquivo nao pode ser aberto.\n");
         system("PAUSE");
         exit(0);
     }
}

//------------------------[ Fun��o fecha_arquivo ]------------------------
void fecha_arquivo()
{
    //Fecha o arquivo fonte
    fclose(arquivoPascal);
}

//------------------------[ Fun��o cria_atomo ]------------------------
void cria_atomo()
{
      if(!strcmp(simbolo,"")) return;
      //Cria um �tomo e "apaga" as informa��es do �tomo anterior
      atomo * at;

      //Aloca mem�ria para o �tomo corrente
      at = (atomo *)malloc(sizeof(atomo));

      //Copia o s�mbolo corrente para o �tomo

      strcpy(at->token,simbolo);

      //Classifica o token do �tomo de acordo com a vari�vel class_simb,
      //que armazena a classifica��o do �ltimo s�mbolo encontrado
      switch(class_simb)
      {
          case identificador: strcpy(at->classificacao,"identificador");
                              break;
          case palavra_reservada: strcpy(at->classificacao,"palavra reservada");
                              break;
          case numero: strcpy(at->classificacao,"numero");
                              break;
          case simb_especial: strcpy(at->classificacao,"simbolo especial");
                              break;
          case simb_especial_composto: strcpy(at->classificacao,"simbolo especial composto");
                              break;
      }

      //Testa a possibilidade de coment�rio de mais de uma linha
      if(cmdul) at->linha = numLinhadump;
      else at->linha = numLinha;

      //Reseta a vari�vel de coment�rio de mais de uma linha
       cmdul = FALSE;

      //Ponteiro para o pr�ximo �tomo � configurado como NULL
      at->prox = NULL;

      //Adiciona o �tomo � lista
      adiciona_atomo(at);

      //Reseta a vari�vel at
      at = NULL;

      //Reseta a vari�vel de s�mbolo corrente
      strcpy(simbolo,"");

      //Reseta a classifica��o de s�mbolo corrente
      class_simb = -1;
}

//------------------------[ Fun��o adiciona_atomo ]------------------------
void adiciona_atomo(atomo *at)
{
     //Adiciona o �tomo at � lista_atomos
     atomo *aux1, *aux2;

     if(lista_atomos == NULL)
     {
        lista_atomos = at;
     }
     else{
          aux1 = lista_atomos;
          while(aux1 != NULL)
          {
             aux2 = aux1;
             aux1 = aux1->prox;
          }
          aux2->prox = at;
     }
}

//------------------------[ Fun��o cria_lista ]------------------------
void cria_lista()
{
     lista_atomos = NULL;
}

//------------------------[ Fun��o imprime_lista ]------------------------
void imprime_lista()
{
     atomo * pont;
     pont = lista_atomos;
     while(pont!= NULL)
     {
       printf(
       "Token: %s | Classificacao: %s | Linha: %d\n",
       pont->token,pont->classificacao, pont->linha);
       printf("------------------------------------------------------------------\n");
       pont = pont->prox;
     }
}

//------------------------[ Fun��o pega_letra ]------------------------
void pega_letra()
{
     //L� uma letra qualquer do arquivo
     fgetpos(arquivoPascal,&anterior);
     fread(&ch[0],sizeof(char),1,arquivoPascal);
     if(feof(arquivoPascal))
     {
        ch[0] = '@';
     }
}

//------------------------[ Fun��o pega_letra_nao_branco ]------------------------
void pega_letra_nao_branco()
{
     //L� uma letra do arquivo
     pega_letra();

     //Enquanto a letra lida for ' ', '\t' ou '\n', continua lendo
     //at� encontrar uma letra diferente
     while(ch[0] == ' ' || ch[0] == '\t' || ch[0] == '\n')
     {
              if(ch[0] == '\n') numLinha++;
              pega_letra();
     }
}

//------------------------[ Fun��o letra_eh_numero ]------------------------
int letra_eh_numero()
{
    int eh_numero = FALSE;
    int cont = 0;

    //Verifica se a letra lida � um dos poss�veis valores num�ricos da linguagem
    for(;cont < max_digitos; cont++)
    {
        //Se for retorna true
        if(ch[0] == digitos[cont])
        {
          eh_numero = TRUE;
          break;
        }
    }
    //Sen�o retorna false
    return eh_numero;
}

//------------------------[ Fun��o letra_eh_caracter ]------------------------
int letra_eh_caracter()
{
    int eh_caracter = FALSE;
    int cont;

    //Verifica se a letra � lida � uma das poss�veis letras da linguagem
    for(cont = 0;cont < max_letras; cont++)
    {
        //Se for retorna true
        if(ch[0] == letras[cont])
        {
            eh_caracter = TRUE;
            break;
        }
    }
    //Sen�o retorna false
    return eh_caracter;
}

//------------------------[ Fun��o eh_palavra_reservada ]------------------------
int eh_palavra_reservada()
{
     int achou_reservada = FALSE;
     int cont = 0;

     //Verifica se o s�mbolo formado � palavra reservada
     for(; cont < max_palavras_reservadas; cont++)
     {
         //Se for retorna true
         if(strcmp(simbolo,palavras_reservadas[cont].descricao) == 0)
         {
           achou_reservada = TRUE;
           break;
         }
     }
     //Retorna false
     return achou_reservada;
}

//------------------------[ Fun��o pega_letra_nao_branco ]------------------------
//Esta fun��o deve verificar se o s�mbolo que est� sendo formado � um valor num�rico
void numeros()
{

     //Enquanto a letra lida do arquivo fonte for n�mero
     while(letra_eh_numero())
     {
        //Se encontrar fim do arquivo fonte, retorna
        if(feof(arquivoPascal)) break;
        //Adiciona a letra ao simbolo
        strncat(simbolo,ch,1);
        //L� a pr�xima letra do arquivo
        pega_letra();
        //Se a letra lida for caracter, ent�o um simbolo invalido ocorre
        //Ex.: 123a&1323. Isto � configurado como erro de caracter inv�lido
        if(letra_eh_caracter())
        {
          //Antes de sair do programa, imprime a lista de �tomos at� o erro
          imprime_lista();
          //Exibe mensagem de erro
          printf("Identificador invalido na linha %d\n",numLinha);
          printf("Saindo...\n");
          system("PAUSE");
          //Sai do programa
          exit(0);
        }
     }
     //Como a �ltima letra lida pode n�o ser um valor num�rico
     //a �ltima posi��o v�lida para a leitura de n�meros ser� a posi��o
     //apontada por anterior
     if(!letra_eh_numero()) fsetpos(arquivoPascal,&anterior);
     //Configura a classe do s�mbolo lido como n�mero
     class_simb = numero;
}

//------------------------[ Fun��o pega_letra_nao_branco ]------------------------
//Esta fun��o deve verificar se o s�mbolo que est� sendo formado � um identificador
void identificadores()
{

     //Enquanto a letra lida do arquivo fonte for uma letra v�lida ou um n�mero v�lido
     while(letra_eh_numero() || letra_eh_caracter())
     {
         //Se encontrar o fim do arquivo acaba
         if(feof(arquivoPascal)) break;
         //Adiciona a letra ao s�mbolo
         strncat(simbolo,ch,1);
         //L� a pr�xima letra do arquivo
         pega_letra();
     }
     //Como a �ltima letra lida pode n�o ser um n�mero ou letra
     //a �ltima posi��o v�lida para a leitura de n�meros ser� a posi��o
     //apontada por anterior
     if(!letra_eh_numero() && !letra_eh_caracter())fsetpos(arquivoPascal,&anterior);
     //Configura a classe do s�mbolo para palavra_reservada ou identificador
     if(eh_palavra_reservada()) class_simb = palavra_reservada;
     else class_simb = identificador;
}

//------------------------[ Fun��o simb_especiais ]------------------------
//Esta fun��o deve verificar se o s�mbolo que est� sendo formado � um s�mbolo especial
void simb_especiais()
{

    char pr[2];      //armazenamento tempor�rio das letras lidas

    //Se encontrar fim do arquivo, retorna
    if(feof(arquivoPascal)) return;
    //Sen�o
    else
    {
        //Caracteres simples
        if(ch[0] == '+' || ch[0] == '-' || ch[0] == ')' || ch[0] == '/' ||
        ch[0] == ',' || ch[0] == ';' || ch[0] == '=' ||ch[0] == '[' || ch[0] == '}' || ch[0] == ']')
        {
          //Configura a classe do s�mbolo formado como s�mbolo especial
          class_simb = simb_especial;
          strncat(simbolo,ch,1);
        }
        /*
        O caracter ( pode ser in�cio do s�mbolo especial composto (* (in�cio de coment�rio)
        Logo, uma verifica��o deve ser feita. Ps.: Caso o s�mbolo formado seja (*, todas as
        letras lidas entre (* e *) devem ser ignoradas.
        */
        else if(ch[0] == '(')
             {
               //Armazena a �ltima letra na vari�vel tempor�ria
               pr[0] = ch[0];
               //L� a pr�xima letra
               pega_letra();

               //Se a letra lida for *, ent�o o s�mbolo � (*
               if(ch[0] == '*'){
                        //Seta comentario mais de uma linha como true, apenas para verifica��o
                        cmdul = TRUE;
                        //Armazena o n�mero de linhas temporariamente
                        numLinhadump = numLinha;
                        //Classifica o s�mbolo como s�mbolo especial composto
                        class_simb = simb_especial_composto;
                        //Armazena a letra lida em pr[1]
                        pr[1] = ch[0];
                        //Cria o s�mbolo com as duas letras de pr[2]
                        strncat(simbolo,pr,2);
                        /*
                          Nesta parte do programa, busca-se pela letra '*', que inicializa o
                          s�mbolo de fecha coment�rio
                        */
                        //L� a pr�xima letra
                        pega_letra();
                        //Enquanto a letra lida n�o for '*'
                        while(ch[0] != '*'){
                                    //Se encontrar o fim do arquivo, gera erro
                                    if(feof(arquivoPascal))
                                    {
                                       imprime_lista();
                                       printf("Fecha comentario nao foi achado.\n");
                                       system("PAUSE");
                                       exit(0);
                                    }
                                    //Se encontrar um '\n' incrementa o contador de linhas
                                    if(ch[0] == '\n') numLinha++;
                                    //L� a pr�xima letra
                                    pega_letra();
                        }//fim while

                        //Se o caracter lido for '*', seta o ponteiro do arquivo posi��o anterior
                        //do ponteiro do arquivo. Isto � necess�rio pois ao voltar para a fun��o main
                        //o arquivo � lido novamente, e neste caso pularia o '*'
                        if(ch[0] == '*') fsetpos(arquivoPascal,&anterior);
               }
               //Se n�o for *, ent�o reposiciona o ponteiro e classifica o s�mbolo ( formado
               //como simbolo especial
               else{
                     fsetpos(arquivoPascal,&anterior);
                     class_simb = simb_especial;
                     strncat(simbolo,pr,1);
               }

         }
         //Se a letra lida for >, verifica-se a possibilidade de formar >=
         else if(ch[0] == '>')
         {
              //Armazena a �ltima letra na vari�vel tempor�ria
              pr[0] = ch[0];
              //L� a pr�xima letra
              pega_letra();
              //Se a letra for =, ent�o cria-se o s�mbolo >= e classifica-o como s�mbolo especial composto
              //Caso contr�rio, o s�mbolo formado ser� > e ser� classificado como s�mbolo especial
              if(ch[0] == '='){ class_simb = simb_especial_composto; pr[1] = ch[0]; strncat(simbolo,pr,2);}
              else{ fsetpos(arquivoPascal,&anterior); class_simb = simb_especial; strncat(simbolo,pr,1); }

         }
         //Se a letra lida for =, verifica-se a possibilidade de formar =<
         else if(ch[0] == '=')
         {
              //Armazena a �ltima letra na vari�vel tempor�ria
              pr[0] = ch[0];
              //Registra a �ltima posi��o v�lida do ponteiro do arquivo fonte
             // fgetpos(arquivoPascal,&anterior);
              //L� a pr�xima letra
              pega_letra();
              //Se a letra for <, ent�o cria-se o s�mbolo =< e classifica-o como s�mbolo especial composto
              //Caso contr�rio, o s�mbolo formado ser� > e ser� classificado como s�mbolo especial
              if(ch[0] == '<'){ class_simb = simb_especial_composto; pr[1] = ch[0]; strncat(simbolo,pr,2);}
              else{ fsetpos(arquivoPascal,&anterior); class_simb = simb_especial; strncat(simbolo,pr,1); }

         }
         else if(ch[0] == '.')
         {
              //Armazena a �ltima letra na vari�vel tempor�ria
              pr[0] = ch[0];
              //L� a pr�xima letra
              pega_letra();
              //Se a letra for >, ent�o cria-se o s�mbolo <> e classifica-o como s�mbolo especial composto
              //Caso contr�rio, o s�mbolo formado ser� > e ser� classificado como s�mbolo especial
              if(ch[0] == '.'){ class_simb = simb_especial_composto; pr[1] = ch[0]; strncat(simbolo,pr,2);}
              else if(ch[0]!= '@'){fsetpos(arquivoPascal,&anterior); class_simb = simb_especial; strncat(simbolo,pr,1); }
              else if(ch[0] == '@'){class_simb = simb_especial; strncat(simbolo,pr,1);}
         }
         //Se a letra lida for <, verifica-se a possibilidade de formar <>
         else if(ch[0] == '<')
         {
              //Armazena a �ltima letra na vari�vel tempor�ria
              pr[0] = ch[0];
              //L� a pr�xima letra
              pega_letra();
              //Se a letra for >, ent�o cria-se o s�mbolo <> e classifica-o como s�mbolo especial composto
              //Caso contr�rio, o s�mbolo formado ser� > e ser� classificado como s�mbolo especial
              if(ch[0] == '>' || ch[0] == '='){ class_simb = simb_especial_composto; pr[1] = ch[0]; strncat(simbolo,pr,2);}
              else{ fsetpos(arquivoPascal,&anterior); class_simb = simb_especial; strncat(simbolo,pr,1); }

         }
         //Se a letra lida for *, verifica-se a possibilidade de formar *)
         else if(ch[0] == '*')
         {
              //Armazena a �ltima letra na vari�vel tempor�ria
              pr[0] = ch[0];
              //L� a pr�xima letra
              pega_letra();
              //Se a letra for ), ent�o cria-se o s�mbolo *) e classifica-o como s�mbolo especial composto
              //Caso contr�rio, o s�mbolo formado ser� > e ser� classificado como s�mbolo especial
              if(ch[0] == ')'){ class_simb = simb_especial_composto; pr[1] = ch[0]; strncat(simbolo,pr,2);}
              else{ fsetpos(arquivoPascal,&anterior); class_simb = simb_especial; strncat(simbolo,pr,1); }

         }
         //Se a letra lida for :, verifica-se a possibilidade de formar :=
         else if(ch[0] == ':')
         {
              //Armazena a �ltima letra na vari�vel tempor�ria
              pr[0] = ch[0];
              //L� a pr�xima letra
              pega_letra();
              //Se a letra for =, ent�o cria-se o s�mbolo := e classifica-o como s�mbolo especial composto
              //Caso contr�rio, o s�mbolo formado ser� > e ser� classificado como s�mbolo especial
              if(ch[0] == '='){ class_simb = simb_especial_composto; pr[1] = ch[0]; strncat(simbolo,pr,2);}
              else{ fsetpos(arquivoPascal,&anterior); class_simb = simb_especial; strncat(simbolo,pr,1); }

         }
                  //Se a letra lida for :, verifica-se a possibilidade de formar :=
         else if(ch[0] == '{')
         {
             //Seta comentario mais de uma linha como true, apenas para verifica��o
             cmdul = TRUE;
             //Armazena o n�mero de linhas temporariamente
             numLinhadump = numLinha;
              //Armazena a �ltima letra na vari�vel tempor�ria
              pr[0] = ch[0];
             //L� a pr�xima letra
              pega_letra();
              while(ch[0] != '}'){
               //Se encontrar o fim do arquivo, gera erro
                if(feof(arquivoPascal))
                {
                 imprime_lista();
                 printf("Fecha comentario nao foi achado.\n");
                 system("PAUSE");
                 exit(0);
                 }
               //Se encontrar um '\n' incrementa o contador de linhas
               if(ch[0] == '\n') numLinha++;
               //L� a pr�xima letra
               pega_letra();
              }//fim while

              if(ch[0] == '}')
              {
                fsetpos(arquivoPascal,&anterior); class_simb = simb_especial; strncat(simbolo,pr,1);
              }
         }
         else{
              //Se a letra lida for um caracter inv�lido, gera erro
              imprime_lista();
              printf("Caracter invalido na linha %d\n",numLinha);
              printf("Saindo...\n");
              system("PAUSE");
              exit(0);
         }
   }
}
#endif
