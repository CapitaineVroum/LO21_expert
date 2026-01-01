#ifndef REGLE_H
#define REGLE_H

typedef struct Condition {
    char *nom;
    int valeurAttendue; // 1 pour "Doit être VRAI", 0 pour "Doit être FAUX"
    struct Condition *suiv;
} Condition;

typedef struct Regle {
    Condition *premisses;
    char *conclusion;
    struct Regle *suiv;
} Regle;

/* --- FONCTIONS DU SUJET (TAD REGLE) --- */

/* 1. Créer une règle vide (sans conclusion ni prémisse) */
Regle* creerRegleVide();

/* 2. Créer (définir) la conclusion d'une règle */
void definirConclusion(Regle *r, const char *conclusion);

/* 3. Ajouter une proposition à la prémisse (en queue) */
void ajouterPropositionPremisse(Regle *r, const char *nomProposition, int valeurAttendue);

/* 4. Tester si une proposition appartient à la prémisse (RECURSIF) */
int appartientPremisse(Condition *c, const char *nomProposition);

/* 5. Supprimer une proposition donnée de la prémisse */
void supprimerPropositionPremisse(Regle *r, const char *nomProposition);

/* 6. Tester si la prémisse est vide */
int estPremisseVide(Regle *r);

/* 7. Accéder à la proposition en tête de prémisse */
char* tetePremisse(Regle *r);

/* 8. Accéder à la conclusion */
char* accederConclusion(Regle *r);


/* --- FONCTIONS UTILITAIRES EXISTANTES (pour les listes de règles) --- */
void ajouterRegle(Regle **liste, Regle *nouvelle);
void libererRegles(Regle *liste);
/* (Garde tes anciennes fonctions comme creerCondition si besoin en interne) */

#endif