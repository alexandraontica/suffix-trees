/* ONȚICĂ Alexandra-Elena - 311CB */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct nod {
    char eticheta;
    struct nod *copii[27];
} TNod, *TArb;

typedef struct celula {
    TArb info;  // pointer catre un nod din arbore
    struct celula *urm;
} TCelula, *TLista;

typedef struct coada {
    TLista inc, sf;
} TCoada;

void CloseFiles(FILE *fin, FILE *fout);

// functii cerinte 1, 2, 3:
TArb AlocNod(char eticheta_nod);
TArb ConstrArb(FILE *fin, int N);
void DistrugeArb(TArb *t);
int AfisareArbore(FILE *fout, TArb t);
int NrFrunze(TArb t);
int NrSufixe(TArb t, int K);
int MaxDescendentiDirecti(TArb t);
int ExistaSufix(TArb t, char *sufix);

// functii coada:
TLista AlocCelula(TArb n);
TCoada* InitQ();
int IntrQ(TCoada *c, TArb n);
int ExtrQ(TCoada *c, TArb *n);
void ResetQ(TCoada *c);
void DistrugeQ(TCoada **c);
