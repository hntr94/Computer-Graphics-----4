Vinatoru Andrei-Ioan
335CB

======================================================================

pentru utilizarea aplicatiei se vor folosi urmatoarele taste:
A, D - deplasarea lui Goku in stanga si dreapta
ARROW_LEFT si ARROW_RIGHT - deplasarea lui Frieza stanga si dreapta
E - block goku
RSHIFT - block frieza
SPACE - loveste goku
RCTRL - loveste frieza

Starile in care se gasesc playerii la un moment dat sunt:
- stand(animatie simpla de stand),
- run (goku alearga, frieza pluteste(in modul de player alearga si el))
- drop (cca 7-9 frame-uri, goku cade jos si se ridica, frieza cade si se ridica cu ajutorul cozii)
- block(la goku este un block simplu, iar la frieza se incolaceste si se roteste in jurul lui)
- kick (2-3 frame-uri; ambii lovesc cu pumnii)

Pentru implementare am creat clasa Player care contine un map pentru toate meshele citite, fiecare intr-un
vector intr-o animatie specifica. In aceasta clasa am si o metoda care creeaza mesha in functie de datele
citite din fisier.

Pentru structura fisireleor auxiliare am ales un format simplu, gandit de mine, pentru a fi mai usor de citit.

Dificultatea principala a temei consta in parametrizarea sprite-urilor dintr-un spritesheet pentru determinarea
coordonatelor de texturare.

Arhiva contine: frameworkul si in plus fisierul cu texturi si fisierele auxiliare.

Functionalitati:
- toate cele prezente in baseline
- bonus, introducerea un NPC simplu care se deplaseaza spre player si il loveste