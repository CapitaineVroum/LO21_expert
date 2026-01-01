#include "liste.h"
#include <stdio.h>

void afficherPropositions(Proposition *liste) {
    Proposition *courant = liste;

    while (courant != NULL) {
        printf("Proposition : %s | Valeur : ", courant->nom);

        if (courant->valeur == VALEUR_VRAIE) {
            printf("VRAI\n");
        } else if (courant->valeur == VALEUR_FAUSSE) {
            printf("FAUX\n");
        } else {
            printf("INCONNUE\n");
        }

        courant = courant->suiv;
    }
}

void afficherRegles(Regle *liste) {
    Regle *courantRegle = liste;

    while (courantRegle != NULL) {
        printf("Règle : ");

        Condition *c = courantRegle->premisses;

        /* Afficher A & B & C */
        while (c != NULL) {
            printf("%s", c->nom);
            if (c->suiv != NULL) {
                printf(" & ");
            }
            c = c->suiv;
        }

        printf(" -> %s\n", courantRegle->conclusion);

        courantRegle = courantRegle->suiv;
    }
}

int compterPropositions(Proposition *liste) {
    int count = 0;
    Proposition *courant = liste;

    while (courant != NULL) {
        count++;
        courant = courant->suiv;
    }

    return count;
}

int compterRegles(Regle *liste) {
    int count = 0;
    Regle *courant = liste;

    while (courant != NULL) {
        count++;
        courant = courant->suiv;
    }

    return count;
}
