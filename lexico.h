/*
            Analisador Léxico para Linguagem Pascal
            Trabalho em Compiladores - 1° Bimestre - 2007

         Componentes do Grupo: Renato Euclides da Silva [renatoes@gmail.com]
                               Mateus Ruiz Balconi      [mateus.balconi@gmail.com]

   Breve Descrição do Trabalho: A Linguagem de Programação Pascal Simplificado
   é utilizada para a implementação de um Compilador, visto que oferece um conjunto
   reduzido de símbolos da linguagem Pascal em sua forma normal. Este analisador
   realiza o parsing (i.e, "quebra") do arquivo de entrada Pascal em átomos. Cada átomo
   contém um símbolo da linguagem, chamado token. Os token são classificados em:
   1-) identificador, 2-) palavra reservada, 3-) número, 4-) símbolo especial ou
   5-) símbolo especial composto, além de uma variável que armazena a linha em que se
   encontra o átomo, a classificação do token (dentre os valores 1 a 5 citados acima),
   e um ponteiro para um próximo átomo. Os átomos são então adicionados a uma lista
   de átomos, e esta lista é então o resultado final da análise léxica.


  Iniciado em 26/03/2007
  Última revisão em 11/04/2007
*/
#ifndef Lexico_h
#define Lexico_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "atomo.h"
//------------------------[ Definições ]------------------------
//Conjunto de letras válidas da Linguagem
char *letras = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
//Conjunto de dígitos válidos da Linguagem
char *digitos = "0123456789";

/*
  Definição da estrutura de armazenamento de palavras reservadas
  Cada palavra reservada, assim como todo símbolo válido da linguagem,
  possui no máximo 24 caracteres. Estas palavras são inicializadas pela função
  void inicializa_palavras().
*/
struct
{
	char descricao[max_tam_simb];
} palavras_reservadas[max_palavras_reservadas];


//------------------------[ Variáveis globais ]------------------------
FILE* arquivoPascal;             //arquivo que será analisado
char simbolo[max_tam_simb];      //símbolo extraído do arquivo
int class_simb = - 1;            //classificação do simbolo atual
int numLinha = 1;                //número da linha atual do arquivo
char ch[2];                      //caracter atual lido. Não foi possível utilizar apenas char
atomo * lista_atomos;            //lista de átomos
int cmdul = FALSE;               //Variável dummy que verifica comentários em mais de uma linha
int numLinhadump;                //Variável dummy que armazena a contagem de linhas quando ocorre
                                 //comentário em mais de uma linha
fpos_t anterior; //armazena a última posição válida do arquivo fonte
//------------------------[ Protótipo das Funções ]------------------------
void abre_arquivo(char[40]);             //abre o arquivo fonte
void adiciona_atomo(atomo *);    //adiciona o átomo à lista
void analex();                   //scanner do texto fonte
void cria_atomo();               //cria átomo após classificação do token
void cria_lista();               //cria a lista de símbolos
int eh_palavra_reservada();      //classifica o token em palavra reservada
void fecha_arquivo();            //fecha o arquivo fonte
void identificadores();          //procura classificar o token como identificador
void imprime_lista();            //imprime a lista resultante da análise léxica
void inicializa_palavras();      //inicializa as palavras reservadas
int letra_eh_caracter();         //retorna TRUE se a ultima letra lida eh caracter
int letra_eh_numero();           //retorna TRUE se a ultima letra lida eh numero
void numeros();                  //procura classificar o token como número
void pega_letra();               //lê uma letra do arquivo
void pega_letra_nao_branco();    //lê uma letra do arquivo, mas ignora os caracteres ' ', '\n' e '\t'
void simb_especiais();           //procura classificar o token como símbolo especial

//------------------------[ Função analex ]------------------------
void analex()
{
    char nomeArquivo[40];

    printf("Digite o nome do arquivo fonte de entrada. O arquivo deve estar\n");
    printf("no mesmo diretório que o arquivo executável deste programa\n");
    printf("Nome: ");
    scanf("%s",&nomeArquivo);
     //Inicializa o símbolo que será formado
     strcpy(simbolo,"");
     //Inicializa o vetor de palavras reservadas
     inicializa_palavras();
     //Abre o arquivo fonte
     abre_arquivo(nomeArquivo);
     //Cria a lista que conterá os átomos formados
     cria_lista();

     printf("\n\nInicializando a Analise Lexica...\n\n");
     //Loop principal
     //Enquanto não for fim do arquivo
     while(!feof(arquivoPascal))
     {
         //Ignora espaço em branco e busca a primeira letra do arquivo
         //que não seja o espaço em branco
         pega_letra_nao_branco();

         //Se a letra for um caracter, verifica-se a possibilidade de que
         //a letra pertence a um identificador
         if(letra_eh_caracter()){
            identificadores();
         }

         //Se a letra for um número, verifica-se a possibilidade de que
         //a letra pertence a um número
         else if(letra_eh_numero()){
              numeros();
         }

         //Se não for letra ou número, verifica-se a possibilidade de que
         //a letra pertence a um símbolo especial
         else {
              simb_especiais();
         }

         cria_atomo();

     }

     //Fecha o arquivo fonte
     fecha_arquivo();

     printf("\n\nAnalise Lexica Concluida.\n");

     //Imprime a lista de átomos
     printf("\n\nImprimindo Resultados\n\n");
     imprime_lista();


}

//------------------------[ Função inicializa_palavras ]------------------------
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

//------------------------[ Função abre_arquivo ]------------------------
void abre_arquivo(char nomeArq[40])
{
     //Tenta abrir o arquivo fonte
     //Caso não consiga, o programa é fechado
     if(!(arquivoPascal = fopen(nomeArq,"r")))
     {
         printf("O arquivo nao pode ser aberto.\n");
         system("PAUSE");
         exit(0);
     }
}

//------------------------[ Função fecha_arquivo ]------------------------
void fecha_arquivo()
{
    //Fecha o arquivo fonte
    fclose(arquivoPascal);
}

//------------------------[ Função cria_atomo ]------------------------
void cria_atomo()
{
      if(!strcmp(simbolo,"")) return;
      //Cria um átomo e "apaga" as informações do átomo anterior
      atomo * at;

      //Aloca memória para o átomo corrente
      at = (atomo *)malloc(sizeof(atomo));

      //Copia o símbolo corrente para o átomo

      strcpy(at->token,simbolo);

      //Classifica o token do átomo de acordo com a variável class_simb,
      //que armazena a classificação do último símbolo encontrado
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

      //Testa a possibilidade de comentário de mais de uma linha
      if(cmdul) at->linha = numLinhadump;
      else at->linha = numLinha;

      //Reseta a variável de comentário de mais de uma linha
       cmdul = FALSE;

      //Ponteiro para o próximo átomo é configurado como NULL
      at->prox = NULL;

      //Adiciona o átomo à lista
      adiciona_atomo(at);

      //Reseta a variável at
      at = NULL;

      //Reseta a variável de símbolo corrente
      strcpy(simbolo,"");

      //Reseta a classificação de símbolo corrente
      class_simb = -1;
}

//------------------------[ Função adiciona_atomo ]------------------------
void adiciona_atomo(atomo *at)
{
     //Adiciona o átomo at à lista_atomos
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

//------------------------[ Função cria_lista ]------------------------
void cria_lista()
{
     lista_atomos = NULL;
}

//------------------------[ Função imprime_lista ]------------------------
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

//------------------------[ Função pega_letra ]------------------------
void pega_letra()
{
     //Lê uma letra qualquer do arquivo
     fgetpos(arquivoPascal,&anterior);
     fread(&ch[0],sizeof(char),1,arquivoPascal);
     if(feof(arquivoPascal))
     {
        ch[0] = '@';
     }
}

//------------------------[ Função pega_letra_nao_branco ]------------------------
void pega_letra_nao_branco()
{
     //Lê uma letra do arquivo
     pega_letra();

     //Enquanto a letra lida for ' ', '\t' ou '\n', continua lendo
     //até encontrar uma letra diferente
     while(ch[0] == ' ' || ch[0] == '\t' || ch[0] == '\n')
     {
              if(ch[0] == '\n') numLinha++;
              pega_letra();
     }
}

//------------------------[ Função letra_eh_numero ]------------------------
int letra_eh_numero()
{
    int eh_numero = FALSE;
    int cont = 0;

    //Verifica se a letra lida é um dos possíveis valores numéricos da linguagem
    for(;cont < max_digitos; cont++)
    {
        //Se for retorna true
        if(ch[0] == digitos[cont])
        {
          eh_numero = TRUE;
          break;
        }
    }
    //Senão retorna false
    return eh_numero;
}

//------------------------[ Função letra_eh_caracter ]------------------------
int letra_eh_caracter()
{
    int eh_caracter = FALSE;
    int cont;

    //Verifica se a letra é lida é uma das possíveis letras da linguagem
    for(cont = 0;cont < max_letras; cont++)
    {
        //Se for retorna true
        if(ch[0] == letras[cont])
        {
            eh_caracter = TRUE;
            break;
        }
    }
    //Senão retorna false
    return eh_caracter;
}

//------------------------[ Função eh_palavra_reservada ]------------------------
int eh_palavra_reservada()
{
     int achou_reservada = FALSE;
     int cont = 0;

     //Verifica se o símbolo formado é palavra reservada
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

//------------------------[ Função pega_letra_nao_branco ]------------------------
//Esta função deve verificar se o símbolo que está sendo formado é um valor numérico
void numeros()
{

     //Enquanto a letra lida do arquivo fonte for número
     while(letra_eh_numero())
     {
        //Se encontrar fim do arquivo fonte, retorna
        if(feof(arquivoPascal)) break;
        //Adiciona a letra ao simbolo
        strncat(simbolo,ch,1);
        //Lê a próxima letra do arquivo
        pega_letra();
        //Se a letra lida for caracter, então um simbolo invalido ocorre
        //Ex.: 123a&1323. Isto é configurado como erro de caracter inválido
        if(letra_eh_caracter())
        {
          //Antes de sair do programa, imprime a lista de átomos até o erro
          imprime_lista();
          //Exibe mensagem de erro
          printf("Identificador invalido na linha %d\n",numLinha);
          printf("Saindo...\n");
          system("PAUSE");
          //Sai do programa
          exit(0);
        }
     }
     //Como a última letra lida pode não ser um valor numérico
     //a última posição válida para a leitura de números será a posição
     //apontada por anterior
     if(!letra_eh_numero()) fsetpos(arquivoPascal,&anterior);
     //Configura a classe do símbolo lido como número
     class_simb = numero;
}

//------------------------[ Função pega_letra_nao_branco ]------------------------
//Esta função deve verificar se o símbolo que está sendo formado é um identificador
void identificadores()
{

     //Enquanto a letra lida do arquivo fonte for uma letra válida ou um número válido
     while(letra_eh_numero() || letra_eh_caracter())
     {
         //Se encontrar o fim do arquivo acaba
         if(feof(arquivoPascal)) break;
         //Adiciona a letra ao símbolo
         strncat(simbolo,ch,1);
         //Lê a próxima letra do arquivo
         pega_letra();
     }
     //Como a última letra lida pode não ser um número ou letra
     //a última posição válida para a leitura de números será a posição
     //apontada por anterior
     if(!letra_eh_numero() && !letra_eh_caracter())fsetpos(arquivoPascal,&anterior);
     //Configura a classe do símbolo para palavra_reservada ou identificador
     if(eh_palavra_reservada()) class_simb = palavra_reservada;
     else class_simb = identificador;
}

//------------------------[ Função simb_especiais ]------------------------
//Esta função deve verificar se o símbolo que está sendo formado é um símbolo especial
void simb_especiais()
{

    char pr[2];      //armazenamento temporário das letras lidas

    //Se encontrar fim do arquivo, retorna
    if(feof(arquivoPascal)) return;
    //Senão
    else
    {
        //Caracteres simples
        if(ch[0] == '+' || ch[0] == '-' || ch[0] == ')' || ch[0] == '/' ||
        ch[0] == ',' || ch[0] == ';' || ch[0] == '=' ||ch[0] == '[' || ch[0] == '}' || ch[0] == ']')
        {
          //Configura a classe do símbolo formado como símbolo especial
          class_simb = simb_especial;
          strncat(simbolo,ch,1);
        }
        /*
        O caracter ( pode ser início do símbolo especial composto (* (início de comentário)
        Logo, uma verificação deve ser feita. Ps.: Caso o símbolo formado seja (*, todas as
        letras lidas entre (* e *) devem ser ignoradas.
        */
        else if(ch[0] == '(')
             {
               //Armazena a última letra na variável temporária
               pr[0] = ch[0];
               //Lê a próxima letra
               pega_letra();

               //Se a letra lida for *, então o símbolo é (*
               if(ch[0] == '*'){
                        //Seta comentario mais de uma linha como true, apenas para verificação
                        cmdul = TRUE;
                        //Armazena o número de linhas temporariamente
                        numLinhadump = numLinha;
                        //Classifica o símbolo como símbolo especial composto
                        class_simb = simb_especial_composto;
                        //Armazena a letra lida em pr[1]
                        pr[1] = ch[0];
                        //Cria o símbolo com as duas letras de pr[2]
                        strncat(simbolo,pr,2);
                        /*
                          Nesta parte do programa, busca-se pela letra '*', que inicializa o
                          símbolo de fecha comentário
                        */
                        //Lê a próxima letra
                        pega_letra();
                        //Enquanto a letra lida não for '*'
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
                                    //Lê a próxima letra
                                    pega_letra();
                        }//fim while

                        //Se o caracter lido for '*', seta o ponteiro do arquivo posição anterior
                        //do ponteiro do arquivo. Isto é necessário pois ao voltar para a função main
                        //o arquivo é lido novamente, e neste caso pularia o '*'
                        if(ch[0] == '*') fsetpos(arquivoPascal,&anterior);
               }
               //Se não for *, então reposiciona o ponteiro e classifica o símbolo ( formado
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
              //Armazena a última letra na variável temporária
              pr[0] = ch[0];
              //Lê a próxima letra
              pega_letra();
              //Se a letra for =, então cria-se o símbolo >= e classifica-o como símbolo especial composto
              //Caso contrário, o símbolo formado será > e será classificado como símbolo especial
              if(ch[0] == '='){ class_simb = simb_especial_composto; pr[1] = ch[0]; strncat(simbolo,pr,2);}
              else{ fsetpos(arquivoPascal,&anterior); class_simb = simb_especial; strncat(simbolo,pr,1); }

         }
         //Se a letra lida for =, verifica-se a possibilidade de formar =<
         else if(ch[0] == '=')
         {
              //Armazena a última letra na variável temporária
              pr[0] = ch[0];
              //Registra a última posição válida do ponteiro do arquivo fonte
             // fgetpos(arquivoPascal,&anterior);
              //Lê a próxima letra
              pega_letra();
              //Se a letra for <, então cria-se o símbolo =< e classifica-o como símbolo especial composto
              //Caso contrário, o símbolo formado será > e será classificado como símbolo especial
              if(ch[0] == '<'){ class_simb = simb_especial_composto; pr[1] = ch[0]; strncat(simbolo,pr,2);}
              else{ fsetpos(arquivoPascal,&anterior); class_simb = simb_especial; strncat(simbolo,pr,1); }

         }
         else if(ch[0] == '.')
         {
              //Armazena a última letra na variável temporária
              pr[0] = ch[0];
              //Lê a próxima letra
              pega_letra();
              //Se a letra for >, então cria-se o símbolo <> e classifica-o como símbolo especial composto
              //Caso contrário, o símbolo formado será > e será classificado como símbolo especial
              if(ch[0] == '.'){ class_simb = simb_especial_composto; pr[1] = ch[0]; strncat(simbolo,pr,2);}
              else if(ch[0]!= '@'){fsetpos(arquivoPascal,&anterior); class_simb = simb_especial; strncat(simbolo,pr,1); }
              else if(ch[0] == '@'){class_simb = simb_especial; strncat(simbolo,pr,1);}
         }
         //Se a letra lida for <, verifica-se a possibilidade de formar <>
         else if(ch[0] == '<')
         {
              //Armazena a última letra na variável temporária
              pr[0] = ch[0];
              //Lê a próxima letra
              pega_letra();
              //Se a letra for >, então cria-se o símbolo <> e classifica-o como símbolo especial composto
              //Caso contrário, o símbolo formado será > e será classificado como símbolo especial
              if(ch[0] == '>' || ch[0] == '='){ class_simb = simb_especial_composto; pr[1] = ch[0]; strncat(simbolo,pr,2);}
              else{ fsetpos(arquivoPascal,&anterior); class_simb = simb_especial; strncat(simbolo,pr,1); }

         }
         //Se a letra lida for *, verifica-se a possibilidade de formar *)
         else if(ch[0] == '*')
         {
              //Armazena a última letra na variável temporária
              pr[0] = ch[0];
              //Lê a próxima letra
              pega_letra();
              //Se a letra for ), então cria-se o símbolo *) e classifica-o como símbolo especial composto
              //Caso contrário, o símbolo formado será > e será classificado como símbolo especial
              if(ch[0] == ')'){ class_simb = simb_especial_composto; pr[1] = ch[0]; strncat(simbolo,pr,2);}
              else{ fsetpos(arquivoPascal,&anterior); class_simb = simb_especial; strncat(simbolo,pr,1); }

         }
         //Se a letra lida for :, verifica-se a possibilidade de formar :=
         else if(ch[0] == ':')
         {
              //Armazena a última letra na variável temporária
              pr[0] = ch[0];
              //Lê a próxima letra
              pega_letra();
              //Se a letra for =, então cria-se o símbolo := e classifica-o como símbolo especial composto
              //Caso contrário, o símbolo formado será > e será classificado como símbolo especial
              if(ch[0] == '='){ class_simb = simb_especial_composto; pr[1] = ch[0]; strncat(simbolo,pr,2);}
              else{ fsetpos(arquivoPascal,&anterior); class_simb = simb_especial; strncat(simbolo,pr,1); }

         }
                  //Se a letra lida for :, verifica-se a possibilidade de formar :=
         else if(ch[0] == '{')
         {
             //Seta comentario mais de uma linha como true, apenas para verificação
             cmdul = TRUE;
             //Armazena o número de linhas temporariamente
             numLinhadump = numLinha;
              //Armazena a última letra na variável temporária
              pr[0] = ch[0];
             //Lê a próxima letra
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
               //Lê a próxima letra
               pega_letra();
              }//fim while

              if(ch[0] == '}')
              {
                fsetpos(arquivoPascal,&anterior); class_simb = simb_especial; strncat(simbolo,pr,1);
              }
         }
         else{
              //Se a letra lida for um caracter inválido, gera erro
              imprime_lista();
              printf("Caracter invalido na linha %d\n",numLinha);
              printf("Saindo...\n");
              system("PAUSE");
              exit(0);
         }
   }
}
#endif
