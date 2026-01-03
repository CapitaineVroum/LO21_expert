#ifndef PARSER_H
#define PARSER_H

#include "proposition.h"
#include "regle.h"

/* Charge depuis fichier */
void chargerPropositions(const char *nomFichier, Proposition **base);
void chargerRegles(const char *nomFichier, Regle **base);

/* Sauvegarde vers fichier (NOUVEAU) */
void sauvegarderPropositions(const char *nomFichier, Proposition *base);
void sauvegarderRegles(const char *nomFichier, Regle *base);

#endif