#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---------- PROPOSITIONS ---------- */

void chargerPropositions(const char *nomFichier, Proposition **liste) {
    FILE *f = fopen(nomFichier, "r");
    if (f == NULL) {
        printf("Erreur : impossible d'ouvrir %s\n", nomFichier);
        return;
    }

    char ligne[128];

    while (fgets(ligne, sizeof(ligne), f) != NULL) {
        // enlever le '\n'
        int len = strlen(ligne);
        if (len > 0 && ligne[len-1] == '\n') {
            ligne[len-1] = '\0';
        }

        if (ligne[0] != '\0') {
            Proposition *p = malloc(sizeof(Proposition));
            p->nom = malloc(strlen(ligne) + 1);
            strcpy(p->nom, ligne);
            p->valeur = VALEUR_INCONNUE;
            p->suiv = NULL;

            // ajout à la liste
            if (*liste == NULL) {
                *liste = p;
            } else {
                Proposition *courant = *liste;
                while (courant->suiv != NULL) {
                    courant = courant->suiv;
                }
                courant->suiv = p;
            }
        }
    }

    fclose(f);
}

void sauvegarderPropositions(const char *nomFichier, Proposition *liste) {
    FILE *f = fopen(nomFichier, "w");
    if (f == NULL) {
        printf("Erreur : impossible d'ouvrir %s\n", nomFichier);
        return;
    }

    Proposition *courant = liste;
    while (courant != NULL) {
        fprintf(f, "%s\n", courant->nom);
        courant = courant->suiv;
    }

    fclose(f);
}


/* ---------- REGLES ---------- */

void chargerRegles(const char *nomFichier, Regle **liste) {
    FILE *f = fopen(nomFichier, "r");
    if (f == NULL) {
        printf("Erreur : impossible d'ouvrir %s\n", nomFichier);
        return;
    }

    char ligne[128];

    while (fgets(ligne, sizeof(ligne), f) != NULL) {
        int len = strlen(ligne);
        if (len > 0 && ligne[len-1] == '\n') {
            ligne[len-1] = '\0';
        }

        if (ligne[0] != '\0') {
            // On cherche "->"
            char *fleche = strstr(ligne, "->");
            if (fleche != NULL) {
                // conclusion = ce qui est après "->"
                char *conclusionStr = fleche + 2;
                while (*conclusionStr == ' ') conclusionStr++; // trim début
                char conclusion[32];
                sscanf(conclusionStr, "%31s", conclusion);

                // prémisses = ce qui est avant "->"
                int taillePremisses = fleche - ligne;
                char premissesStr[128];
                strncpy(premissesStr, ligne, taillePremisses);
                premissesStr[taillePremisses] = '\0';

                // Création des conditions
                Condition *premisses = NULL;
                char *token = strtok(premissesStr, "&");
                while (token != NULL) {
                    while (*token == ' ') token++; // trim début
                    Condition *c = malloc(sizeof(Condition));
                    c->nom = malloc(strlen(token)+1);
                    strcpy(c->nom, token);
                    c->suiv = NULL;

                    // ajout à la liste des prémisses
                    if (premisses == NULL) {
                        premisses = c;
                    } else {
                        Condition *cur = premisses;
                        while (cur->suiv != NULL) cur = cur->suiv;
                        cur->suiv = c;
                    }

                    token = strtok(NULL, "&");
                }

                // Création de la règle
                Regle *r = creerRegleVide();
                definirConclusion(r, conclusion);
                r->premisses = premisses;
                r->conclusion = malloc(strlen(conclusion)+1);
                strcpy(r->conclusion, conclusion);
                r->suiv = NULL;

                // ajout à la liste de règles
                if (*liste == NULL) {
                    *liste = r;
                } else {
                    Regle *cur = *liste;
                    while (cur->suiv != NULL) cur = cur->suiv;
                    cur->suiv = r;
                }
            }
        }
    }

    fclose(f);
}

void sauvegarderRegles(const char *nomFichier, Regle *liste) {
    FILE *f = fopen(nomFichier, "w");
    if (f == NULL) {
        printf("Erreur : impossible d'ouvrir %s\n", nomFichier);
        return;
    }

    Regle *r = liste;
    while (r != NULL) {
        // afficher prémisses
        Condition *c = r->premisses;
        while (c != NULL) {
            fprintf(f, "%s", c->nom);
            if (c->suiv != NULL) fprintf(f, " & ");
            c = c->suiv;
        }

        fprintf(f, " -> %s\n", r->conclusion);
        r = r->suiv;
    }

    fclose(f);
}
