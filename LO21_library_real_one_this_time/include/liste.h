#ifndef LISTE_H
#define LISTE_H

#include "proposition.h"
#include "regle.h"

/* Affiche toutes les propositions */
void afficherPropositions(Proposition *liste);

/* Affiche toutes les règles */
void afficherRegles(Regle *liste);

/* Compte le nombre de propositions */
int compterPropositions(Proposition *liste);

/* Compte le nombre de règles */
int compterRegles(Regle *liste);

/* Supprime la règle numéro N (commence à 1) */
void supprimerRegleParIndex(Regle **liste, int index);

#endif
