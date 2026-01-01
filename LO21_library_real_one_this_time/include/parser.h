#ifndef PARSER_H
#define PARSER_H

#include "proposition.h"
#include "regle.h"

/* ---------- PROPOSITIONS ---------- */

/* Charge les propositions depuis un fichier .txt */
void chargerPropositions(const char *nomFichier, Proposition **liste);

/* Sauvegarde les propositions dans un fichier .txt */
void sauvegarderPropositions(const char *nomFichier, Proposition *liste);


/* ---------- REGLES ---------- */

/* Charge les règles depuis un fichier .txt */
void chargerRegles(const char *nomFichier, Regle **liste);

/* Sauvegarde les règles dans un fichier .txt */
void sauvegarderRegles(const char *nomFichier, Regle *liste);

#endif
