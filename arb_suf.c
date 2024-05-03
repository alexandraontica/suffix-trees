/* ONȚICĂ Alexandra-Elena - 311CB */
#include "arb_suf.h"

void CloseFiles(FILE *fin, FILE *fout)
// inchide fisierele de input si output
{
    fclose(fin);
    fclose(fout);
}

TArb AlocNod(char eticheta_nod)
// aloca un nod cu eticheta primita ca parametru
{
    TArb aux = (TArb)calloc(1, sizeof(TNod));
    if (aux) {
        aux->eticheta = eticheta_nod;

        int i;
        for (i = 0; i < 27; i++) {
            aux->copii[i] = NULL;
        }
    }
    return aux;
}

TArb ConstrArb(FILE *fin, int N)
// construieste arborele de sufixe
{
    // aloc radacina:
    TArb t = AlocNod('#');  // eticheta radacinii este irelevanta, nu o folosesc
    if (!t) {
        return NULL;
    }

    // adaug in arbore nodul '$'
    t->copii[0] = AlocNod('$');
    if (!t->copii[0]) {
        free(t);
        return NULL;
    }

    int i;
    for (i = 0; i < N; i++) {
        char cuvant[21];  // presupun ca nu exista cuvinte cu mai mult de 20 de litere
        fscanf(fin, "%s", cuvant);

        // parcurg sufixele cuvantului curent:
        int len_cuv = strlen(cuvant);
        int j;
        for (j = len_cuv - 1; j >= 0; j--) {
            char *sufix = cuvant + j;
            int len_suf = strlen(sufix);

            // verific daca sufixul sau parti din acesta exista deja in arbore;
            // ma opresc la prima litera din sufix care nu mai apare in ordinea
            // din cuvant in arbore:
            TArb aux = t;
            int k;
            for (k = 0; k < len_suf; k++) {
                if (aux->copii[sufix[k] - 'a' + 1]) {
                    aux = aux->copii[sufix[k] - 'a' + 1];
                } else {
                    break;
                }
            }

            // adaug portiunea din sufix care nu exista deja in arbore:
            while (k < len_suf) {
                aux->copii[sufix[k] - 'a' + 1] = AlocNod(sufix[k]);
                if (!aux->copii[sufix[k] - 'a' + 1]) {
                    DistrugeArb(&t);
                    return NULL;
                }

                aux = aux->copii[sufix[k] - 'a' + 1];
                k++;
            }

             // am ajuns la finalul sufixului
             // daca nu exista deja nodul '$' il adaug
            if (!aux->copii[0]) {
                aux->copii[0] = AlocNod('$');
                if (!aux->copii[0]) {
                    DistrugeArb(&t);
                    return NULL;
                }
            }
        }
    }

    return t;
}

void DistrugeArb(TArb *t)
// elibereaza memoria ocupata de arborele de sufixe
{
    if (!(*t)) {
        return;
    }

    int i;
    for (i = 0; i < 27; i++) {
        DistrugeArb(&((*t)->copii[i]));
    }

    free(*t);
    *t = NULL;
}

int AfisareArbore(FILE *fout, TArb t)
// afiseaza arborele
// parcurgere in latime folosind o coada
{
    TCoada *c = InitQ();
    if (!c) {
        return 0;
    }

    // radacina nu o introduc in coada deoarece nu trebuie sa o afisez si pe ea
    // incep prin a adauga in coada nodurile de pe primul nivel
    int i;
    for (i = 0; i < 27; i++) {
        if (t && t->copii[i]) {
            int rez = IntrQ(c, t->copii[i]);
            if (!rez) {
                DistrugeQ(&c);
                return 0;
            }
        }
    }

    int rez = IntrQ(c, NULL);  // marchez finalul unui nivel ca sa stiu sa afisez newline
        if (!rez) {
            DistrugeQ(&c);
            return 0;
        }

    while (c->inc) {  // cat timp coada e nevida
        TArb n;
        ExtrQ(c, &n);  // extrag un nod din coada
        
        // afisez informatia corespunzatoare nodului extras:
        if (!n) {
            fprintf(fout, "\n");

            if (c->inc) {
                int rez = IntrQ(c, NULL);  // marchez finalul unui nivel
                if (!rez) {
                    DistrugeQ(&c);
                    return 0;
                }
            }
        } else {
            fprintf(fout, "%c ", n->eticheta);

            // adaug in coada copiii nodului extras:
            int i;
            for (i = 0; i < 27; i++) {
                if (n->copii[i]) {
                    int rez = IntrQ(c, n->copii[i]);
                    if (!rez) {
                        DistrugeQ(&c);
                        return 0;
                    }
                }
            }
        }
    }

    DistrugeQ(&c);
    return 1;
}

int NrFrunze(TArb t)
// numara frunzele din arbore
{
    if (!t) {
        return 0;
    }

    int nr = 0;

    // nodurile frunza sunt cele cu caracterul '$', retinute pe poz 0 in vectorul de copii
    if (t->copii[0]) {
        nr++;
    }

    int i;
    for (i = 1; i < 27; i++) {
        nr = nr + NrFrunze(t->copii[i]);
    }

    return nr;
}

int NrSufixe(TArb t, int K)
// numara sufixele de lungima K din arbore
{
    if (!t) {
        return 0;
    }

    // sufixele de K litere incep toate de pe nivelul 1 (pe nivelul 0 e radacina)
    // si au caracterul '$' pe nivelul K + 1

    //  verific daca ma aflu pe nivelul K si daca pe nivelul urmator am nodul '$':
    if (K == 0 && t->copii[0]) {
        return 1;
    }

    int nr = 0, i;
    for (i = 1; i < 27; i++) {
        nr = nr + NrSufixe(t->copii[i], K - 1);
    }

    return nr;
}

int MaxDescendentiDirecti(TArb t)
// determina numarul maxim de descendenti directi ai unui nod din arbore
{
    if (!t) {
        return 0;
    }

    // numar descendetii directi ai nodului curent:
    int nr = 0, i;
    for (i = 0; i < 27; i++) {
        if (t->copii[i]) {
            nr++;
        }
    }

    // calculez maximul:
    int max = nr;
    for (i = 0; i < 27; i++) {
        int max_copil = MaxDescendentiDirecti(t->copii[i]);
        if (max_copil > max) {
            max = max_copil;
        }
    }

    return max;
}

int ExistaSufix(TArb t, char *sufix)
// verifica daca un sufix dat apartine arborelui
{
    // verific litera cu litera daca sufixul se afla in arbore:
    int len_suf = strlen(sufix);
    int k;
    for (k = 0; k < len_suf; k++) {
        if (t && t->copii[sufix[k] - 'a' + 1]) {
            t = t->copii[sufix[k] - 'a' + 1];
        } else {
            break;  // litera curenta nu se afla in ordinea din sufix si in arbore
        }
    }

    if (k == len_suf && t->copii[0]) {
        // am parcurs tot sufixul litera cu litera si
        // nodul la care am ajuns are ca si copil caracterul de final de sufix
        return 1;
    }
    return 0;
}

// implementare coada pentru parcurgerea in latime:

TLista AlocCelula(TArb n)
// aloca o celula (pentru coada)
{
    TLista aux = (TLista)calloc(1, sizeof(TCelula));
    if (!aux) {
        return NULL;
    }

    aux->info = n;
    aux->urm = NULL;

    return aux;
}

TCoada* InitQ()
// creeaza coada vida
{
    TCoada *c = (TCoada *)calloc(1, sizeof(TCoada));
    if (!c) {
        return NULL;
    }

    c->inc = NULL;
    c->sf = NULL;

    return c;
}

int IntrQ(TCoada *c, TArb n)
// intoarce 1: introducerea in coada a reusit, 0: in caz contrar
{ 
    TLista aux = AlocCelula(n);
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

int ExtrQ(TCoada *c, TArb *n)
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
        TLista aux = c->inc;
        c->inc = c->inc->urm;
        free(aux);
    }

    return 1;
}

void ResetQ(TCoada *c)
// c devine coada vida
{
    if (!c) {
        return;
    }

    while (c->inc) {
        TLista aux = c->inc;
        c->inc = c->inc->urm;
        free(aux);
    }
    c->sf = NULL;
}

void DistrugeQ(TCoada **c)
// elibereaza memoria ocupata de coada
{
    ResetQ(*c);
    free(*c);
    *c = NULL;
}
