// /* ONȚICĂ Alexandra-Elena - 311CB */
// #include "arb_suf_comp.h"
// #include "arb_suf.h"

// TArbComp AlocNodComp(char *eticheta_nod)
// {
//     TArbComp aux = (TArbComp)calloc(1, sizeof(TNodComp));
//     if (aux) {
//         aux->eticheta = strdup(eticheta_nod);
//         if (!aux->eticheta) {
//             free(aux);
//             aux = NULL;
//         } else {
//             aux->copii = NULL;
//             aux->nr_copii = 0;
//         }
//     }
//     return aux;
// }

// TArbComp ConstrArbComp(FILE *fin, int N) {
//     TArb t = ConstrArb(fin, N);
//     if (!t) {
//         return NULL;
//     }

//     TArbComp t_comp = AlocNodComp("");

//     t_comp->copii = (TArbComp *)calloc(1, sizeof(TArbComp));
//     if (!t_comp->copii) {
//         DistrugeArbComp(&t_comp);
//         return NULL;
//     }
//     t_comp->nr_copii = 1;

//     t_comp->copii[0] = AlocNodComp("$");
//     if (!t_comp->copii[0]) {
//         DistrugeArbComp(&t_comp);
//         return NULL;
//     }

//     // parcurg t in adancime:
    

//     DistrugeArb(&t);
//     return t_comp;
// }

// // TArbComp ConstrArbComp(FILE *fin, int N)
// // {
// //     TArbComp t = AlocNodComp("");
// //     if (!t) {
// //         return NULL;
// //     }

// //     t->copii = (TArbComp *)calloc(27, sizeof(TArbComp));
// //     if (!t->copii) {
// //         DistrugeArbComp(&t);
// //         return NULL;
// //     }
// //     t->nr_copii = 1;

// //     t->copii[0] = AlocNodComp("$");
// //     if (!t->copii[0]) {
// //         DistrugeArbComp(&t);
// //         return NULL;
// //     }

// //     int i;
// //     for (i = 0; i < N; i++) {
// //         char cuvant[21];
// //         fscanf(fin, "%s", cuvant);

// //         int len_cuv = strlen(cuvant);
// //         int j;
// //         for (j = len_cuv - 1; j >= 0; j--) {
// //             char *sufix = cuvant + j;
// //             int len_suf = strlen(sufix);

// //             // verific daca sufixul sau parti din acesta exista deja in arbore;
// //             // ma opresc la prima litera din sufix care nu mai apare in ordinea
// //             // din cuvant in arbore:
// //             TArbComp aux = t;
// //             int k;
// //             for (k = 1; k < len_suf; k++) {
// //                 if (aux->copii[sufix[k] - 'a' + 1]) {
// //                     aux = aux->copii[sufix[k] - 'a' + 1];
// //                 } else {
// //                     break;
// //                 }
// //             }

// //             // adaug portiunea din sufix care nu exista deja in arbore:
// //             while (k < len_suf) {
// //                 aux->copii[sufix[k] - 'a' + 1] = AlocNodComp(sufix[k]);
// //                 if (!aux->copii[sufix[k] - 'a' + 1]) {
// //                     DistrugeArbComp(&t);
// //                     return NULL;
// //                 }

// //                 aux = aux->copii[sufix[k] - 'a' + 1];
// //                 k++;
// //             }

// //             if (!aux->copii[0]) {
// //                 aux->copii[0] = AlocNodComp('$');  // am ajuns la finalul sufixului
// //                 if (!aux->copii[0]) {
// //                     DistrugeArbComp(&t);
// //                     return NULL;
// //                 }
// //             }
// //         }
// //     }

// //     return t;
// // }

// int AfisareArboreComp(FILE *fout, TArbComp t)
// {
//     TCoadaComp *c = InitQComp();
//     if (!c) {
//         return 0;
//     }

//     int i;
//     for (i = 0; i < t->nr_copii; i++) {
//         if (t && t->copii[i]) {
//             int rez = IntrQComp(c, t->copii[i]);
//             if (!rez) {
//                 DistrugeQComp(&c);
//                 return 0;
//             }
//         }
//     }

//     int rez = IntrQComp(c, NULL);  // marchez finalul unui nivel
//         if (!rez) {
//             DistrugeQComp(&c);
//             return 0;
//         }

//     while (c->inc) {  // coada e nevida
//         TArbComp n;
//         ExtrQComp(c, &n);  // nu este nevoie sa verific ce returneaza functia, stiu ca c e coada nevida
        
//         if (!n) {
//             fprintf(fout, "\n");

//             if (c->inc) {
//                 int rez = IntrQComp(c, NULL);  // marchez finalul unui nivel
//                 if (!rez) {
//                     DistrugeQComp(&c);
//                     return 0;
//                 }
//             }
//         } else {
//             fprintf(fout, "%s ", n->eticheta);

//             int i;
//             for (i = 0; i < n->nr_copii; i++) {
//                 if (n && n->copii[i]) {
//                     int rez = IntrQComp(c, n->copii[i]);
//                     if (!rez) {
//                         DistrugeQComp(&c);
//                         return 0;
//                     }
//                 }
//             }
//         }
//     }

//     DistrugeQComp(&c);
//     return 1;
// }

// void DistrugeArbComp(TArbComp *t)
// {
//     if (!(*t)) {
//         return;
//     }

//     for (int i = 0; i < (*t)->nr_copii; i++) {
//         DistrugeArbComp(&((*t)->copii[i]));
//     }

//     free((*t)->copii);
//     free((*t)->eticheta);
//     free(*t);
//     *t = NULL;
// }

// // implementare coada pentru parcurgerea in latime:

// TListaComp AlocCelulaComp(TArbComp n)
// {
//     TListaComp aux = (TListaComp)calloc(1, sizeof(TCelulaComp));
//     if (!aux) {
//         return NULL;
//     }

//     aux->info = n;
//     aux->urm = NULL;

//     return aux;
// }

// TCoadaComp* InitQComp()
// // creare coada vida
// {
//     TCoadaComp *c = (TCoadaComp *)calloc(1, sizeof(TCoadaComp));
//     if (!c) {
//         return NULL;
//     }

//     c->inc = NULL;
//     c->sf = NULL;

//     return c;
// }

// int IntrQComp(TCoadaComp *c, TArbComp n)
// // intoarce 1: introducerea in coada a reusit, 0: in caz contrar
// { 
//     TListaComp aux = AlocCelulaComp(n);
//     if (!aux) {
//         return 0;
//     }

//     if (!c->inc) {  // e coada vida
//         c->inc = aux;
//         c->sf = aux;
//     } else {
//         c->sf->urm = aux;
//         c->sf = aux;
//     }
  
//     return 1;             
// }

// int ExtrQComp(TCoadaComp *c, TArbComp *n)
// // intoarce 0: coada e vida, 1: s-a extras un nod din coada
// {
//     if (!c->inc) {  // e coada vida
//         return 0;
//     }

//     *n = c->inc->info;  // pastrez informatia elementului extras
//     if (c->inc == c->sf) {  // coada are un element
//         free(c->inc);
//         c->inc = NULL;  // devine coada vida
//         c->sf = NULL;
//     } else {
//         TListaComp aux = c->inc;
//         c->inc = c->inc->urm;
//         free(aux);
//     }

//     return 1;
// }

// void ResetQComp(TCoadaComp *c)
// // c devine coada vida
// {
//     if (!c) {
//         return;
//     }

//     while (c->inc) {
//         TListaComp aux = c->inc;
//         c->inc = c->inc->urm;
//         free(aux);
//     }
//     c->sf = NULL;
// }

// void DistrugeQComp(TCoadaComp **c)
// {
//     ResetQComp(*c);
//     free(*c);
//     *c = NULL;
// }
