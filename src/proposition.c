#include "proposition.h"
#include <stdlib.h>
#include <string.h>

Proposition* creerProposition(const char *nom) {
    Proposition *p = malloc(sizeof(Proposition));
    if (p == NULL) {
        return NULL;
    }

    int longueur = strlen(nom);
    p->nom = malloc(longueur + 1);
    if (p->nom == NULL) {
        free(p);
        return NULL;
    }

    strcpy(p->nom, nom);
    p->valeur = VALEUR_INCONNUE;
    p->suiv = NULL;

    return p;
}

void ajouterProposition(Proposition **liste, Proposition *nouvelle) {
    if (*liste == NULL) {
        *liste = nouvelle;
        return;
    }

    Proposition *courant = *liste;
    while (courant->suiv != NULL) {
        courant = courant->suiv;
    }

    courant->suiv = nouvelle;
}

Proposition* chercherProposition(Proposition *liste, const char *nom) {
    Proposition *courant = liste;

    while (courant != NULL) {
        if (strcmp(courant->nom, nom) == 0) {
            return courant;
        }
        courant = courant->suiv;
    }

    return NULL;
}

void changerValeur(Proposition *p, ValeurProposition nouvelleValeur) {
    if (p != NULL) {
        p->valeur = nouvelleValeur;
    }
}

void libererPropositions(Proposition *liste) {
    Proposition *courant = liste;

    while (courant != NULL) {
        Proposition *suiv = courant->suiv;

        free(courant->nom);
        free(courant);

        courant = suiv;
    }
}
