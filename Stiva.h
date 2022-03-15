#ifndef STIVA_H_INCLUDED
#define STIVA_H_INCLUDED
#include "Lista.h"

typedef struct stiva
{
    TLista l;
}*Stiva;

Stiva initS();
void elibS(Stiva s, void (*elib)(void*));

void push(Stiva s, void *el);
void* pop(Stiva s);
void* extragereS(Stiva s, void *el, int (*cmp)(void*,void*));
#endif // STIVA_H_INCLUDED
