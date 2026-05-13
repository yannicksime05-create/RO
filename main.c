#include <stdio.h>
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
    }
    else {
        cmd = true;
        puts("\n\tAucun fichier trouve dans la liste des arguments.\n\tBasculement en mode console...\n");
        if( !pstart() )  return -1;

        p = create();

        input_start = clock();

        gestion_du_type_de_pl(p);
        printf("\tVous souhaitez donc resoudre un probleme de %s.\n", (p->type == 'm') ? "minimisation" : "Maximisation");
        gestion_de_la_fonction_objectif(p);
        puts("\n\n\t-----------------");
        gestion_des_contraintes(p);

        input_end = clock();
    }

    puts("\n\n\tVous souhaitez donc resoudre le PL suivant :");
    affichage_du_pl(p);

    start = clock();
//    methode_graphique(p);
    methode_du_simplexe(p);
//    methode_duale_du_simplexe(p);
    end = clock();

    if(cmd) printf("\n\tDuree de l'input: %fs\n", convert_to_seconds(input_end, input_start));
//    else    save_to_file(filename);

    printf("\n\tDuree de la resolution: %fs\n", convert_to_seconds(end, start));


    clean(p);
    p = NULL;
    return 0;
}
