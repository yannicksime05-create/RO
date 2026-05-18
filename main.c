#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pl.h"
#include "resolution.h"

bool pstart() {
    char c;
    puts("\n\t\t\t\tSalut, je m'appelle sky, votre Assistant de Resolution de Programme Lineaire (ARPL)");
    puts("\n\tAvant de continuer, petite information :");
    puts("\n\t\t1. Je ne vous demanderai pas de contraintes sur les variables car j'ai ete programme pour supposer qu'elles sont toujours toutes soumises à des contraintes de non-negativite.");
    do{
        printf("\n\tSi cela est clair, pouvons nous continuer ? y (Oui) / n (Non) : ");
        scanf("%c", &c);
        if(c == '\n') c = 'y';
    }while(c != 'y');

    return c == 'y';
}


int main(int argc, char *argv[]) {
    clock_t start, input_start, end, input_end;
    Programme_Lineaire *p = NULL;
    char *filename = NULL;
    X *solutions = NULL;
    bool cmd = false;

    if(argc == 2) {
        filename = argv[1];
        printf("\n\tFichier trouve : %s\n", filename);
        printf("\tExtraction du programme a partir du fichier...");

        input_start = clock();
        p = pl_apartir_dun_fichier(filename);
        input_end = clock();
        if(!p) {
            fprintf(stderr, "Impossible de creer le programme a partir du fichier fourni.\n");
            return -1;
        }

        printf("\t\tTermine. Temps mis : %fs\n", convert_to_seconds(input_end, input_start));
        save_to_file(filename, p, NULL);
    }
    else {
        cmd = true;
        puts("\n\tAucun fichier trouve dans la liste des arguments.\n\tBasculement en mode console...\n");

        if( !pstart() )  return -1;
        p = create();

        input_start = clock();
        gestion_du_pl(p);
        input_end = clock();
    }

    puts("\n\n\tVous souhaitez donc resoudre le PL suivant :");
    affichage_du_pl(p, NULL);

    start = clock();
//    methode_graphique(p);
    solutions = methode_du_simplexe(p);
//    methode_duale_du_simplexe(p);
    end = clock();

    if(cmd) printf("\n\tDuree de l'input: %fs\n", convert_to_seconds(input_end, input_start));
    else {
        save_to_file(filename, p, solutions);
        printf("\n\tLes resultats ont ete enregistres dans le fichier : %s\n", filename);
    }

    printf("\n\tDuree de la resolution: %fs\n", convert_to_seconds(end, start));


    free(solutions); solutions = NULL;
    clean(p); p = NULL;
    return 0;
}
