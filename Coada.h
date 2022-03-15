#ifndef COADA_H_INCLUDED
#define COADA_H_INCLUDED
#include "Lista.h"

typedef struct coada
{
    TLista l;
}*Coada;

Coada initC();
void elibC(Coada c, void (*elib)(void*));

int enqueue(Coada c, void* el);
void* dequeue(Coada c);
void* extragereC(Coada c, void *el, int (*cmp)(void*,void*));

#endif // COADA_H_INCLUDED
