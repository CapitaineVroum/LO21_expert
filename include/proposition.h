//
// Created by Donat on 09/12/2025.
//

#ifndef PROPOSITION_H
#define PROPOSITION_H

typedef enum {
    VALEUR_INCONNUE = -1,
    VALEUR_FAUSSE = 0,
    VALEUR_VRAIE = 1
} ValeurProposition;

typedef struct Proposition {
    char *nom;
    ValeurProposition valeur;
    struct Proposition *suiv;
} Proposition;

/* Crée une nouvelle proposition avec valeur inconnue */
Proposition* creerProposition(const char *nom);

/* Ajoute une proposition à la fin de la liste */
void ajouterProposition(Proposition **liste, Proposition *nouvelle);

/* Cherche une proposition par son nom */
Proposition* chercherProposition(Proposition *liste, const char *nom);

/* Change la valeur d'une proposition */
void changerValeur(Proposition *p, ValeurProposition nouvelleValeur);

/* Libère toute la liste des propositions */
void libererPropositions(Proposition *liste);

#endif
