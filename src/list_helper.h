#ifndef LIST_HELPER_H
#define LIST_HELPER_H

/** ***************************************************************************************
 Auxiliares para criação e manipulação de listas
   
*******************************************************************************************/

#include "arraylist.h"

/** NameList é apenas uma abstração para um arraylist que contém nomes (char *) */
typedef arraylist NameList;

/** Este typedef permite declarar ponteiros para funções que recebem um pointeiro
        de void e retornam um ponteiro para char (ver joinList para exemplo de uso).
    As funções StringConverter devem ser usadas para converter um determinado valor
        void * em string (elas devem alocar memória utilizando malloc ou similares).
*/
typedef char * (*StringConverter)(void *);

/** Cria uma lista, adicionando um valor caso este não seja nulo.*/
arraylist * createList(void * value);

/** Adiciona um valor 'value' a lista e retorna ela.*/
arraylist * appendList(arraylist * list, void * value);

NameList * createNameList(char * name);
NameList * appendNameList(NameList * namelist, char * name);

/** Concatena todos os itens de uma lista 'namelist' em uma string, 
    separandos com o símbolo 'symbol'.
    Caso o ponteiro para funções 'itemConverter' não seja nulo,
    ele será utilizado para converter cada item da lista (um void *) em
    string.  */
char * joinList(arraylist * namelist, const char * symbol, StringConverter itemConverter);

/** Destroi uma lista e todos os itens não nulos armazenados nela*/
void destroyList(arraylist * list);

#endif
