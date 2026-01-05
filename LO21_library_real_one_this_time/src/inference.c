#include "inference.h"
#include "proposition.h"
#include <stdio.h>
#include <string.h>


/* Vérifie si un nom est présent dans une liste de conditions */
int estDansConditions(Condition *liste, const char *nom) {
    Condition *c = liste;
    while (c != NULL) {
        if (strcmp(c->nom, nom) == 0) return 1;
        c = c->suiv;
    }
    return 0;
}

/* Vérifie si toutes les conditions d'une règle sont respectées */
int conditionsVraies(Condition *conditions, Proposition *listePropositions) {
    Condition *c = conditions;
    while (c != NULL) {
        Proposition *p = chercherProposition(listePropositions, c->nom);

        // Si le fait n'existe pas, il est INCONNU (-1)
        int valeurActuelle = (p != NULL) ? p->valeur : VALEUR_INCONNUE;

        // On compare avec ce que la règle attend (0 ou 1)
        if (valeurActuelle != c->valeurAttendue) {
            return 0;
        }
        c = c->suiv;
    }
    return 1;
}

/* --- FONCTIONS PRINCIPALES --- */

/* Invalide récursivement les conséquences d'un fait modifié */
void invaliderConsequences(Proposition *propositions, Regle *regles, const char *nomFaitModifie) {
    Regle *r = regles;
    while (r != NULL) {
        // Si le fait modifié est utilisé dans cette règle
        if (estDansConditions(r->premisses, nomFaitModifie)) {

            Proposition *concl = chercherProposition(propositions, r->conclusion);

            // Si la conclusion était VRAIE, on la réinitialise
            if (concl != NULL && concl->valeur == VALEUR_VRAIE) {
                concl->valeur = VALEUR_INCONNUE;

                printf("  [REVISION] '%s' est remis à INCONNU car '%s' a changé.\n",
                       concl->nom, nomFaitModifie);

                invaliderConsequences(propositions, regles, concl->nom);
            }
        }
        r = r->suiv;
    }
}

/* Moteur d'inférence */
void chainageAvant(Proposition **propositions, Regle *regles) {
    int changement = 1;
    int iteration = 0;

    while (changement == 1) {
        changement = 0;
        iteration++;
        Regle *r = regles;

        while (r != NULL) {
            if (conditionsVraies(r->premisses, *propositions)) {

                Proposition *conclusion = chercherProposition(*propositions, r->conclusion);

                // Création automatique si inexistant
                if (conclusion == NULL) {
                    conclusion = creerProposition(r->conclusion);
                    ajouterProposition(propositions, conclusion);
                    printf("  [INFO] Création automatique du fait : '%s'\n", r->conclusion);
                }

                if (conclusion->valeur != VALEUR_VRAIE) {
                    conclusion->valeur = VALEUR_VRAIE;
                    changement = 1;

                    printf("\n[SUCCES] Règle validée (Itération %d) :\n", iteration);
                    printf("  SI ");
                    Condition *c = r->premisses;
                    while (c != NULL) {
                        printf("%s (= %d)", c->nom, c->valeurAttendue);
                        if (c->suiv != NULL) printf(" ET ");
                        c = c->suiv;
                    }
                    printf("\n  ALORS '%s' devient VRAI.\n", conclusion->nom);
                }
            }
            r = r->suiv;
        }
    }
}