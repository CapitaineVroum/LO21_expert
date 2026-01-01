#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "proposition.h"
#include "regle.h"
#include "inference.h"
#include "liste.h"

/* Fonction utilitaire pour vider le buffer clavier */
void viderBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

/* Fonction pour lire une chaine proprement */
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

/* Menu de modification avec gestion de la non-monotonie */
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
        while(getchar() != '\n'); val = -2;
    }
    viderBuffer();

    // 1. Mise à jour brute
    if (val == 1) p->valeur = VALEUR_VRAIE;
    else if (val == 0) p->valeur = VALEUR_FAUSSE;
    else if (val == -1) p->valeur = VALEUR_INCONNUE;
    else {
        printf("Valeur invalide.\n");
        return;
    }

    printf("\n--- CYCLE DE MISE A JOUR ---\n");

    // 2. Invalidation
    printf("1. Invalidation des conséquences...\n");
    invaliderConsequences(bf, bc, nom);

    // 3. Recalcul
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
        if (scanf("%d", &typeCond) != 1) typeCond = 1; // Par défaut Vrai
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

void menuSupprimerFait(Proposition **bf, Regle *bc) {
    char nom[50];

    printf("\n--- SUPPRESSION D'UN FAIT ---\n");
    afficherPropositions(*bf);
    printf("Nom du fait à supprimer : ");
    lireChaine(nom, 50);

    // 1. Sécurité : Vérifier s'il existe
    Proposition *p = chercherProposition(*bf, nom);
    if (p == NULL) {
        printf("Erreur : Ce fait n'existe pas.\n");
        return;
    }

    // 2. IMPORTANT : Invalider les conséquences AVANT de supprimer
    // Car une fois supprimé, le moteur ne le trouvera plus pour dire "Ah tiens, il a changé"
    // On considère que sa suppression équivaut à le passer à "INCONNU" pour le système
    printf("Invalidation des déductions liées...\n");
    invaliderConsequences(*bf, bc, nom);

    // 3. Suppression effective
    supprimerProposition(bf, nom);
    printf("Fait '%s' supprimé de la base.\n", nom);

    // 4. Relance du moteur (Optionnel, pour nettoyer l'affichage)
    chainageAvant(bf, bc);
}

void menuSupprimerRegle(Regle **bc) {
    int choix;

    printf("\n--- SUPPRESSION D'UNE REGLE ---\n");
    // On affiche les règles avec leur numéro
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

int main() {
    Proposition *baseFaits = NULL;
    Regle *baseConnaissances = NULL;
    int choix = 0;

    printf("=== SYSTEME EXPERT LO21 (Avancé) ===\n");

    do {
        printf("\n--- MENU PRINCIPAL ---\n");
        printf("1. Ajouter un FAIT (Créer)\n");
        printf("2. Modifier un FAIT (Active le recalcul)\n");
        printf("3. Supprimer un FAIT\n");
        printf("4. Ajouter une REGLE\n");
        printf("5. Supprimer une REGLE\n");
        printf("6. Afficher la Base de Faits\n");
        printf("7. Afficher la Base de Règles\n");
        printf("8. Lancer le Moteur d'Inférence\n");
        printf("9. DEMO : Tester le TAD Regle\n");
        printf("0. Quitter\n");
        printf("Votre choix : ");

        if (scanf("%d", &choix) != 1) {
            while(getchar() != '\n'); choix = -1;
        } else {
            viderBuffer();
        }

        switch (choix) {
            case 1:
                menuAjouterFait(&baseFaits);
                break;
            case 2:
                // Correction ici : on passe les 2 bases
                menuModifierFait(baseFaits, baseConnaissances);
                break;
            case 3:
                menuSupprimerFait(&baseFaits, baseConnaissances);
                break;
            case 4:
                menuAjouterRegle(&baseConnaissances, baseFaits);
                break;
            case 5:
                menuSupprimerRegle(&baseConnaissances);
                break;
            case 6:
                printf("\n--- FAITS ---\n");
                if (baseFaits == NULL) printf("(Vide)\n");
                else afficherPropositions(baseFaits);
                break;
            case 7:
                printf("\n--- REGLES ---\n");
                if (baseConnaissances == NULL) printf("(Vide)\n");
                else afficherRegles(baseConnaissances);
                break;
            case 8:
                printf("\n--- MOTEUR ---\n");
                if (baseConnaissances == NULL) printf("Aucune règle.\n");
                else {
                    chainageAvant(&baseFaits, baseConnaissances);
                    printf("\n--- ETAT FINAL ---\n");
                    afficherPropositions(baseFaits);
                }
                break;
            case 9:
                printf("\n--- DEMO TAD ---\n");
                Regle *demo = creerRegleVide();
                definirConclusion(demo, "TestConcl");

                // Correction ici : ajout du 3ème argument (1 pour VRAI)
                ajouterPropositionPremisse(demo, "A", 1);
                ajouterPropositionPremisse(demo, "B", 1);
                ajouterPropositionPremisse(demo, "C", 1);

                printf("Regle de démo créée en mémoire.\n");
                printf("Tête prémisse : %s\n", tetePremisse(demo));

                libererRegles(demo);
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