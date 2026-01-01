#include "io.h"
#include "parser.h"
#include <stdio.h>

void chargerBase(const char *fichierProps,
                 const char *fichierRegles,
                 Proposition **propositions,
                 Regle **regles)
{
    printf("Chargement des propositions...\n");
    chargerPropositions(fichierProps, propositions);

    printf("Chargement des règles...\n");
    chargerRegles(fichierRegles, regles);

    printf("Base de connaissances chargée.\n");
}

void sauvegarderBase(const char *fichierProps,
                     const char *fichierRegles,
                     Proposition *propositions,
                     Regle *regles)
{
    printf("Sauvegarde des propositions...\n");
    sauvegarderPropositions(fichierProps, propositions);

    printf("Sauvegarde des règles...\n");
    sauvegarderRegles(fichierRegles, regles);

    printf("Base de connaissances sauvegardée.\n");
}
