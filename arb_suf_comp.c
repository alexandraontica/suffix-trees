/* ONȚICĂ Alexandra-Elena - 311CB */
#include "arb_suf_comp.h"

TArbComp AlocNodComp(char *eticheta_nod)
// aloca un nod pentru arborele compact cu eticheta primita ca parametru
{
    TArbComp aux = (TArbComp)calloc(1, sizeof(TNodComp));
    if (aux) {
        aux->eticheta = strdup(eticheta_nod);
        if (!aux->eticheta) {
            free(aux);
            aux = NULL;
        } else {
            aux->copii = NULL;
            aux->nr_copii = 0;
        }
    }
    return aux;
}

TArbComp TransfTArbInTArbComp(TArb t, char *eticheta)
// transforma arborele creat ca la cerinta 1 intr-un arbore de tipul TArbComp
{
    if (!t) {
        return NULL;
    }

    // aloc nodul pe care vreau sa il adaug in arborele compact:
    TArbComp t_comp = AlocNodComp(eticheta);
    if (!t_comp) {
        return NULL;
    }

    // creez vectorul de copii:
    int i;
    for (i = 0; i < 27; ++i) {
        if (t->copii[i]) {  // daca exista copilul
            // transform eticheta in string:
            char eticheta_urm[2] = "";
            eticheta_urm[0] = t->copii[i]->eticheta;
            eticheta_urm[1] = '\0';

            // aloc suficiente pozitii in vectorul de copii ca sa pot sa il adaug pe cel curent:
            TArbComp *aux = (TArbComp *)realloc(t_comp->copii, (t_comp->nr_copii + 1) * sizeof(TArbComp));
            if (!aux) {
                DistrugeArbComp(&t_comp);
                return NULL;
            }
            t_comp->copii = aux;

            // transform subarborele cu radacina in copilul curent in tipul TArbComp:
            t_comp->copii[t_comp->nr_copii] = TransfTArbInTArbComp(t->copii[i], eticheta_urm);
            if (!t_comp->copii[t_comp->nr_copii]) {
                DistrugeArbComp(&t_comp);
                return NULL;
            }
            
            t_comp->nr_copii++;
        }
    }

    return t_comp;
}

int ComprimareSufixe(TArbComp t)
// transforma arborele intr-unul compact
{
    if (!t) {
        return 1;  // nimic de coprimat
    }

    // daca nodul curent are un singur fiu diferit de '$':
    if (t->nr_copii == 1 && strcmp(t->copii[0]->eticheta, "$")) {
        // eticheta fiului poate fi adaugata la cea a nodului curent:
        char *aux = (char *)realloc(t->eticheta, strlen(t->eticheta) + strlen(t->copii[0]->eticheta) + 1);
        if (!aux) {
            return 0;
        }

        t->eticheta = aux;
        strcat(t->eticheta, t->copii[0]->eticheta);

        // elimin fiul; copiii fiului devin copiii nodului curent:
        TArbComp *aux2 = (TArbComp *)realloc(t->copii, t->copii[0]->nr_copii * sizeof(TArbComp));
        if (!aux2) {
            return 0;
        }

        t->copii = aux2;

        TArbComp aux3 = t->copii[0];
        t->nr_copii = t->copii[0]->nr_copii;
        
        int i;
        for (i = 0; i < t->nr_copii; i++) {
            t->copii[i] = aux3->copii[i]; 
        }

        free(aux3->eticheta);
        free(aux3->copii);
        free(aux3);

        // apelez functia pentru nodul curent din nou deoarece
        // copiii fiului pe care doar ce l-am eliminat nu au fost verificati:
        int rez = ComprimareSufixe(t);
        if (!rez) {
            return 0;
        }
    }

    int i;
    for (i = 0; i < t->nr_copii; i++) {
        int rez = ComprimareSufixe(t->copii[i]);
        if (!rez) {
            return 0;
        }
    }

    return 1;
}

TArbComp ConstrArbComp(FILE *fin, int N)
// construieste arborele de sufixe compact
{
    // construieste arborele ca la cerinta 1:
    TArb t_suf = ConstrArb(fin, N);
    if (!t_suf) {
        return NULL;
    }

    // transforma arborele intr-unul de tipul TArbComp (tot atomic):
    TArbComp t = TransfTArbInTArbComp(t_suf, "#");
    DistrugeArb(&t_suf);
    if (!t) {
        return NULL;
    }

    // transforma arborele atomic intr-unul compact:
    int rez = ComprimareSufixe(t);
    if (!rez) {
        DistrugeArbComp(&t);
        return NULL;
    }

    return t;
}

int AfisareArboreComp(FILE *fout, TArbComp t)
// afiseaza arborele compact
// parcurgere in latime folosind o coada
{
    TCoadaComp *c = InitQComp();
    if (!c) {
        return 0;
    }

    // radacina nu o introduc in coada deoarece nu trebuie sa o afisez si pe ea
    // incep prin a adauga in coada nodurile de pe primul nivel
    int i;
    for (i = 0; i < t->nr_copii; i++) {
        if (t->copii && t->copii[i]) {
            int rez = IntrQComp(c, t->copii[i]);
            if (!rez) {
                DistrugeQComp(&c);
                return 0;
            }
        }
    }

    int rez = IntrQComp(c, NULL);  // marchez finalul unui nivel
        if (!rez) {
            DistrugeQComp(&c);
            return 0;
        }

    while (c->inc) {  // cat timp coada e nevida
        TArbComp n;
        ExtrQComp(c, &n);  // extrag un nod din coada
        
        // afisez informatia corespunzatoare nodului extras:
        if (!n) {
            fprintf(fout, "\n");

            if (c->inc) {
                int rez = IntrQComp(c, NULL);  // marchez finalul unui nivel
                if (!rez) {
                    DistrugeQComp(&c);
                    return 0;
                }
            }
        } else {
            fprintf(fout, "%s ", n->eticheta);

            // adaug in coada copiii nodului extras:
            int i;
            for (i = 0; i < n->nr_copii; i++) {
                if (n && n->copii[i]) {
                    int rez = IntrQComp(c, n->copii[i]);
                    if (!rez) {
                        DistrugeQComp(&c);
                        return 0;
                    }
                }
            }
        }
    }

    DistrugeQComp(&c);
    return 1;
}

void DistrugeArbComp(TArbComp *t)
// elibereaza memoria ocupata de arborele de sufixe compact
{
    if (!(*t)) {
        return;
    }

    int i;
    for ( i = 0; i < (*t)->nr_copii; i++) {
        DistrugeArbComp(&((*t)->copii[i]));
    }

    free((*t)->copii);
    free((*t)->eticheta);
    free(*t);
    *t = NULL;
}

// implementare coada pentru parcurgerea in latime:

TListaComp AlocCelulaComp(TArbComp n)
// aloca o celula (pentru coada)
{
    TListaComp aux = (TListaComp)calloc(1, sizeof(TCelulaComp));
    if (!aux) {
        return NULL;
    }

    aux->info = n;
    aux->urm = NULL;

    return aux;
}

TCoadaComp* InitQComp()
// creeaza coada vida
{
    TCoadaComp *c = (TCoadaComp *)calloc(1, sizeof(TCoadaComp));
    if (!c) {
        return NULL;
    }

    c->inc = NULL;
    c->sf = NULL;

    return c;
}

int IntrQComp(TCoadaComp *c, TArbComp n)
// intoarce 1: introducerea in coada a reusit, 0: in caz contrar
{ 
    TListaComp aux = AlocCelulaComp(n);
    if (!aux) {
        return 0;
    }

    if (!c->inc) {  // e coada vida
        c->inc = aux;
        c->sf = aux;
    } else {
        c->sf->urm = aux;
        c->sf = aux;
    }
  
    return 1;             
}

int ExtrQComp(TCoadaComp *c, TArbComp *n)
// intoarce 0: coada e vida, 1: s-a extras un nod din coada
{
    if (!c->inc) {  // e coada vida
        return 0;
    }

    *n = c->inc->info;  // pastrez informatia elementului extras
    if (c->inc == c->sf) {  // coada are un element
        free(c->inc);
        c->inc = NULL;  // devine coada vida
        c->sf = NULL;
    } else {
        TListaComp aux = c->inc;
        c->inc = c->inc->urm;
        free(aux);
    }

    return 1;
}

void ResetQComp(TCoadaComp *c)
// c devine coada vida
{
    if (!c) {
        return;
    }

    while (c->inc) {
        TListaComp aux = c->inc;
        c->inc = c->inc->urm;
        free(aux);
    }
    c->sf = NULL;
}

void DistrugeQComp(TCoadaComp **c)
// elibereaza memoria ocupata de coada
{
    ResetQComp(*c);
    free(*c);
    *c = NULL;
}
