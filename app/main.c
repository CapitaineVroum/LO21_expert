#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "proposition.h"
#include "regle.h"
#include "inference.h"
#include "liste.h"
#include "io.h"

void viderBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void lireChaine(char *buffer, int taille) {
    fgets(buffer, taille, stdin);
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
}

void menuAjouterFait(Proposition **bf) {
    char nom[50];
    printf("Nom du fait à ajouter (ex: MoteurChaud) : ");
    lireChaine(nom, 50);

    Proposition *p = chercherProposition(*bf, nom);
    if (p == NULL) {
        p = creerProposition(nom);
        ajouterProposition(bf, p);
    }
    changerValeur(p, VALEUR_VRAIE);
    printf("Fait '%s' ajouté et marqué VRAI.\n", nom);
}

void menuModifierFait(Proposition *bf, Regle *bc) {
    char nom[50];
    int val;

    printf("\n--- LISTE DES FAITS ACTUELS ---\n");
    afficherPropositions(bf);
    printf("-------------------------------\n");

    printf("Nom du fait à modifier : ");
    lireChaine(nom, 50);

    Proposition *p = chercherProposition(bf, nom);
    if (p == NULL) {
        printf("Erreur : Fait inconnu.\n");
        return;
    }

    printf("Nouvelle valeur (1=VRAI, 0=FAUX, -1=INCONNU) : ");
    if (scanf("%d", &val) != 1) {
        while(getchar() != '\n');
        val = -2;
    }
    viderBuffer();

    if (val == 1) p->valeur = VALEUR_VRAIE;
    else if (val == 0) p->valeur = VALEUR_FAUSSE;
    else if (val == -1) p->valeur = VALEUR_INCONNUE;
    else {
        printf("Valeur invalide.\n");
        return;
    }

    printf("\n--- CYCLE DE MISE A JOUR ---\n");
    printf("1. Invalidation des conséquences...\n");
    invaliderConsequences(bf, bc, nom);
    printf("2. Relance du moteur...\n");
    chainageAvant(&bf, bc);
    printf("--- MISE A JOUR TERMINEE ---\n");
}

void menuAjouterRegle(Regle **bc, Proposition *bf) {
    char buffer[50];
    int typeCond;

    printf("\n--- CREATION D'UNE REGLE ---\n");
    printf("Propositions disponibles :\n");
    afficherPropositions(bf);
    printf("--------------------------\n");

    Regle *r = creerRegleVide();
    if (r == NULL) return;

    printf("Quelle est la CONCLUSION de la règle ? ");
    lireChaine(buffer, 50);
    definirConclusion(r, buffer);

    printf("Ajout des prémisses (tapez 'FIN' pour arrêter) :\n");
    while (1) {
        printf("  - Nom de la condition : ");
        lireChaine(buffer, 50);

        if (strcmp(buffer, "FIN") == 0 || strcmp(buffer, "fin") == 0) {
            break;
        }

        printf("    Doit-elle être VRAIE (1) ou FAUSSE (0) ? ");
        if (scanf("%d", &typeCond) != 1) typeCond = 1;
        viderBuffer();

        ajouterPropositionPremisse(r, buffer, typeCond);
    }

    if (estPremisseVide(r)) {
        printf("Règle vide ignorée.\n");
        libererRegles(r);
    } else {
        ajouterRegle(bc, r);
        printf("Règle ajoutée !\n");
    }
}

void menuModifierRegle(Regle *bc, Proposition *bf) {
    if (bc == NULL) {
        printf("Aucune règle à modifier.\n");
        return;
    }

    /* 1. Sélection de la règle */
    printf("\n--- MODIFIER UNE REGLE ---\n");
    Regle *courant = accesTeteBase(bc);
    int i = 1;
    while (courant != NULL) {
        printf("%d. SI %s ... ALORS %s\n", i, tetePremisse(courant), accederConclusion(courant));
        courant = courant->suiv;
        i++;
    }

    int choixRegle;
    printf("Numero de la règle : ");
    if (scanf("%d", &choixRegle) != 1) {
        viderBuffer();
        return;
    }

    viderBuffer();

    Regle *r = bc;
    for (int k = 1; k < choixRegle && r != NULL; k++) r = r->suiv;

    if (r == NULL) { printf("Règle introuvable.\n"); return; }

    /* 2. Sous-menu de modification */
    int sousChoix = 0;
    do {
        printf("\n--- REGLE ACTUELLE ---\n");
        printf("SI ");
        Condition *c = r->premisses;
        while (c != NULL) {
            printf("%s=%d ", c->nom, c->valeurAttendue);
            if (c->suiv) printf("ET ");
            c = c->suiv;
        }
        printf("ALORS %s\n", r->conclusion);
        printf("----------------------\n");
        printf("1. Ajouter une condition\n");
        printf("2. Supprimer une condition\n");
        printf("0. Retour\n");
        printf("Votre choix : ");
        scanf("%d", &sousChoix); viderBuffer();

        if (sousChoix == 1) {
            char buffer[50];
            int val;
            printf("Nom de la prémisse à ajouter : ");
            lireChaine(buffer, 50);

            if (appartientPremisse(r->premisses, buffer)) {
                printf("Erreur : Cette condition existe déjà !\n");
            } else {
                printf("Valeur (1=VRAI, 0=FAUX) : ");
                scanf("%d", &val); viderBuffer();
                ajouterPropositionPremisse(r, buffer, val);
                printf("Condition ajoutée.\n");
            }

        } else if (sousChoix == 2) {
            char buffer[50];
            printf("Nom de la prémisse à supprimer : ");
            lireChaine(buffer, 50);

            if (appartientPremisse(r->premisses, buffer)) {
                supprimerPropositionPremisse(r, buffer);
                printf("Condition supprimée.\n");
            } else {
                printf("Erreur : Cette condition n'est pas dans la règle.\n");
            }
        }

    } while (sousChoix != 0);
}

void menuSupprimerFait(Proposition **bf, Regle *bc) {
    char nom[50];
    printf("\n--- SUPPRESSION D'UN FAIT ---\n");
    afficherPropositions(*bf);
    printf("Nom du fait à supprimer : ");
    lireChaine(nom, 50);

    Proposition *p = chercherProposition(*bf, nom);
    if (p == NULL) {
        printf("Erreur : Ce fait n'existe pas.\n");
        return;
    }

    invaliderConsequences(*bf, bc, nom);
    supprimerProposition(bf, nom);
    printf("Fait '%s' supprimé de la base.\n", nom);
    chainageAvant(bf, bc);
}

void menuSupprimerRegle(Regle **bc) {
    int choix;
    printf("\n--- SUPPRESSION D'UNE REGLE ---\n");

    Regle *r = *bc;
    int i = 1;
    if (r == NULL) {
        printf("(Aucune règle)\n");
        return;
    }
    while (r != NULL) {
        printf("%d. SI %s ... ALORS %s\n", i, tetePremisse(r), accederConclusion(r));
        r = r->suiv;
        i++;
    }

    printf("Numéro de la règle à supprimer : ");
    if (scanf("%d", &choix) != 1) {
        while(getchar() != '\n');
        return;
    }
    viderBuffer();

    supprimerRegleParIndex(bc, choix);
    printf("Règle supprimée.\n");
}

void menuCharger(Proposition **bf, Regle **bc) {
    char fProps[50];
    char fRegles[50];

    printf("\n--- CHARGEMENT DE BASE (via IO) ---\n");
    printf("Nom du fichier de FAITS (ex: faits.txt) : ");
    lireChaine(fProps, 50);

    printf("Nom du fichier de REGLES (ex: regles.txt) : ");
    lireChaine(fRegles, 50);
    chargerBase(fProps, fRegles, bf, bc);
}

void menuSauvegarder(Proposition *bf, Regle *bc) {
    char fProps[50];
    char fRegles[50];

    printf("\n--- SAUVEGARDE DE LA BASE ---\n");
    printf("Nom du fichier pour les FAITS (ex: faits_save.txt) : ");
    lireChaine(fProps, 50);

    printf("Nom du fichier pour les REGLES (ex: regles_save.txt) : ");
    lireChaine(fRegles, 50);
    sauvegarderBase(fProps, fRegles, bf, bc);
}

int main() {
    Proposition *baseFaits = NULL;
    Regle *baseConnaissances = NULL;
    int choix = 0;

    printf("=== SYSTEME EXPERT LO21 (Projet Final) ===\n");

    do {
        printf("\n--- MENU PRINCIPAL ---\n");
        printf("1. Ajouter un FAIT\n");
        printf("2. Modifier un FAIT (Vrai/Faux/Inconnu)\n");
        printf("3. Supprimer un FAIT\n");
        printf("4. Ajouter une REGLE\n");
        printf("5. MODIFIER UNE REGLE (Ajout/Suppr prémisse)\n");
        printf("6. Supprimer une REGLE\n");
        printf("7. Afficher la Base de Faits\n");
        printf("8. Afficher la Base de Règles\n");
        printf("9. Lancer le Moteur d'Inférence\n");
        printf("10. CHARGER UNE BASE DEPUIS FICHIERS\n");
        printf("11. SAUVEGARDER la base\n");
        printf("0. Quitter\n");
        printf("Votre choix : ");

        if (scanf("%d", &choix) != 1) {
            while(getchar() != '\n')
                ; choix = -1;
        } else {
            viderBuffer();
        }

        switch (choix) {
            case 1:
                menuAjouterFait(&baseFaits);
                break;
            case 2:
                menuModifierFait(baseFaits, baseConnaissances);
                break;
            case 3:
                menuSupprimerFait(&baseFaits, baseConnaissances);
                break;
            case 4:
                menuAjouterRegle(&baseConnaissances, baseFaits);
                break;
            case 5:
                menuModifierRegle(baseConnaissances, baseFaits);
                break;
            case 6:
                menuSupprimerRegle(&baseConnaissances);
                break;
            case 7:
                printf("\n--- FAITS ---\n");
                if (baseFaits == NULL) printf("(Vide)\n");
                else afficherPropositions(baseFaits);
                break;
            case 8:
                printf("\n--- REGLES ---\n");
                if (baseConnaissances == NULL) printf("(Vide)\n");
                else afficherRegles(baseConnaissances);
                break;
            case 9:
                printf("\n--- MOTEUR ---\n");
                if (baseConnaissances == NULL) printf("Aucune règle.\n");
                else {
                    chainageAvant(&baseFaits, baseConnaissances);
                    printf("\n--- RESULTAT ---\n");
                    afficherPropositions(baseFaits);
                }
                break;
            case 10:
                menuCharger(&baseFaits, &baseConnaissances);
                break;
            case 11:
                menuSauvegarder(baseFaits, baseConnaissances);
                break;
            case 0:
                printf("Au revoir.\n");
                break;
            default:
                printf("Choix invalide.\n");
        }
    } while (choix != 0);

    libererPropositions(baseFaits);
    libererRegles(baseConnaissances);
    return 0;
}