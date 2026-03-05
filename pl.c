#include <stdio.h>
#include <stdlib.h>
//#include <ctype.h>
#include "pl.h"

/**
*   Affiche le 'message' demandant à l'utilisateur de saisir un entier et renvoie l'entier saisi.
*
*   @param message          Le message (demandant la valeur) à afficher.
*   @return                 L'entier entré par l'utilisateur.
*/
int int_input(const char *message) {
    int i;
    printf("%s", message);
    scanf("%d", &i);

    return i;
}

/**
*   Affiche le 'message' demandant à l'utilisateur de saisir un réel et renvoie le réél saisi.
*
*   @param message          Le message (demandant la valeur) à afficher.
*   @param pos
*   @return                 Le réel entré par l'utilisateur.
*   @sa int_input
*/
float float_input(const char *message, int pos) {
    float f;
    printf(message, pos);
    scanf("%f", &f);

    return f;
}


/**
*   Affiche le 'message' demandant à l'utilisateur de saisir un caractère et renvoie le caractère saisi.
*
*   @param message
*   @return                 Le caractère saisi par l'utilisateur.
*/
char char_input(const char *message) {
    char c;
    printf("%s", message);
    //ou bien while(getchar() != '\n');
    scanf(" %c", &c);

    return c;
}


/**
*   Cette fonction demande à l'utilisateur le nombre de variables de décisions que possède son programme et nous le renvoie.
*
*   @return n           Le nombre de variables de décisons.
*/
int nvd() {
    int n;
    do {
        puts("\n\t-----------------");
        n = int_input("\tEntrez le nombre de variables de décision du probleme : ");
    }while(n <= 0);

    return n;
}

/**
*   Cette fonction se charge de créer le tableau contenant les coefficients de la fonction objectif et de les récupérer.
*
*   @param  n         Le nombre de coefficients de la fonction objectif.
*   @return           Le tableau contenant les coefficients de la fonction objectif.
*/
float *ocoeffs(int n) {
    puts("\n\t-----------------");
    //Création du tableau devant contenir les coefficients de la fonction objectif.
    float *c = (float *) calloc(n, sizeof(float));
    //Récupération des coefficients de la fonction objectif.
    for(int i = 0; i < n; ++i) {
        c[i] = float_input("\tEntrez le coefficient de x%d dans la fonction objectif : ", i+1);
    }
    puts("\n\t-----------------");

    return c;
}

void affichage_de_z(const Programme_Lineaire *p) {
    printf("Z = ");
    for(int i = 0; i < p->columns; ++i) {
        printf("%f * x%d", p->objectif[i], i+1);
        if(i < p->columns-1)
            printf(" + ");
    }
}

void affichage_des_contraintes(const Programme_Lineaire *p) {
    for(int i = 0; i < p->rows; ++i) {
        printf("\t\t");
        for(int j = 0; j < p->columns; ++j) {
            printf("%f * x%d", p->contraintes[i].coeffs[j], j+1);
            if(j < p->columns-1)
                printf(" + ");
        }
        printf(" %c %f", p->contraintes[i].type, p->b[i]);
        printf("\n");
    }
}

void affichage_du_pl(const Programme_Lineaire *p) {
    printf("\t");
    p->type == 'm' ? printf("min ") : printf("Max ");
    affichage_de_z(p);
    puts("\n\ts.a :");
    affichage_des_contraintes(p);

}

void gestion_du_type_de_pl(Programme_Lineaire *p) {
    char t;
    do {
        t = char_input("\n\tQuel type de probleme souhaitez-vous resoudre ? m (minimisation) / M (Maximisation) : ");
    }while(t != 'm' && t != 'M');
    p->type = t;
}

void gestion_de_la_fonction_objectif(Programme_Lineaire *p) {
    //Récupération du nombre de variables de décision (coefficients) dans la fonction objectif.
    p->columns = nvd();
    p->objectif = ocoeffs(p->columns);
    char o_ok;
    do {
        short i, option;

        //Affichage de la fonction objectif pour s'assurer que les bonnes valeurs ont été récupérées.
        printf("\n\n\tVotre fonction objectif est donc : ");
        affichage_de_z(p);

        //Gestion des modifications sur la fonction objectif.
        o_ok = char_input("\n\tY a-t-il des modifications que vous souhaiteriez apporter ? y (Oui) / n (Non) : ");
        if(o_ok == 'y') {
            puts("\n\t\t\t\t\t\t\tModifications possibles\n");
            puts("\t\t\t1. Modifier le coefficient d'une des variables de la fonction objectif");
            puts("\t\t\t2. Modifier le nombre de variables de décisions du problème (NB: Dans ce cas il vous faudra re-entrer tous les coefficients !)");
            puts("\n\t\t\t0. Oups, je me suis trompé ! Je n'ai aucune modification à faire.");
            option = (short) int_input("\t\t\tQuelle modification souhaitez vous apporter ? : ");

            switch(option) {
                case 0:
                    o_ok = 'n';
                    break;
                case 1:
                    i = int_input("\n\tVeuillez entrer le numero de la variable dont vous souhaitez changer le coefficient : ");
                    p->objectif[i-1] = float_input("\tEntrez le nouveau coefficient de la variable %d de la fonction objectif : ", i);
                    break;
                case 2:
                    free(p->objectif);
                    p->objectif = NULL;
                    p->columns = nvd();
                    p->objectif = ocoeffs(p->columns);
                    break;
                default:
                    puts("\tVeuillez choisir parmi les options proposees !");
                    break;
            }
        }
    }while(o_ok != 'n');
}

void gestion_des_contraintes(Programme_Lineaire *p) {
    //Récupération du nombre de contraintes du problème.
    int r;
    do {
        r = int_input("\tEntrez le nombre de contraintes de votre probleme : ");
    }while(r <= 0);
    p->rows = r;
    //Création du tableau devant contenir toutes les contraintes.
    p->contraintes = (Contrainte *) calloc(p->rows, sizeof(Contrainte));
    //Création du tableau devant contenir le coté droit des contraintes.
    p->b = (float *) calloc(p->rows, sizeof(float));

    char cb_ok, ct;
    short option;
    int j;
    //Récupération des coefficients des contraintes du problème.
    for(int i = 0; i < p->rows; ++i) {
        //Création du tableau devant contenir les coefficients de la contrainte n° i.
        p->contraintes[i].coeffs = (float *) calloc(p->columns, sizeof(float));
        printf("\tContrainte N°%d :\n", i+1);
        //Parcours du tableau pour stocker les coefficients.
        for(j = 0; j < p->columns; ++j) {
            printf("\t\tVeuillez entrer le coefficient de x%d dans la contrainte %d : ", j+1, i+1);
            scanf("%f", &p->contraintes[i].coeffs[j]);
        }

        ct = char_input("\t\tVeuillez entrer le type de la contrainte (<, >, =) : ");
        if(ct == '\n')
            p->contraintes[i].type = '<';
        else
            p->contraintes[i].type = ct;



        p->b[i] = float_input("\t\tVeuillez entrer le cote droit de la contrainte %d : ", i+1);
        printf("\n");

        do{
            printf("\n\tLa contrainte N°%d est donc :\t", i+1);
            for(j = 0; j < p->columns; ++j) {
                printf("%f * x%d", p->contraintes[i].coeffs[j], j+1);
                if(j < p->columns-1)
                    printf(" + ");
            }
            printf(" %c ", p->contraintes[i].type);
            printf("%f", p->b[i]);

            cb_ok = char_input("\n\tY a-t-il des modifications que vous souhaiteriez apporter ? y (Oui) / n (Non) : ");
            if(cb_ok == 'y') {
                puts("\n\t\t\t\t\t\t\tModifications possibles\n");
                puts("\t\t\t1. Modifier le coefficient de l'une des variables");
                puts("\t\t\t2. Modifier le cote droit de la contrainte");
                puts("\t\t\t3. Modifier le type de la contrainte");
                puts("\n\t\t\t0. Oups, je me suis trompé ! Je n'ai aucune modification à faire.");
                option = (short) int_input("\t\t\tQuelle modification souhaitez vous apporter ? : ");

                switch(option) {
                    case 0:
                        cb_ok = 'n';
                        break;
                    case 1:
                        j = int_input("\n\tVeuillez entrer le numéro de la variable dont vous souhaitez changer le coefficient : ");
                        p->contraintes[i].coeffs[j-1] = float_input("\tEntrez le nouveau coefficient de la variable %d : ", j);
                        break;
                    case 2:
                        p->b[i] = float_input("\n\tVeuillez entrer le cote droit de la contrainte %d : ", i+1);
                        break;
                    case 3:
                        p->contraintes[i].type = char_input("\n\tVeuillez entrer le type de la contrainte (<, >, =) : ");
                        break;
                    default:
                        puts("Veuillez choisir parmi les options proposees !");
                        break;
                }

            }
        }while(cb_ok != 'n');
        puts("");
    }
}


//Programme_Lineaire *pl_apartir_dun_fichier(const char *file) {
//    FILE *f = fopen(file, "r");
//    if(!f) {
//        printf("Impossible d'ouvrir le fichier : %s !!!\n", file);
//        exit(EXIT_FAILURE);
//    }
//
//    //
//    char type = getc(f);
//
//    char c = getc(f);
//    if(c != ':') {
//        printf("Erreur : ");
//        return NULL;
//    }
//
//    do{
//        c = getc(f);
//    }
//    while( isspace(c) && c != EOF );
//    if(c != '[') {
//        printf("Erreur : ");
//        return NULL;
//    }
//
//    float x;
//    fscanf(f, "%f", &x);
//
//    return NULL;
//}


void clean(Programme_Lineaire *p) {
    //Suppression du tableau contenant les contraintes.
    free(p->objectif);
    p->objectif = NULL;
    //
    for(int i = 0; i < p->rows; i++) {
        //Suppression du tableau contenant les coefficients de la contrainte N° i.
        free(p->contraintes[i].coeffs);
        p->contraintes[i].coeffs = NULL;
    }
    //Suppression du tableau contenant toutes les contraintes.
    free(p->contraintes);
    p->contraintes = NULL;
    //Suppression du tableau contenant le coté droit des contraintes.
    free(p->b);
    p->b = NULL;
}
