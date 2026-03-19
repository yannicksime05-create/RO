#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "pl.h"
#include "resolution.h"

bool start() {
    char c;
    puts("\n\t\t\tSalut, je m'appelle sky, votre Assistant de Resolution de Programme Lineaire (ARPL)");
    puts("\n\tAvant de continuer, petite information :");
    puts("\n\t\t1. Je ne vous demanderai pas de contraintes sur les variables car j'ai ete programme pour supposer qu'elles sont toujours toutes soumises à des contraintes de non-negativite.");
    do{
        printf("\n\tSi cela est clair, pouvons nous continuer ? y (Oui) / n (Non) : ");
        scanf("%c", &c);
        if(c == '\n') c = 'y';
    }while(c != 'y');

    return c == 'y';
}

char *remove_newline(char *s) {
    s[strcspn(s, "\n")] = 0;

    return s;
}


int main(void) {
//    if(argc == 2) {
//        File *file = fopen(argv[1], "r");
//        if(!file) {
//            printf("Impossible d'ouvrir le fichier : %s", argv[1]);
//            return -1;
//        }
//
//        pl_apartir_dun_fichier(file);
//
//    }

    if( start() ) {
        puts("\n");
        Programme_Lineaire primal = { 0 };

        time_t start1;
        time(&start1);

        gestion_du_type_de_pl(&primal);
        printf("\tVous souhaitez donc resoudre un probleme de %s.\n", (primal.type == 'm') ? "minimisation" : "Maximisation");

        gestion_de_la_fonction_objectif(&primal);
        puts("\n\n\t-----------------");

        gestion_des_contraintes(&primal);

        time_t end1 = time(NULL);

        puts("\n\n\tVous souhaitez donc resoudre le PL suivant :");
        affichage_du_pl(&primal);

        time_t start2;
        time(&start2);

//        methode_graphique(&primal);
        methode_du_simplexe(&primal);
//        methode_duale_du_simplexe(&primal);

        time_t end2;
        time(&end2);

        printf("\n\tLa recuperation des valeurs a commence a : %s.\n", remove_newline(ctime(&start1)));
        printf("\tElle s'est termine a : %s, et a dure : %.2lfs.\n", remove_newline(ctime(&end1)), difftime(end1, start1));
        printf("\n\tLa resolution a commence a : %s.\n", remove_newline(ctime(&start2)));
        printf("\tElle s'est termine a : %s, et a dure : %.2lfs.\n", remove_newline(ctime(&end2)), difftime(end2, start2));


//        graphique(&primal);

//        //M = 77, m = 109, 109 - 77 = 32
//        Programme_Lineaire dual;
//        dual.objectif = primal.b;
//        /*Le but de cette partie étant uniquement de tester l'affichage du dual, nous n'avons pas besoin de
//            réellement transposer la matrice des contraintes du primal, juste de changer la façon dont on y accède dans une boucle.
//
//          Mais peut-être plus tard faudra t-il vraiment calculer cette transposée...
//        */
//        dual.contraintes = primal.contraintes;
//        dual.b = primal.objectif;
//        dual.rows = primal.columns;
//        dual.columns = primal.rows;
//        dual.type = (primal.type == 'm') ? 'M' : 'm';



        clean(&primal);
    }

    return 0;
}
