/* ONȚICĂ Alexandra-Elena - 311CB */
#include "arb_suf.h"
#include "arb_suf_comp.h"

int main(int argc, char *argv[])
{
    int idx_in = 2;
    int idx_out = 3;

    if (argc == 5) {
        // cerinta 2 are un parametru in plus (<K>) inainte de numele fisierelor
        idx_in++;
        idx_out++;   
    }

    // deschid fisierele de input/output:
    FILE *fin = fopen(argv[idx_in], "r");

    if (fin == NULL) {
        fprintf(stderr, "Fisierul de intrare nu a fost deschis\n");
        return 0;
    }

    FILE *fout = fopen(argv[idx_out], "w");

    if (fout == NULL) {
        fprintf(stderr, "Fisierul de iesire nu a fost deschis\n");
        fclose(fin);
        return 0;
    }
    
    int N;
    fscanf(fin, "%d", &N);  // numar cuvinte de adaugat in arbore

    // identific cerinta pe care trebuie sa o rezolv:
    if (!strcmp(argv[1], "-c1")) {
        // cerinta 1
        TArb arb_suf = ConstrArb(fin, N);
        if (!arb_suf) {
            fprintf(stderr, "Constructia arborelui nu a reusit\n");
            CloseFiles(fin, fout);
            return 0;
        }

        int rez = AfisareArbore(fout, arb_suf);
        if (!rez) {
            // pot aparea probleme la alocari (coada)
            fprintf(stderr, "Afisarea nu a reusit\n");
        }

        DistrugeArb(&arb_suf);
    } else if (!strcmp(argv[1], "-c2")) {
        // cerinta 2
        TArb arb_suf = ConstrArb(fin, N);
        if (!arb_suf) {
            fprintf(stderr, "Constructia arborelui nu a reusit\n");
            CloseFiles(fin, fout);
            return 0;
        }
        
        fprintf(fout, "%d\n", NrFrunze(arb_suf));

        // K poate avea maxim 2 cifre (nu exista cuvinte mai lungi)
        int K = argv[2][0] - '0';
        if (argv[2][1] != '\0') {
            K = K * 10 + argv[2][1] - '0';
        }
        fprintf(fout, "%d\n", NrSufixe(arb_suf, K));
        
        fprintf(fout, "%d\n", MaxDescendentiDirecti(arb_suf));

        DistrugeArb(&arb_suf);
    } else if (!strcmp(argv[1], "-c3")) {
        // cerinta 3
        int M;
        fscanf(fin, "%d", &M);

        TArb arb_suf = ConstrArb(fin, N);
        if (!arb_suf) {
            fprintf(stderr, "Constructia arborelui nu a reusit\n");
            CloseFiles(fin, fout);
            return 0;
        }

        int i;
        for (i = 0; i < M; i++) {
            char sufix[21];
            fscanf(fin, "%s", sufix);
            fprintf(fout, "%d\n", ExistaSufix(arb_suf, sufix));
        }

        DistrugeArb(&arb_suf);
    } else if (!strcmp(argv[1], "-c4")) {
        // cerinta 4
        TArbComp arb_suf_comp = ConstrArbComp(fin, N);
        if (!arb_suf_comp) {
            fprintf(stderr, "Constructia arborelui nu a reusit\n");
            CloseFiles(fin, fout);
            return 0;
        }

        int rez = AfisareArboreComp(fout, arb_suf_comp);
        if (!rez) {
            fprintf(stderr, "Afisarea nu a reusit\n");
        }        

        DistrugeArbComp(&arb_suf_comp);
    } else {
        fprintf(stderr, "Cerinta invalida\n");
    }

    CloseFiles(fin, fout);
    return 0;
}
