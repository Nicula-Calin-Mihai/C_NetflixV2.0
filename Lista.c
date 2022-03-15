#include "Lista.h"

TLista initL()
{
    return NULL;
}

TCelula* alocareCelula(void* info, TCelula *urm)
{
    TCelula* c = (TCelula*)malloc(sizeof(TCelula));
    c->info = info;
    c->urm = urm;
    return c;
}

int inserare(TLista *l, void *el)
{
    if( *l == NULL )
    {
        *l = alocareCelula(el, NULL);
        return 1;
    }
    else
    {
        int pos = 2;
        TCelula *c = *l;
        while( c->urm != NULL )
        {
            c = c->urm;
            pos++;
        }
        c->urm = alocareCelula(el, NULL);
        return pos;
    }
}

void inserareFata(TLista *l, void *el)
{
    if( *l == NULL )
        inserare(l, el);
    else
        *l = alocareCelula(el, *l);
}

int inserareOrd(TLista *l, void *el, int (*cmp)(void*,void*))
{
    if( *l == NULL )
    {
        inserare(l, el);
        return 1;
    }

    if( cmp(el, (*l)->info) < 0 )
    {
        inserareFata(l, el);
        return 1;
    }
    else
    {
        int pos = 2;
        TCelula *c = *l;
        while( c->urm != NULL && cmp(el, c->urm->info) > 0 )
        {
            c = c->urm;
             pos++;
        }
        c->urm = alocareCelula(el, c->urm);
        return pos;
    }
}

void eliberare(TLista *l, void (*elib)(void*))
{
    if( *l == NULL )
        return;
    eliberare( &(*l)->urm, elib );
    elib((*l)->info);
    free(*l);
}

void* extragere(TLista *l, void *el, int (*cmp)(void*,void*))
{
    if( *l == NULL )
        return NULL;

    if( cmp((*l)->info, el) == 0 )
        return extragereFata(l);
    else
    {
        TCelula *c = *l;
        while( c->urm != NULL && cmp(c->urm->info, el) != 0 )
            c = c->urm;

        if( c->urm != NULL )
        {
            TCelula *del = c->urm;
            void *info = del->info;
            c->urm = del->urm;
            free(del);
            return info;
        }
        return NULL;
    }
}

void* extragereFata(TLista *l)
{
    if(*l == NULL)
        return NULL;

    TCelula *c = *l;
    *l = (*l)->urm;
    void* info = c->info;
    free(c);
    return info;
}

void* cautare(TLista *l, void *el, int (*cmp)(void*,void*))
{
    TCelula *c = *l;
    while( c != NULL && cmp(c->info, el) != 0 )
        c = c->urm;

    if( c != NULL )
        return c->info;
    return NULL;
}

void afisare(TLista l, void (*print)(void*, FILE*), FILE* f)
{
    fprintf(f, "[");
    TCelula *c = l;
    while( c != NULL )
    {
        print(c->info, f);
        if( c->urm != NULL )
            fprintf(f, ", ");
        c = c->urm;
    }
    fprintf(f, "]");
}

void inserareTop10 (TLista *l, void *el, int poz)
{
    if (!*l)
        inserare(l, el);
    else
    if (poz == 1)
        inserareFata(l, el);
    else
    {
        int i = 2;
        TCelula *del = *l;
        while (i != poz)
        {
            del = del->urm;
            i++;
        }
        del->urm = alocareCelula(el,del->urm);
    }
}