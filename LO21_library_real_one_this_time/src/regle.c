#include "regle.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* 1. Créer une règle vide */
Regle* creerRegleVide() {
    Regle *r = malloc(sizeof(Regle));
    if (r == NULL) return NULL;

    r->premisses = NULL;
    r->conclusion = NULL;
    r->suiv = NULL;
    return r;
}

/* 2. Définir la conclusion */
void definirConclusion(Regle *r, const char *conclusion) {
    if (r == NULL) return;
    if (r->conclusion != NULL) free(r->conclusion); // Nettoyage si existant

    int len = strlen(conclusion);
    r->conclusion = malloc(len + 1);
    strcpy(r->conclusion, conclusion);
}

/* Utlitaire interne pour créer une condition */
Condition* creerConditionNode(const char *nom) {
    Condition *c = malloc(sizeof(Condition));
    c->nom = malloc(strlen(nom) + 1);
    strcpy(c->nom, nom);
    c->suiv = NULL;
    return c;
}

/* 3. Ajouter en queue de prémisse */
void ajouterPropositionPremisse(Regle *r, const char *nomProposition, int valeurAttendue) {
    if (r == NULL) return;

    Condition *nouv = malloc(sizeof(Condition));
    nouv->nom = malloc(strlen(nomProposition) + 1);
    strcpy(nouv->nom, nomProposition);
    nouv->valeurAttendue = valeurAttendue; // <--- NOUVEAU
    nouv->suiv = NULL;

    if (r->premisses == NULL) {
        r->premisses = nouv;
    } else {
        Condition *courant = r->premisses;
        while (courant->suiv != NULL) courant = courant->suiv;
        courant->suiv = nouv;
    }
}

/* 4. Test d'appartenance RECURSIF */
int appartientPremisse(Condition *c, const char *nomProposition) {
    if (c == NULL) return 0; // Cas de base : liste vide ou fin atteinte

    if (strcmp(c->nom, nomProposition) == 0) return 1; // Trouvé !

    return appartientPremisse(c->suiv, nomProposition); // Appel récursif
}

/* 5. Supprimer une proposition de la prémisse */
void supprimerPropositionPremisse(Regle *r, const char *nomProposition) {
    if (r == NULL || r->premisses == NULL) return;

    Condition *courant = r->premisses;
    Condition *prec = NULL;

    while (courant != NULL) {
        if (strcmp(courant->nom, nomProposition) == 0) {
            // On a trouvé, on supprime
            if (prec == NULL) {
                // C'était la tête
                r->premisses = courant->suiv;
            } else {
                prec->suiv = courant->suiv;
            }
            free(courant->nom);
            free(courant);
            return; // On supprime la première occurrence et on sort
        }
        prec = courant;
        courant = courant->suiv;
    }
}

/* 6. Tester si prémisse vide */
int estPremisseVide(Regle *r) {
    return (r == NULL || r->premisses == NULL);
}

/* 7. Accéder à la tête */
char* tetePremisse(Regle *r) {
    if (estPremisseVide(r)) return NULL;
    return r->premisses->nom;
}

/* 8. Accéder à la conclusion */
char* accederConclusion(Regle *r) {
    if (r == NULL) return NULL;
    return r->conclusion;
}

/* --- FONCTIONS LISTE REGLE (Inchangées ou adaptées) --- */

void ajouterRegle(Regle **liste, Regle *nouvelle) {
    if (*liste == NULL) {
        *liste = nouvelle;
    } else {
        Regle *courant = *liste;
        while (courant->suiv != NULL) courant = courant->suiv;
        courant->suiv = nouvelle;
    }
}

void libererRegles(Regle *liste) {
    Regle *courant = liste;
    while (courant != NULL) {
        Regle *suiv = courant->suiv;

        // Libération des prémisses
        Condition *c = courant->premisses;
        while (c != NULL) {
            Condition *cSuiv = c->suiv;
            free(c->nom);
            free(c);
            c = cSuiv;
        }

        if (courant->conclusion) free(courant->conclusion);
        free(courant);
        courant = suiv;
    }
}