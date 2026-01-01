#ifndef INFERENCE_H
#define INFERENCE_H

#include "proposition.h"
#include "regle.h"

/* Note le double étoile **propositions pour permettre l'ajout dynamique */
void chainageAvant(Proposition **propositions, Regle *regles);
void invaliderConsequences(Proposition *propositions, Regle *regles, const char *nomFaitModifie);

#endif