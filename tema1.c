// Nicula Calin-MihaI 324CBb

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_SIZE 300

typedef struct Dir{
	char *name;   // numele directorului
	struct Dir* parent;  // pointer catre parintele directorului(null pentru radacina)
	struct File* head_children_files;  // pointer catre primul element de tip File din interiorul directorului
	struct Dir* head_children_dirs; // pointer catre primul element de tip Dir din interiorul directorului
	struct Dir* next; // pointer catre urmatorul element din lista in care se afla directorul
}Dir; // structura de tip director

typedef struct File {
	char *name;  // numele fisierului
	struct Dir* parent; // pointer catre directorul pe
	struct File* next; // pointer catre urmatorul element din lista de fisiere
}File; // structura de tip fisier

// Creeaza un fisier
void touch (Dir* parent, char* name)
{
    // In cazul in care 
    if( parent->head_children_files == NULL )
    {
        File *cel;                  // "cel" de la celula
        cel = malloc(sizeof(File)); // Aici o sa creez celula in care se afla noul fisier pentru a fi atasata
        cel->name = malloc((strlen(name) + 1)*sizeof(char));
        strcpy(cel->name, name);
        cel->next = NULL;
        cel->parent = parent;
        parent->head_children_files = cel;
        return; 
    }

    if( !strcmp(parent->head_children_files->name, name) ) // Verific daca exita deja, in primul element din lista
    {
        printf("File already exists\n");
        return;
    }

    File *f = parent->head_children_files; 
    while (f->next)                                        // Acest while are scopul de parcurgere a listei
                                                          // Si se opreste la ultimul element nenul
    {
        if (!strcmp(name, f->name))                        // Verific daca exita deja
        {
            printf("File already exists\n");               // Verific daca exista deja in lista
            return;
        }
        f = f->next;
    }
    File *cel;
    cel = malloc(sizeof(File));
    cel->name = malloc((strlen(name) + 1)*sizeof(char));   // marimea numelui + 1, acel 1 tine NULL-ul de la final
    strcpy(cel->name, name);
    cel->next = NULL;
    cel->parent = parent;
    f->next = cel;
}

void mkdir (Dir* parent, char* name)
{
    if( parent->head_children_dirs == NULL ) // Verific daca lista este goala
    {
        Dir *cel;
        cel = malloc(sizeof(Dir));
        cel->name = malloc((strlen(name) + 1)*sizeof(char));
        strcpy(cel->name, name);
        cel->next = NULL;
        cel->parent = parent;
        cel->head_children_files = NULL;
        cel->head_children_dirs = NULL;
        parent->head_children_dirs = cel;
        return;
    }

    if(!strcmp(parent->head_children_dirs->name, name)) // Verific daca exita deja, in primul element din lista
    {
        printf("Directory already exists\n");
        return;
    }

    Dir *d = parent->head_children_dirs;
    while (d->next)
    {
        if (!strcmp(name, d->name))
        {
            printf("Directory already exists\n");
            return;
        }
        d = d->next;
    }

    Dir *cel;
    cel = malloc(sizeof(Dir));
    cel->name = malloc((strlen(name) + 1)*sizeof(char));
    strcpy(cel->name, name);
    cel->next = NULL;
    cel->parent = parent;
    cel->head_children_files = NULL;
    cel->head_children_dirs = NULL;
    d->next = cel;
}

void ls (Dir* parent)
{
    Dir *d;
    d = parent->head_children_dirs;
    while (d)
    {
        printf("%s\n", d->name);
        d = d->next;
    }
    File *f;
    f = parent->head_children_files;
    while (f)
    {
        printf("%s\n", f->name);
        f = f->next;
    }
}

void rm (Dir* parent, char* name)
{
    if( parent->head_children_files == NULL )
    {
        printf("Could not find the file\n");
        return;
    }

    if (!strcmp(name, parent->head_children_files->name)) // Verific daca exita deja, in primul element din lista
    {
        File *cel = parent->head_children_files->next;
        free(parent->head_children_files->name);
        free(parent->head_children_files);
        parent->head_children_files = cel;
        return;
    }

    File *f = parent->head_children_files;
    while (f->next)
    {
        if (!strcmp(name, (f->next)->name))
        {
            File *cel = f->next->next;
            free(f->next->name);
            free(f->next);
            f->next = cel;
            return;
        }
        f = f->next;
    }
    printf("Could not find the file\n");
}

void stergereFoldere(Dir* parent); 
void stergereFolder (Dir* parent)
{
    free(parent->name);
    stergereFoldere(parent->head_children_dirs);
    while(parent->head_children_files)
    {
        File *cel = parent->head_children_files->next;
        rm(parent, parent->head_children_files->name);
        parent->head_children_files = cel;
    }
    free(parent);
}

void stergereFoldere (Dir* parent)
{
    while(parent)
    {
        Dir *cel = parent->next;
        stergereFolder(parent);
        parent = cel;
    }
}

void rmdir (Dir* parent, char* name)
{
    if( parent->head_children_dirs == NULL ) // Verific daca lista este goala, sa stiu daca am ce sa sterg
    {
        printf("Could not find the dir\n");
        return;
    }

    if (!strcmp(name, parent->head_children_dirs->name)) // Verific daca exita deja, in primul element din lista
    {
        Dir *cel = parent->head_children_dirs->next;
        stergereFolder(parent->head_children_dirs);
        parent->head_children_dirs = cel;
        return;
    }

    Dir *d = parent->head_children_dirs;
    while (d->next) 
    {
        if (!strcmp(name, (d->next)->name)) // Verific urmatorul element din lista fiindca 
                                           // lista este simplu inlantuita, adica nu pot sa ma intorc, 
                                          // iar eu am nevoie de elementul precedent celui care trebuie sters
        {
            Dir *cel = d->next->next;
            stergereFolder(d->next);
            d->next = cel;
            return;
        }
        d = d->next;
    }
    // Daca niciun folder nu se potriveste, atunci nu avem ce sterge *shrug*
    printf("Could not find the dir\n");
}

void cd(Dir** target, char* name)
{
    if (!strcmp(name, "..") && (*target)->parent == NULL) // Verific daca 'target' == 'home'
        return;
    if (!strcmp(name, "..")) // daca nu s a intrat pe primul if, putem urca
    {
        *target = (*target)->parent;
        return;
    }
    Dir *aux = (*target)->head_children_dirs;
    while (aux)                        // Chiar daca toate folderele de aici au acelasi parent
    {
        if (!strcmp(aux->name, name))  // Verificam daca macar exista 'name' aici
        {
            *target = aux;
            return;
        }
        aux = aux->next;
    }
    printf("No directories found!\n"); // Nu s a gasit
}

void tree(Dir* target, int level)
{
    Dir* aux = target->head_children_dirs;
    while (aux) // Directoare
    {
        for (int i = 1; i <= level; i++)
            printf("    ");
        printf("%s\n", aux->name);
        tree(aux, level + 1);
        aux = aux->next;
    }
    File* Aux = target->head_children_files;
    while (Aux) // Fisiere
    {
        for (int i = 1; i <= level; i++)
            printf("    ");
        printf("%s\n", Aux->name);
        Aux = Aux->next;
    }
}

char *pwd (Dir* target)
{
    char *alee;
    if (!target->parent)
    {
        alee = malloc(sizeof(char) * MAX_INPUT_LINE_SIZE);
	    strcpy(alee, "/");
        strcat(alee, target->name);

        return alee;
    }
    alee = pwd(target->parent);
    strcat(alee, "/");
    strcat(alee, target->name);
    return alee;
}

void stop (Dir* target)
{
    stergereFolder(target);
    exit(0);
}

void mv(Dir* parent, char* oldname, char* newname){
    Dir* auxD = parent->head_children_dirs;
    int okoldnameF = 1, okoldnameD = 1, oknewname = 0; // Ca sa fie clar si usor de urmarit,
                                                      // daca 'ok'-urile sunt pe 0 inseamna ca este totul in regula,
                                                     // iar daca sunt pe 1 este o buba
    while (auxD) // Aici facem verificarea pentru directoare
    {
        if (!strcmp(newname, auxD->name))
        {
            oknewname = 1;
        }
        if (!strcmp(oldname, auxD->name))
        {
            okoldnameD = 0;
        }
        auxD = auxD->next;
    }
    File* auxF = parent->head_children_files; 
    while (auxF) // Aici pentru fisiere
    {
        if (!strcmp(newname, auxF->name))
        {
            oknewname = 1;

        }
        if (!strcmp(oldname, auxF->name))
        {
            okoldnameF = 0;
        }
        auxF = auxF->next;
    }
    if (okoldnameD && okoldnameF) // In cazul in care nu a gasit niciun tip cu numele 'oldname'
    {
        printf("File/Director not found\n");
        return;
    }
    else if (oknewname)           // In cazul in care exista deja un 'newname'
    {
        printf("File/Director already exists\n");
        return;
    }
    else // In cazul in care exista macar un 'okoldname' pe 0
    {
        if (!okoldnameD){ // Aici verific pentru directoare
            auxD = parent->head_children_dirs;
            Dir *ant = NULL;
            while (auxD) // Cautam pozitia 'oldname'
            {   
                if (!strcmp(oldname, auxD->name)){
                    break;  // Iese cand il gaseste
                }
                ant = auxD; // Se retine precedentul aici
                auxD = auxD->next;
            }            
            if (ant) // Verific daca nu sunt in primul element
            {
                ant->next = auxD->next;   
            }
            else
            {
                if (auxD->next) // Verific sa nu fie singurul element din lista
                parent->head_children_dirs = auxD->next;
                else // Unicul element din lista
                {
                    strcpy(parent->head_children_dirs->name, newname);
                    return;
                }
            }

            Dir *cel = auxD;
            while (auxD->next) // Ma duc pana n capatul listei sa termin ce am inceput
            {   
                auxD = auxD->next;
            }
            auxD->next = cel;
            cel->next = NULL;
            strcpy(cel->name, newname);
        }
        else              // Aici pentru fisiere
        {
            auxF = parent->head_children_files;
            File *ant = NULL;
            while (auxF)
            {   
                if (!strcmp(oldname, auxF->name)){
                    break;  // Iese cand il gaseste
                }
                ant = auxF; // Se retine precedentul aici
                auxF = auxF->next;
            }
            if (ant) // Verific daca nu sunt in primul element
            {
                ant->next = auxF->next;   
            }
            else
            {
                if(auxF->next) // Verific sa nu fie singurul element din lista
                parent->head_children_files = auxF->next;
                else // Unicul element din lista
                {
                    strcpy(parent->head_children_files->name, newname);
                    return;
                }
            }
            File *cel = auxF;
            while (auxF->next) // Ma duc pana n capatul listei sa termin ce am inceput
            {   
                auxF = auxF->next;
            }
            auxF->next = cel;
            cel->next = NULL;
            strcpy(cel->name, newname);
        }
    }
}

int main () {
    Dir *home = malloc(sizeof(Dir));
    char *cuv, *oldname;
    home->name = malloc(sizeof(char) * 5); // Ia nastere directorul home '/'
    strcpy(home->name, "home");
    home->parent = NULL;
    home->next = NULL;
    home->head_children_files = NULL;
    home->head_children_dirs = NULL;
    Dir *aici = home;                      // Aici se retine directorul in care suntem
    char *eu = malloc(sizeof(char) * (MAX_INPUT_LINE_SIZE + 1));
    do
    {
        fgets(eu, MAX_INPUT_LINE_SIZE, stdin); // Aici se citesc liniile
        eu[strlen(eu) - 1] = '\0';            // Eliminam ultimul caracter care este un 'ENTER'
        cuv = strtok(eu, " ");               // Extrag primul sau singurul cuvant din linie
        if (!strcmp(cuv, "mkdir"))
        {
            cuv = strtok(NULL, " ");
            mkdir(aici, cuv);
        }
        if (!strcmp(cuv, "touch"))
        {
            cuv = strtok(NULL, " ");
            touch(aici, cuv);
        }
        if (!strcmp(cuv, "ls")){
            ls(aici);}
        if (!strcmp(cuv, "rm"))
        {
            cuv = strtok(NULL, " ");
            rm(aici, cuv);
        }
        if (!strcmp(cuv, "rmdir"))
        {
            cuv = strtok(NULL, " ");
            rmdir(aici, cuv);
        }
        if (!strcmp(cuv, "cd"))
        {
            cuv = strtok(NULL, " ");
            cd(&aici, cuv);
        }
	    if(!strcmp(cuv, "tree"))
	{
	    tree(aici, 0);                    // Nivelul de unde pornim este 0
	}
	    if(!strcmp(cuv, "pwd"))
	{
	    char* alee = pwd(aici);            // Fac asta pentru ca functia este >char*< pwd
	    printf("%s\n", alee);
	    free(alee);
	}
        if (!strcmp("stop", eu))
    {
        free(eu);
        stop(home);
    }
        if(!strcmp(cuv, "mv"))
        {
            oldname = strtok(NULL, " ");
            cuv = strtok(NULL, " ");       // Aici extrag 'newname'
            mv(aici, oldname, cuv);        // 'cuv' este 'newname'
        }
    } while (1);
    return 0;
}
