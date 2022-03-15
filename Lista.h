#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED
#include <stdlib.h>
#include <stdio.h>

typedef struct celula
{
	void *info;
 	struct celula * urm;
} TCelula, *TLista;

TLista initL();
void eliberare(TLista *l, void (*elib)(void*));

int inserare(TLista *l, void *el);
void inserareFata(TLista *l, void *el);
int inserareOrd(TLista *l, void *el, int (*cmp)(void*,void*));
void inserareTop10 (TLista *l, void *el, int poz);

void* extragere(TLista *l, void *el, int (*cmp)(void*,void*));
void* extragereFata(TLista *l);
void* cautare(TLista *l, void *el, int (*cmp)(void*,void*));

void afisare(TLista l, void (*print)(void*, FILE*), FILE* f);

#endif // LISTA_H_INCLUDED
