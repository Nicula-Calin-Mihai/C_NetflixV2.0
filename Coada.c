#include "Coada.h"
#define Front(coada) (coada->l)

Coada initC()
{
    Coada c = (Coada)malloc(sizeof(struct coada));
    c->l = initL();
    return c;
}

void elibC(Coada c, void (*elib)(void*))
{
    eliberare(&c->l, elib);
    free(c);
}

int enqueue(Coada c, void* el)
{
    return inserare(&c->l, el);
}

void* dequeue(Coada c)
{
    return extragereFata(&c->l);
}

void* extragereC(Coada c, void *el, int (*cmp)(void*,void*))
{
    Coada aux = initC();

    while( Front(c) != NULL && cmp(el, Front(c)->info) != 0 )
        enqueue(aux, dequeue(c));

    void *info = NULL;
    if( Front(c) != NULL )
    {
        info = Front(c)->info;
        dequeue(c);
    }

    while( Front(c) != NULL )
        enqueue(aux, dequeue(c));
    while( Front(aux) != NULL )
        enqueue(c, dequeue(aux));

    free(aux);
    return info;
}
