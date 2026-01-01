#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "proposition.h"
#include "regle.h"

/* Helper pour nettoyer les fins de ligne */
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

        if (chercherProposition(*base, buffer) == NULL) {
            Proposition *p = creerProposition(buffer);
            changerValeur(p, VALEUR_VRAIE);
            ajouterProposition(base, p);
            printf("  + Fait chargé : %s\n", buffer);
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