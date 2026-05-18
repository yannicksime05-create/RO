#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <time.h>
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

Programme_Lineaire *create() {
    Programme_Lineaire *p = malloc(sizeof(Programme_Lineaire));
    if(!p) {
        fprintf(stderr, "Impossible de trouver l'espace memoire pour stocker le programme. Code d'erreur: %d\n%s\n", errno, strerror(errno));
        return NULL;
    }

    p->rows = p->columns = 0;
    p->objectif = NULL;
    p->contraintes = NULL;
    p->b = NULL;

    return p;
}


void affichage_de_z(const Programme_Lineaire *p, FILE *f) {
    fprintf(f, "Z = ");
    for(int i = 0; i < p->columns; ++i) {
        fprintf(f, "%f * x%d", p->objectif[i], i+1);
        if(i < p->columns-1)
            fprintf(f, " + ");
    }
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
        affichage_de_z(p, NULL);

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

void gestion_du_pl(Programme_Lineaire *p) {
    gestion_du_type_de_pl(p);
    printf("\tVous souhaitez donc resoudre un probleme de %s.\n", (p->type == 'm') ? "minimisation" : "Maximisation");
    gestion_de_la_fonction_objectif(p);
    puts("\n\n\t-----------------");
    gestion_des_contraintes(p);
}


void affichage_des_contraintes(const Programme_Lineaire *p, FILE *f) {
    for(int i = 0; i < p->rows; ++i) {
        fprintf(f, "\t\t");
        for(int j = 0; j < p->columns; ++j) {
            fprintf(f, "%f * x%d", p->contraintes[i].coeffs[j], j+1);
            if(j < p->columns-1)
                fprintf(f, " + ");
        }
        fprintf(f, " %c %f", p->contraintes[i].type, p->b[i]);
        fprintf(f, "\n");
    }
}

void affichage_du_pl(const Programme_Lineaire *p, FILE *outputfile) {
    FILE *f = outputfile;
    if(!f) f = stdout;

    fprintf(f, "\t");
    fprintf(f, p->type == 'm' ? "min " : "Max ");

    affichage_de_z(p, f);
    fputs("\n\ts.a :\n", f);
    affichage_des_contraintes(p, f);

}


//begin: m,      n
//^      ^       ^
//i = 1, i = 2,  i = 3
void mn_apartir_du_fichier(const int i, const char *tmp, Programme_Lineaire *p) {
    if(i == 2) {
        p->rows = atoi(tmp);
//        printf("rows = %d\n", p->rows);
    }
    else if(i == 3) {
        p->columns = atoi(tmp);
//        printf("columns = %d\n", p->columns);
    }
}

//z:     c1,   , ... ,    n
//^      ^                ^
//i = 1, i = 2 , ... ,    i = n+1
bool fonction_objectif_apartir_du_fichier(const int i, const char *tmp, Programme_Lineaire *p) {
    if(i == 1) {
        p->type = tmp[0] == 'z' ? 'm' : 'M';
//        printf("type = %s\n", p->type == 'm' ? "min" : "max");
    }
    else if(i-1 <= p->columns) {
        if(!p->objectif) {
            p->objectif = calloc(p->columns, sizeof(float));
            if(!p->objectif) {
                fprintf(stderr, "Code d'erreur: %d\n%s\n", errno, strerror(errno));
                return false;
            }
        }

        p->objectif[i-2] = atof(tmp);
//                    strtof();
//                    strtod();
    }

    return true;
}

//c:        a11,    a12,   ... ,    a1n,        (<, >, =),  b1
//^         ^       ^               ^           ^           ^
//i = 1,    i = 2,  i = 3, ... ,    i = n+1,    i = n+2,    i = n+3
bool contrainte_apartir_du_fichier(const int lineno, const int i, const char *tmp, Programme_Lineaire *p) {
    if(i == 1) return true;

    if(!p->contraintes) {
        p->contraintes = calloc(p->rows, sizeof(Contrainte));
        if(!p->contraintes) {
            fprintf(stderr, "Impossible de stocker %d contraintes. Code d'erreur: %d\n%s\n", p->rows, errno, strerror(errno));
            return false;
        }
    }

    if(!p->contraintes[lineno-3].coeffs) {
        p->contraintes[lineno-3].coeffs = calloc(p->columns, sizeof(float));
        if(!p->contraintes[lineno-3].coeffs) {
            fprintf(stderr, "Impossible de stocker les coefficients de la contrainte n°%d. Code d'erreur: %d\n%s\n", lineno-2, errno, strerror(errno));
            return false;
        }
    }

    if(i-1 == p->columns+2 && !p->b) {
        p->b = calloc(p->rows, sizeof(float));
        if(!p->b) {
            fprintf(stderr, "Impossible de stocker le cote droit de la contrainte n°%d. Code d'erreur: %d\n%s\n", lineno-2, errno, strerror(errno));
            return false;
        }
    }

//            printf("contrainte n°%d, coeff[%d] = %lf\n", lineno-2, i-2, p->contraintes[lineno-3].coeffs[i-2]);
    if(i-1 <= p->columns)           p->contraintes[lineno-3].coeffs[i-2] = atof(tmp);
    else if(i-1 == p->columns + 1)  p->contraintes[lineno-3].type = tmp[strlen(tmp) - 1];
    else if(i-1 == p->columns + 2)  p->b[lineno-3] = atof(tmp);

    return true;
}

//syntaxe:
//begin: m, n
//z: c1, c2, ... , cn
//c: a11, a12, ... , a1n, (<, >, =), b1
//.
//.
//.
//c: am1, am2, ... , amn, (<, >, =), bm
//end:
Programme_Lineaire *pl_apartir_dun_fichier(const char *filename) {
    FILE *f = fopen(filename, "r");
    if(!f) {
        fprintf(stderr, "\n\tImpossible d'ouvrir le fichier : %s\n", filename);
        return NULL;
    }

    Programme_Lineaire *p = create();
    if(!p) {
        fclose(f);
        f = NULL;
        return NULL;
    }

    char *line = NULL, *tmp = NULL;
    bool error = false;
    int lineno = 1, i;
    size_t n;
    while( getline(&line, &n, f) != -1 && strcmp(line, "end:\n") != 0 ) {
        line[strcspn(line, "\n")] = 0;
//        printf("lineno: %d, line: \"%s\", n = %ld\n", lineno, line, n);

        i = 1;
        tmp = strtok(line, ":");
        while(tmp) {
//            printf("tmp = %s\n", tmp);

            //on est sur la ligne "begin: m, n"
            if(lineno == 1) {
                mn_apartir_du_fichier(i, tmp, p);
            }
            //on est sur la ligne "z: c1, ..., cn"
            else if( lineno == 2 && !fonction_objectif_apartir_du_fichier(i, tmp, p) ) {
                error = true;
                break;
            }
            //c: a11, a12, ... , a1n, (<, >, =), b1
            else if( lineno > 2 && !contrainte_apartir_du_fichier(lineno, i, tmp, p) ) {
                error = true;
                break;
            }

            tmp = strtok(NULL, " ,");
            i++;
        }

        if(error) break;
        lineno++;
    }
    tmp = NULL;
    //A ce niveau, on est censé avoir : line = "end"
//    line[strcspn(line, "\n")] = 0;
//    printf("Apres la boucle\nlineno: %d, line: \"%s\", n = %ld\n", lineno, line, n);
    free(line);
    line = NULL;
    fclose(f);
    f = NULL;

    if(error) {
        if(p->b)                free(p->b);
        else if(p->objectif)    free(p->objectif);
        else if(p->contraintes) {
            for(int i = 0; i < p->rows; i++) {
                if(p->contraintes[i].coeffs) {
                    free(p->contraintes[i].coeffs);
                    p->contraintes[i].coeffs = NULL;
                }
            }
            free(p->contraintes);
        }

        p->objectif = NULL;
        p->b = NULL;
        p->contraintes = NULL;

        free(p);
        p = NULL;
    }

    return p;
}


void save_to_file(const char *filename, const Programme_Lineaire *p, const X *solutions) {
    FILE *f = fopen(filename, "a+");
    if(!f) {
        fprintf(stderr, "\n\tImpossible d'ouvrir le fichier : %s\n", filename);
        return;
    }
    fseek(f, 0, SEEK_END);

    if(solutions) {
        fputs("\n\tLes solutions sont :\n\n", f);
        for(int i = 0; i < p->columns - p->rows; i++) {
            fprintf(f, "\t\tx%d = %f\n", solutions[i].column, (solutions[i].row < 0) ? 0.0f : p->b[solutions[i].row]);
        }
        fputs("\n\t\t=============================================================================", f);
    }
    else {
        time_t now = time(NULL);
        fputs("\n\n\n\t\t=============================================================================\n\n", f);
        fprintf(f, "\t\t\t\t\t\t\tResolu le : %s\n", ctime(&now));
        fputs("\t\t\t\t\t====================================================\n", f);
        fputs("\n\tVous souhaitiez resoudre le PL suivant :\n\n", f);
        affichage_du_pl(p, f);
        fputs("\n\t\t\t\t\t====================================================\n", f);
    }

    fclose(f);
}

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

    free(p);
    p = NULL;
}
