# Arbori de sufixe

Pentru a deschide fisierele de input si output, am calculat mai întâi poziția numelor acestora in vectorul parametrilor în linia de comandă deoarece în cadrul cerinței 2 există un parametru în plus și, deci, poziția (indicii) se modifică.
Am deschis fișierele, verificând dacă operațiile au reușit, și am citit numărul de cuvinte care vor trebui adăugate în arborele de sufixe în cadrul cerințelor.

## Cerința 1
Arborele de sufixe îl voi reprezenta cu ajutorul unei structuri `nod` (tipul `TArb` va fi pointer către o astfel structură) cu câmpurile `eticheta` (char) în care salvez informația nodului și un vector `copii` de tipul `struct nod *` ce conține pointeri către nodurile ce vor reprezenta copiii nodului curent. Copilul de pe poziția 0, dacă există, va avea mereu eticheta *$*. De la poziția 1 la poziția 26, copilul de pe poziția *i* va avea ca etichetă a *i*-a literă din alfabet.

### Construcție arbore de sufixe
Am alocat rădăcina arborelui și primul nod cu eticheta *$*. Am citit pe rând fiecare cuvânt din fișierul de intrare. Pentru fiecare cuvânt, am parcurs toate sufixele posibile și am verificat dacă acestea există (total sau parțial) în arbore. Am verificat pentru fiecare literă din sufix dacă nodul curent are copilul corespunzător următoarei litere și m-am oprit la prima literă care nu există în arbore. Literele care nu existau deja le-am adaugat ulterior, alocând nodurile corespunzătoare acestora.

### Afișare arbore - parcurgere în lățime
Pentru a putea parcurge arborele în lățime m-am folosit de o coada. Am început prin a adăuga în coadă nodurile de pe nivelul 1 (rădăcina, aflată pe nivelul 0, nu trebuie afișată). La finalul parcurgerii unui nivel am adăugat în coadă un nod *NULL* ca să știu când trebuie să afișez *newline*. Am extras pe rând câte un nod din coada, am afișat informația corespunzătoare nodului, iar dacă acesta era diferit de *NULL* i-am adăugat copiii în coadă.

## Cerința 2
Am construit arborele de sufixe ca la cerința 1.

### Numărul de noduri frunză
Pentru a calcula numărul de frunze am folosit o funcție recursivă. 
Nodurile frunză sunt cele cu eticheta *$*, așa că de fiecare dată când se întâlnește un nod care are acest copil, numărul de frunze crește cu 1. După prelucrarea unui nod apelez funcția pentru copiii acestuia (parcurgere în adâncime).

### Numărul de sufixe de dimensiune K
Pentru a calcula numărul de sufixe de dimensiune K am folosit o funcție recursivă.
Sufixele de K litere din arbore încep toate de pe nivelul 1 (pe nivelul 0 e radăcina) și au caracterul *$* pe nivelul K + 1, deci numărul de sufixe de dimensiune K este egal cu numărul de noduri ce marchează finalul unui sufix de pe nivelul K + 1.

### Numărul maxim de descendenți direcți pe care îi are un nod
Pentru a calcula numărul maxim de descendenți direcți pe care îi are un nod am folosit o funcție recursivă.
Pentru fiecare nod am calculat numărul de descendenți, pe care l-am comparat cu numărul maxim de descenți din subarborii cu rădăcina în copiii săi.

## Cerința 3
Am construit arborele de sufixe ca la cerința 1 și am citit cele M sufixe. Pentru fiecare sufix, verific literă cu literă dacă se află în arbore, similar ca la cerința 1 (înainte să adaug un sufix în arbore verific dacă există deja). Mă opresc pe prima literă care nu se află în arbore pe poziția potrivită. Dacă ajung la finalul sufixului și pe nivelul următor am nodul cu eticheta *$*, atunci sufixul există în arbore.

## Cerința 4
Pentru arborele de sufixe compact am definit un nou tip, `TArbComp`. Spre deosebire de `TArb`, copiii nodurilor de acest tip vor fi alocați dinamic, deci îmi trebuie să păstrez pentru fiecare nod și numărul de copii. Eticheta neavând o singură literă, nu mai pot face asocierea în arborele de copii poziție-literă, așa că am preferat să păstrez doar copiii diferiți de *NULL*.

### Construcție arbore de sufixe compact
Am început prin a construi un arbore de sufixe ca la cerința 1, de tipul `TArb`, pe care l-am transformat apoi în tipul `TArbComp` (am făcut acest lucru și nu am contruit direct arborele atomic de tipul `TArbComp` ca să mă asigur că sufixele sunt ordonate alfabetic).
Am transformat arborele atomic de tipul `TArbComp` într-unul compact „comprimând” sufixele astfel: dacă nodul curent are un singur fiu și acesta este diferit de *$*, eticheta fiului poate fi concatenată la cea a nodului curent; elimin apoi fiul și copiii fiului devin copiii nodului curent. Apelez din nou funcția de comprimare asupra nodului curent deoarece copiii fiului pe care l-am eliminat nu au fost încă verificați (se poate ca fiul șters să fi avut un singur copil, deci din nou nodul curent are un singur copil căruia trebuie să i se aplice operațiile descrise mai sus). Apelez, apoi, funcția pentru fiecare copil al nodului curent.

Afișarea am realizat-o la fel ca la cerința 1, folosindu-mă de o coadă.
