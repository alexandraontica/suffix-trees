/* ONȚICĂ Alexandra-Elena - 311CB */
#include "arb_suf_comp.h"
#include "arb_suf.h"

TArbComp AlocNodComp(char *eticheta_nod)
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
// scriu arborele de sufixe creat ca la cerintele anterioare
// folosind structura TArbComp ca sa il pot prelucra ulterior
{
    if (!t) {
        return NULL;
    }
    TArbComp t_comp = AlocNodComp(eticheta);
    if (!t_comp) {
        return NULL;
    }

    int i;
    for (i = 0; i < 27; ++i) {
        if (t->copii[i]) {
            char eticheta_urm[2] = "";
            eticheta_urm[0] = t->copii[i]->eticheta;
            eticheta_urm[1] = '\0';

            TArbComp *aux = (TArbComp *)realloc(t_comp->copii, (t_comp->nr_copii + 1) * sizeof(TArbComp));
            if (!aux) {
                DistrugeArbComp(&t_comp);
                return NULL;
            }
            t_comp->copii = aux;

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

void CompresareSufixe(TArbComp t)
{
    if (!t) {
        return;
    }

    if (t->nr_copii == 1 && strcmp(t->copii[0]->eticheta, "$")) {
        char *aux = (char *)realloc(t->eticheta, strlen(t->eticheta) + strlen(t->copii[0]->eticheta) + 1);
        if (!aux) {
            return;
        }

        t->eticheta = aux;
        strcat(t->eticheta, t->copii[0]->eticheta);

        TArbComp *aux2 = (TArbComp *)realloc(t->copii, t->copii[0]->nr_copii * sizeof(TArbComp));
        if (!aux2) {
            return;
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

        CompresareSufixe(t); // de justificat
    }

    int i;
    for (i = 0; i < t->nr_copii; i++) {
        CompresareSufixe(t->copii[i]);
    }
}

TArbComp ConstrArbComp(FILE *fin, int N)
{
    TArb t_suf = ConstrArb(fin, N);
    if (!t_suf) {
        return NULL;
    }

    TArbComp t = TransfTArbInTArbComp(t_suf, "#");
    DistrugeArb(&t_suf);
    if (!t) {
        return NULL;
    }

    CompresareSufixe(t);

    return t;
}

int AfisareArboreComp(FILE *fout, TArbComp t)
{
    TCoadaComp *c = InitQComp();
    if (!c) {
        return 0;
    }

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

    while (c->inc) {  // coada e nevida
        TArbComp n;
        ExtrQComp(c, &n);  // nu este nevoie sa verific ce returneaza functia, stiu ca c e coada nevida
        
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
{
    if (!(*t)) {
        return;
    }

    for (int i = 0; i < (*t)->nr_copii; i++) {
        DistrugeArbComp(&((*t)->copii[i]));
    }

    free((*t)->copii);
    free((*t)->eticheta);
    free(*t);
    *t = NULL;
}

// implementare coada pentru parcurgerea in latime:

TListaComp AlocCelulaComp(TArbComp n)
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
// creare coada vida
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
{
    ResetQComp(*c);
    free(*c);
    *c = NULL;
}
