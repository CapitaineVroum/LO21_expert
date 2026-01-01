#ifndef IO_H
#define IO_H

#include "proposition.h"
#include "regle.h"

/* Charge toute la base de connaissances */
void chargerBase(const char *fichierProps,
                 const char *fichierRegles,
                 Proposition **propositions,
                 Regle **regles);

/* Sauvegarde toute la base de connaissances */
void sauvegarderBase(const char *fichierProps,
                     const char *fichierRegles,
                     Proposition *propositions,
                     Regle *regles);

#endif
