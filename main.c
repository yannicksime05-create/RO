#include <stdio.h>
#include <stdbool.h>
#include "pl.h"
#include "resolution.h"

bool start() {
    char c;
    puts("\n\t\t\tSalut, je m'appelle sky, votre Assistant de Resolution de Programme Lineaire (ARPL)");
    puts("\n\tAvant de continuer, petite information :");
    puts("\n\t\t1. Je ne vous demanderai pas de contraintes sur les variables car j'ai ete programme pour supposer qu'elles sont toujours toutes soumises à des contraintes de non-negativite.");
    printf("\n\tSi cela est clair, pouvons nous continuer ? y (Oui) / n (Non) : ");
    scanf("%c", &c);

    return c == 'y';
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

        gestion_du_type_de_pl(&primal);
        printf("\tVous souhaitez donc resoudre un probleme de %s.\n", (primal.type == 'm') ? "minimisation" : "Maximisation");

        gestion_de_la_fonction_objectif(&primal);
        puts("\n\n\t-----------------");

        gestion_des_contraintes(&primal);
        //puts("\n\t-----------------");
        puts("\n\n\tVous souhaitez donc resoudre le PL suivant :");
        affichage_du_pl(&primal);
////

        transformation_avant_simplexe(&primal);
        methode_du_simplexe(&primal);
//        methode_du_simplexe();

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
