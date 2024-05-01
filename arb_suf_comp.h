/* ONȚICĂ Alexandra-Elena - 311CB */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arb_suf.h"

#ifndef ARBORE_COMP_H
#define ARBORE_COMP_H

typedef struct nod_comp {
    char *eticheta;
    int nr_copii;
    struct nod_comp **copii;
} TNodComp, *TArbComp;

typedef struct celula_comp {
    TArbComp info;  // pointer catre un nod din arbore
    struct celula_comp *urm;
} TCelulaComp, *TListaComp;

typedef struct coada_comp {
    TListaComp inc, sf;
} TCoadaComp;

// functii cerinta 4:
TArbComp AlocNodComp(char *eticheta_nod);
TArbComp TransfTArbInTArbComp(TArb t, char *eticheta);
void CompresareSufixe(TArbComp t);
TArbComp ConstrArbComp(FILE *fin, int N);
int AfisareArboreComp(FILE *fout, TArbComp t);
void DistrugeArbComp(TArbComp *t);

// functii coada:
TListaComp AlocCelulaComp(TArbComp n);
TCoadaComp* InitQComp();
int IntrQComp(TCoadaComp *c, TArbComp n);
int ExtrQComp(TCoadaComp *c, TArbComp *n);
void ResetQComp(TCoadaComp *c);
void DistrugeQComp(TCoadaComp **c);

#endif
