#ifndef INFERENCE_H
#define INFERENCE_H

#include "proposition.h"
#include "regle.h"

void chainageAvant(Proposition **propositions, Regle *regles);
void invaliderConsequences(Proposition *propositions, Regle *regles, const char *nomFaitModifie);

#endif