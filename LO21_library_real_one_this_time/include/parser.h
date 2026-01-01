#ifndef PARSER_H
#define PARSER_H

#include "proposition.h"
#include "regle.h"

/* Charge uniquement les propositions depuis un fichier */
void chargerPropositions(const char *nomFichier, Proposition **base);

/* Charge uniquement les règles depuis un fichier */
void chargerRegles(const char *nomFichier, Regle **base);

#endif