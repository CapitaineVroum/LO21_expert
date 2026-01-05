#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "proposition.h"
#include "regle.h"

/* nettoyer les fins de ligne */
void nettoyerChaine(char *str) {
    char *p = strchr(str, '\n');
    if (p) *p = '\0';
    p = strchr(str, '\r');
    if (p) *p = '\0';
}

void chargerPropositions(const char *nomFichier, Proposition **base) {
    FILE *f = fopen(nomFichier, "r");
    char buffer[256];

    if (f == NULL) {
        printf("  [ERREUR] Impossible d'ouvrir le fichier faits : %s\n", nomFichier);
        return;
    }

    while (fgets(buffer, 256, f)) {
        nettoyerChaine(buffer);
        if (strlen(buffer) == 0) continue;

        char nom[50];
        int val = 1; // Par défaut VRAI si pas de =

        // On cherche le signe '='
        char *egal = strchr(buffer, '=');

        if (egal != NULL) {
            *egal = '\0'; // Coupe la chaîne ici
            strcpy(nom, buffer); // Copie la partie avant le =
            val = atoi(egal + 1); // Convertit la partie après le = en int
        } else {
            strcpy(nom, buffer);
        }

        if (chercherProposition(*base, nom) == NULL) {
            Proposition *p = creerProposition(nom);

            if (val == 1) changerValeur(p, VALEUR_VRAIE);
            else if (val == 0) changerValeur(p, VALEUR_FAUSSE);
            else changerValeur(p, VALEUR_INCONNUE);

            ajouterProposition(base, p);
            printf("  + Fait chargé : %s (Val=%d)\n", p->nom, p->valeur);
        }
    }
    fclose(f);
}

void chargerRegles(const char *nomFichier, Regle **base) {
    FILE *f = fopen(nomFichier, "r");
    char buffer[256];

    if (f == NULL) {
        printf("  [ERREUR] Impossible d'ouvrir le fichier regles : %s\n", nomFichier);
        return;
    }

    while (fgets(buffer, 256, f)) {
        nettoyerChaine(buffer);
        if (strlen(buffer) == 0) continue;

        /* Format : Conclusion Premisse1=1 Premisse2=0 ... */
        char *token = strtok(buffer, " ");
        if (token == NULL) continue;

        Regle *r = creerRegleVide();
        definirConclusion(r, token);

        while ((token = strtok(NULL, " ")) != NULL) {
            char *egal = strchr(token, '=');
            int valeurAttendue = 1;
            char nomPremisse[50];

            if (egal != NULL) {
                *egal = '\0';
                strcpy(nomPremisse, token);
                valeurAttendue = atoi(egal + 1);
            } else {
                strcpy(nomPremisse, token);
            }
            ajouterPropositionPremisse(r, nomPremisse, valeurAttendue);
        }

        ajouterRegle(base, r);
        printf("  + Regle chargée : %s\n", accederConclusion(r));
    }
    fclose(f);
}

void sauvegarderPropositions(const char *nomFichier, Proposition *base) {
    FILE *f = fopen(nomFichier, "w");
    if (f == NULL) {
        printf("  [ERREUR] Impossible de créer le fichier faits : %s\n", nomFichier);
        return;
    }

    Proposition *courant = base;
    while (courant != NULL) {
        fprintf(f, "%s=%d\n", courant->nom, courant->valeur);
        courant = courant->suiv;
    }
    fclose(f);
    printf("  + Faits sauvegardés dans %s\n", nomFichier);
}

void sauvegarderRegles(const char *nomFichier, Regle *base) {
    FILE *f = fopen(nomFichier, "w");
    if (f == NULL) {
        printf("  [ERREUR] Impossible de créer le fichier règles : %s\n", nomFichier);
        return;
    }

    Regle *r = base;
    while (r != NULL) {
        // 1. Ecrire la conclusion
        if (r->conclusion != NULL) {
            fprintf(f, "%s", r->conclusion);
        }

        // 2. Ecrire les prémisses (Format: Nom=Valeur)
        Condition *c = r->premisses;
        while (c != NULL) {
            fprintf(f, " %s=%d", c->nom, c->valeurAttendue);
            c = c->suiv;
        }

        fprintf(f, "\n");
        r = r->suiv;
    }

    fclose(f);
    printf("  + Règles sauvegardées dans %s\n", nomFichier);
}