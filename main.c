// Nicula Calin-Mihai - 314CBb
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Lista.h"
#include "Stiva.h"
#include "Coada.h"

typedef struct serial
{
	int ID;
	char *nume;
	float rating;
	int nrsez;
	int durata;
	Coada sezoane;
} Serial;

typedef struct sezon
{
	int nrep;
	Coada episoade;
} Sezon;

typedef struct episod
{
	int durata;
} Episod;

void elibEpisod(void *a)
{
    Episod *e = (Episod*)a;
    free(e);
}

void elibSezon(void *a)
{
    Sezon *s = (Sezon*)a;
    elibC(s->episoade, elibEpisod);
    free(s);
}

void elibSerial(void *a)
{
    Serial *s = (Serial*)a;
    free(s->nume);
    elibC(s->sezoane, elibSezon);
    free(s);
}

void afisareSerial(void *a, FILE *f)
{
    Serial *s = (Serial*)a;
	// exact o zecimala
    fprintf(f, "(%s, %.1f)", s->nume, s->rating);
}

int comparaSerial(void *a, void *b)
{
    Serial *sa = (Serial*)a;
    Serial *sb = (Serial*)b;

    if( sa->rating == sb->rating )
        return strcmp(sa->nume, sb->nume);
    else
    {
        if(sb->rating > sa->rating)
            return 1;
        else
            return -1;
    }
}

int comparaNume(void *a, void *b)
{
	Serial *sa = (Serial*)a;
    Serial *sb = (Serial*)b;

	return strcmp(sa->nume, sb->nume);
}

void updateTop(TLista *l)
{
    int i;
    TCelula *c = *l;
    for (i = 0; i < 9 && (c->urm != NULL);i++)
    {
        c = c->urm;
    }
	// refac lista de 10 seriale
    if (c->urm != NULL)
    {
        elibSerial(c->urm->info);
        free(c->urm);
        c->urm = NULL;
    }
}

int main(int argc, char **argv)
{
	void *cond;

    TLista tendinte = initL();
    TLista documentare = initL();
    TLista tutoriale = initL();
    TLista top10 = initL();
	Stiva currently_watching = initS();
	Stiva history = initS();
	Coada watch_later = initC();

	Serial *s;

	char *token;
	int i, j, poz;
	char buffer[1005];

	FILE *pFile, *jFile;

  	// Extragem din argumentul dat la compilare al 2-lea si al 3-lea cuvant
 	 // Unde avem numele fisierelor
  	// Deschidem fisierele pentru Read si Write
  	pFile = fopen(argv[1], "r");
  	jFile = fopen(argv[2], "w");

	while(fgets(buffer, 1000, pFile) != NULL)
	{
		// elimin '\n'
		buffer[strlen(buffer) - 1] = '\0';
		token = strtok(buffer, " ");
		// Add 
		if (!strcmp(token, "add"))
		{
			s = malloc(sizeof(Serial));
			s->durata = 0;
			// asez descrescator dupa rating, lexicografic dupa nume
			// ID
			token = strtok(NULL, " ");
			s->ID = atoi(token);
			// Nume
			token = strtok(NULL, " ");
			s->nume = (char*)malloc(33*sizeof(char));
			strcpy(s->nume,token);
			// Rating
			token = strtok(NULL, " ");
			s->rating = atof(token);
			// Nr Sezoane
			token = strtok(NULL, " ");
			s->nrsez = atoi(token);
			s->sezoane = initC();
			for (i = 0; i < s->nrsez; i++)
			{
				Sezon *sez = malloc(sizeof(Sezon));

				//Nr episoade
				token = strtok(NULL, " ");
				sez->nrep = atoi(token);
				sez->episoade = initC();
				for (j = 0; j < sez->nrep; j++)
				{
					Episod *ep = malloc(sizeof(Episod));
					// Durata/episod
					token = strtok(NULL, " ");
					ep->durata = atoi(token);
					s->durata = s->durata + ep->durata;
					enqueue(sez->episoade, (void*)ep);
				}
				enqueue(s->sezoane, (void*)sez);
			}
			// Tendinte
			if (s->ID == 1)
			{
				poz = inserareOrd(&tendinte, (void*)s, comparaSerial);
			}
			else
			// Documentare
			if (s->ID == 2)
			{
				poz = inserareOrd(&documentare, (void*)s, comparaSerial);
			}
			else
			// Tutoriale
			{
				poz = inserareOrd(&tutoriale, (void*)s, comparaSerial);
			}
			fprintf(jFile, "Serialul %s a fost adaugat la pozitia %d.\n", s->nume, poz);
		}
		else
		// Add sez
		if (!strcmp(token, "add_sez"))
		{
			// Nume
			s = malloc(sizeof(Serial));
			// Nume
			token = strtok(NULL, " ");
			s->nume = (char*)malloc(33*sizeof(char));
			strcpy(s->nume, token);
			// Find serial
			cond = cautare(&tendinte, (void*)s, comparaNume);
			if (cond == NULL)
			{
				cond = cautare(&documentare, (void*)s, comparaNume);
			}
			if (cond == NULL)
			{
				cond = cautare(&tutoriale, (void*)s, comparaNume);
			}
			if (cond == NULL)
			{
				cond = cautare(&top10, (void*)s, comparaNume);
			}
			if (cond == NULL)
			{
				cond = cautare(&currently_watching->l, (void*)s, comparaNume);
			}
			if (cond == NULL)
			{
				cond = cautare(&watch_later->l, (void*)s, comparaNume);
			}
			if (cond == NULL)
			{
				cond = cautare(&history->l, (void*)s, comparaNume);
			}
			Sezon *sez = malloc(sizeof(Sezon));

			//Nr episoade
			token = strtok(NULL, " ");
			sez->nrep = atoi(token);
			sez->episoade = initC();
			for (j = 0; j < sez->nrep; j++)
			{
				Episod *ep = malloc(sizeof(Episod));
				// Durata/episod
				token = strtok(NULL, " ");
				ep->durata = atoi(token);
				((Serial*)(cond))->durata = ((Serial*)(cond))->durata + ep->durata;
				enqueue(sez->episoade, (void*)ep);
			}
			enqueue(((Serial*)(cond))->sezoane, (void*)sez);
			fprintf(jFile, "Serialul %s are un sezon nou.\n", s->nume);
			free(s->nume);
			free(s);
			(((Serial*)(cond))->nrsez)++;
		}
		else
		// Add top
		if (!strcmp(token, "add_top"))
		{
			s = malloc(sizeof(Serial));
			// asez descrescator dupa rating, lexicografic dupa nume
			// ID = 4
			s->ID = 4;
			s->durata = 0;
			token = strtok(NULL, " ");
			poz = atoi(token);
			
			// Nume
			token = strtok(NULL, " ");
			s->nume = (char*)malloc(33*sizeof(char));
			strcpy(s->nume,token);
			// Rating
			token = strtok(NULL, " ");
			s->rating = atof(token);
			// Nr Sezoane
			token = strtok(NULL, " ");
			s->nrsez = atoi(token);
			s->sezoane = initC();
			for (i = 0; i < s->nrsez; i++)
			{
				Sezon *sez = malloc(sizeof(Sezon));

				//Nr episoade
				token = strtok(NULL, " ");
				sez->nrep = atoi(token);
				sez->episoade = initC();
				for (j = 0; j < sez->nrep; j++)
				{
					Episod *ep = malloc(sizeof(Episod));
					// Durata/episod
					token = strtok(NULL, " ");
					ep->durata = atoi(token);
					s->durata = s->durata + ep->durata;
					enqueue(sez->episoade, (void*)ep);
				}
				enqueue(s->sezoane, (void*)sez);
			}
			inserareTop10(&top10, (void*)s, poz);
			// refac de 10 seriale top10
			updateTop(&top10);
			fprintf(jFile, "Categoria top10: ");
			afisare(top10, afisareSerial, jFile);
			fprintf(jFile, ".\n");
		}
		else
		// Later
		if (!strcmp(token, "later"))
		{
			s = malloc(sizeof(Serial));
			// Nume
			token = strtok(NULL, " ");
			s->nume = (char*)malloc(33*sizeof(char));
			strcpy(s->nume, token);
			
			cond = extragere(&tendinte, (void*)s, comparaNume);
			if (cond == NULL)
			{
				cond = extragere(&documentare, (void*)s, comparaNume);
			}
			if (cond == NULL)
			{
				cond = extragere(&tutoriale, (void*)s, comparaNume);
			}
			if (cond == NULL)
			{
				cond = extragere(&top10, (void*)s, comparaNume);
			}
			poz = enqueue(watch_later, cond);
			fprintf(jFile, "Serialul %s se afla in coada de asteptare pe pozitia %d.\n", s->nume, poz);
			free(s->nume);
			free(s);
		}
		else
		// Watch
		if (!strcmp(token, "watch"))
		{
			s = malloc(sizeof(Serial));
			// Nume
			token = strtok(NULL, " ");
			s->nume = (char*)malloc(33*sizeof(char));
			strcpy(s->nume, token);
			
			cond = extragere(&tendinte, (void*)s, comparaNume);
			if (cond == NULL)
			{
				cond = extragere(&documentare, (void*)s, comparaNume);
			}
			if (cond == NULL)
			{
				cond = extragere(&tutoriale, (void*)s, comparaNume);
			}
			if (cond == NULL)
			{
				cond = extragere(&top10, (void*)s, comparaNume);
			}
			if (cond == NULL)
			{
				cond = extragereS(currently_watching, (void*)s, comparaNume);
			}
			if (cond == NULL)
			{
				cond = extragereC(watch_later, (void*)s, comparaNume);
			}
			if (cond == NULL)
			{
				cond = extragereS(history, (void*)s, comparaNume);
			}
			Serial *ser = ((Serial*)(cond));
			// Durata
			token = strtok(NULL, " ");
			if (ser->durata <= atoi(token))
			{
				push(history, cond);
				fprintf(jFile, "Serialul %s a fost vizionat integral.\n", ser->nume);
			}
			else
			{
				ser->durata = ser->durata - atoi(token);
				push(currently_watching, cond);
			}
			free(s->nume);
			free(s);
		}
		else
		// Show
		if (!strcmp(token, "show"))
		{
			token = strtok(NULL, " ");
			// Top10
			if (!strcmp(token, "top10"))
			{
				fprintf(jFile, "Categoria top10: ");
				afisare(top10, afisareSerial, jFile);
				fprintf(jFile, ".\n");
			}
			else
			// Later
			if (!strcmp(token, "later"))
			{
				fprintf(jFile, "Categoria later: ");
				afisare(watch_later->l, afisareSerial, jFile);
				fprintf(jFile, ".\n");
			}
			else
			// Watching
			if (!strcmp(token, "watching"))
			{
				fprintf(jFile, "Categoria watching: ");
				afisare(currently_watching->l, afisareSerial, jFile);
				fprintf(jFile, ".\n");
			}
			else
			// History
			if (!strcmp(token, "history"))
			{
				fprintf(jFile, "Categoria history: ");
				afisare(history->l, afisareSerial, jFile);
				fprintf(jFile, ".\n");
			}
			else
			{
				if (atoi(token) == 1)
				{
					fprintf(jFile, "Categoria 1: ");
					afisare(tendinte, afisareSerial, jFile);
					fprintf(jFile, ".\n");
				}
				else
				if (atoi(token) == 2)
				{
					fprintf(jFile, "Categoria 2: ");
					afisare(documentare, afisareSerial, jFile);
					fprintf(jFile, ".\n");
				}
				else
				{
					fprintf(jFile, "Categoria 3: ");
					afisare(tutoriale, afisareSerial, jFile);
					fprintf(jFile, ".\n");
				}
			}
		}

	}

	// Le inchidem
	fclose(pFile);
	fclose(jFile);
	// Eliberam structurile alocate
	eliberare(&tendinte, elibSerial);
	eliberare(&documentare, elibSerial);
	eliberare(&tutoriale, elibSerial);
	eliberare(&top10, elibSerial);
	elibC(watch_later, elibSerial);
	elibS(currently_watching, elibSerial);
	elibS(history, elibSerial);

    return 0;
}
