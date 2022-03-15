#include "Stiva.h"
#define Top(stiva) (stiva->l)

Stiva initS()
{
    Stiva s = (Stiva)malloc(sizeof(struct stiva));
    s->l = initL();
    return s;
}

void elibS(Stiva s, void (*elib)(void*))
{
    eliberare(&s->l, elib);
    free(s);
}

void push(Stiva s, void *el)
{
    inserareFata(&s->l, el);
}

void* pop(Stiva s)
{
    return extragereFata(&s->l);
}

void* extragereS(Stiva s, void *el, int (*cmp)(void*,void*))
{
    Stiva aux = initS();

    while( Top(s) != NULL && cmp(el, Top(s)->info) != 0 )
        push(aux, pop(s));

    void *info = NULL;
    if( Top(s) != NULL )
    {
        info = Top(s)->info;
        pop(s);
    }

    while( Top(aux) != NULL )
        push(s, pop(aux));

    free(aux);
    return info;
}
