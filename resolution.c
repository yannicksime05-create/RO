#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "resolution.h"

/**
*   @brief Cette fonction détermine et indique à l'utilisateur les points à tracer pour pouvoir résoudre son PL graphiquement.
*   Cette fonction n'est donc à utiliser que lorsque le programme linéaire de l'utilisateur est de degré 2.
*
*   Soit à déterminer les points permettant de tracer la contrainte : c1 * x1 + c2 * x2 = b
*
*   Pour ce faire, il faut remarquer que chaque coefficient (ci) de la contrainte permettra d'obtenir
*   l'abscisse et l'ordonnée d'un des points cherchés;
*
*   Explication:
*     Lorsqu'on essaie de déteminer x1 et x2 respectant la contrainte, le moyen le plus rapide est encore de mettre l'une
*     des deux variables à 0 et déterminer la valeur de l'autre en faisant b / ci, ce qui permet d'obtenir les points
*        - (0, b / ci) lorsque x1 = 0
*        - (b / ci, 0) lorsque x2 = 0
*
*   Le 'problème' avec ces points, c'est que si bi % ci != 0, on obtient un un point difficile à tracer.
*   Cette fonction essaye donc de toujours trouver des points plus facilement réprésentables i.e. avec des coordonnées entières.
*
*   Voici des exemples de résultats attendus par cette fonction :
*
*       Max Z = 2x1 + 3x2
*       s.à     2x1 + x2 <= 4               (2, 0), (0, 4)
*               x1 + 2x2 <= 5               (1, 2), (5, 0)
*               x1, x2 >= 0
*
*        -------------- ----------- -----------              -------------- ----------- -----------
*       | contrainte 1 |    P1     |    P2     |            | contrainte 2 |    P1     |    P2     |
*        -------------- ----------- -----------              -------------- ----------- -----------
*       |   x1         |    2      |    0      |            |   x1         |    1      |    5      |
*        -------------- ----------- -----------              -------------- ----------- -----------
*       |   x2         |    0      |    4      |            |   x2         |    2      |    0      |
*        -------------- ----------- -----------              -------------- ----------- -----------
*
*
*       Max Z = 30x1 + 20x2
*       s.à     2x1 + x2 <= 8               (4, 0), (0, 8)
*               x1 + 3x2 <= 8               (2, 2), (8, 0)
*               x1, x2 >= 0
*
*        -------------- ----------- -----------              -------------- ----------- -----------
*       | contrainte 1 |    P1     |    P2     |            | contrainte 2 |    P1     |    P2     |
*        -------------- ----------- -----------              -------------- ----------- -----------
*       |   x1         |    4      |    0      |            |   x1         |    2      |    8      |
*        -------------- ----------- -----------              -------------- ----------- -----------
*       |   x2         |    0      |    8      |            |   x2         |    2      |    0      |
*        -------------- ----------- -----------              -------------- ----------- -----------
*
*
*       Max Z = 5x1 + 4x2
*       s.à     6x1 + 4x2 <= 24             (4, 0), (0, 6)
*               x1 + 2x2 <= 6               (6, 0), (0, 3)
*               -x1 + x2 <= 1               (1, 2), (0, 1)
*                     x2 <= 2
*               x1, x2 >= 0
*
*        -------------- ----------- -----------              -------------- ----------- -----------              -------------- ----------- -----------
*       | contrainte 1 |    P1     |    P2     |            | contrainte 2 |    P1     |    P2     |            | contrainte 3 |    P1     |    P2     |
*        -------------- ----------- -----------              -------------- ----------- -----------              -------------- ----------- -----------
*       |   x1         |    4      |    0      |            |   x1         |    6      |    0      |            |   x1         |    1      |    0      |
*        -------------- ----------- -----------              -------------- ----------- -----------              -------------- ----------- -----------
*       |   x2         |    0      |    6      |            |   x2         |    0      |    3      |            |   x2         |    2      |    1      |
*        -------------- ----------- -----------              -------------- ----------- -----------              -------------- ----------- -----------
*
*
*       Max Z = 5x1 + 6x2
*       s.à     x1 + 2x2 <= 3               (3, 0), (1, 1)
*               2x1 + 2x2 <= 4              (2, 0), (0, 2)
*               3x1 + x2 <= 5               (1, 2), (0, 5)
*
*        -------------- ----------- -----------              -------------- ----------- -----------
*       | contrainte 1 |    P1     |    P2     |            | contrainte 2 |    P1     |    P2     |
*        -------------- ----------- -----------              -------------- ----------- -----------
*       |   x1         |    3      |    1      |            |   x1         |    2      |    0      |
*        -------------- ----------- -----------              -------------- ----------- -----------
*       |   x2         |    0      |    1      |            |   x2         |    0      |    2      |
*        -------------- ----------- -----------              -------------- ----------- -----------
*
*
*   @param *p       L'adresse du programme linéaire afin de pouvoir accéder à toutes ses données.
*/
void methode_graphique(Programme_Lineaire *p) {
    Point p1 = {0}, p2 = {0};

//    Point points[2];


    float cj;
    for(int i = 0; i < p->rows; i++) {
//        for(int j = 0; j < p->columns; j++) {
//            cj = p->contraintes[i].coeffs[j];
//
//            if(cj < 0) {
//                points[j].abs = (j == 0) ? 1 : (int) ( fabs(cj) + p->b[i] ) / p->contraintes[i].coeffs[0];
//                points[j].ord = (j == 1) ? 1 : (int) ( fabs(cj) + p->b[i] ) / p->contraintes[i].coeffs[1];
//            }
//            else if(cj == 0.0f) {
//                points[j].abs = 0;
//                points[j].ord = 0;
//            }
//            else {
//                points[j].abs = (j == 0) ? (int) floor(p->b[i] / cj) : ((int) p->b[i]) % ((int) cj);
//                points[j].ord = (j == 1) ? (int) floor(p->b[i] / cj) : ((int) p->b[i]) % ((int) cj);
//            }
//        }




        //La façon de trouver le point dépend grandement de la valeur du coefficient
        //  - Si le coefficient est > 0, les coordonnées du point qu'il engendre se trouvent grâce à la formule : b = q * ci + r
        //avec :
        //  b, le coté droit de la contrainte,
        //  q, le quotient de la division entière,
        //  ci, le coefficient i de la contrainte,
        //  r, le reste de la division.
        //Et les points cherchés sont :
        //  P1(q, r) lorsque le coefficient est celui de x1,
        //  P2(r, q) lorsque le coefficient est celui de x2.
        //
        //Ex:   3x1 + x2 <= 5
        //  En utilisant le coeff de x1, on a : 5 = 1 * 3 + 2 => P1(1, 2)
        //  En utilisant le coeff de x2, on a : 5 = 5 * 1 + 0 => P2(0, 5)
        //
        //
        //  - Si le coefficient est nul, (dans ce cas le second coefficient est obligatoirement non nul sinon, la contrainte
        //elle-meme n'existe pas), l'un des deux points pour tracer la contrainte est alors le point O(0, 0).
        //Et le second point se trouve soit en allant au 1er cas (ci-dessus), soit en allant au 3ème cas (ci-dessous).
        //
        //Ex:   0 * x1 + x2 <= 2
        //  Le coefficient de x1 est nul donc on a le point P1 = O(0, 0),
        //  Le coefficient de x2 est positif donc on applique le 1er cas :
        //      2 = 2 * 1 + 0 => P2(0, 2).
        //
        //  Beaucoup de bavardages pour rien sur ce deuxième cas (en vrai, tout ça pour un truc comme : x2 <= 2, sérieux ?!),
        //mais étant donné que je suis en train de documenter tous les cas, il fallait mentionner également celui-ci.
        //
        //
        //  - Si le coefficient est < 0, (probablement le cas le plus difficile)
        cj = p->contraintes[i].coeffs[0];
        if(cj > 0) {
            p1.ord = ((int) p->b[i]) % ((int) cj);

            //Solution possiblement temporaire
            p1.abs = (int) floor((p->b[i] - p->contraintes[i].coeffs[1] * p1.ord) / cj);

//            p1.abs = (int) floor(p->b[i] / cj);
        }
        else if(!cj) {
            p1.abs = 0;
            p1.ord = 0;
        }
        else {
//            p1.abs = 1;
//            p1.ord = (int) ( fabs(cj) + p->b[i] ) / p->contraintes[i].coeffs[1];

            p1.abs = (int) ( p->b[i] * p->contraintes[i].coeffs[1] - p->b[i] );
            p1.ord = (int) p->b[i];
        }


        cj = p->contraintes[i].coeffs[1];
        if(cj > 0) {
            p2.abs = ((int) p->b[i]) % ((int) cj);

            //Solution possiblement temporaire
            p2.ord = (int) floor((p->b[i] - p->contraintes[i].coeffs[0] * p2.abs) / cj);

//            p2.ord = (int) floor(p->b[i] / cj);
        }
        else if(!cj) {
            p2.abs = 0;
            p2.ord = 0;
        }
        else {
//            p2.abs = (int) ( fabs(cj) + p->b[i] ) / p->contraintes[i].coeffs[0];
//            p2.ord = 1;

            p2.abs = (int) p->b[i];
            p2.ord = (int) ( p->b[i] * p->contraintes[i].coeffs[0] - p->b[i] );
        }

        //À ce niveau on connait déjà tous les points utiles pour tracer la contrainte. Il ne reste donc plus qu'à les afficher.
        printf("Pour la contrainte %d, tracez les points :\n", i+1);
        printf("\t\tP1 (%d, %d)\n", p1.abs, p1.ord);
        printf("\t\tP2 (%d, %d)\n", p2.abs, p2.ord);
    }
}


void sort(X *solutions, const int size) {
    int c;
    X tmp;

    for(int i = 0; i < size; i++) {
        c = solutions[i].column;

        if(c >= 0) {
            tmp = solutions[c];
            solutions[c] = solutions[i];
            solutions[i] = tmp;
        }
        else {
            solutions[i].column = i;
        }
    }
}


/**
*   Cette fonction renvoie l'indice de l'élément le plus négatif dans le tableau.
*/
int min(const float *b, const int size) {
    int l = 0, r = size - 1;

    while(l < r) {
        if(b[l] < b[r])      r--;
        else if(b[l] > b[r]) l++;
        else {
            l++;
            r--;
        }
    }

    return l;
}

/**
*   Cette fonction sert de condition d'arrêt à la fois pour la méthode du simplexe et pour la méthode duale du simplexe.
*
*   Dans la méthode du simplexe, on s'arrête lorsque tous les coefficients de la fonction objectif (z)
*   sont non-négatifs; dans ces cas là l'appel de cette fonction ressemblera à :    condition_arret(z, column)
*
*   Tandis que dans la méthode duale, on s'arrête lorsque tous les éléments du côté droit (b)
*   sont non-négatifs;  dans ces cas là l'appel de cette fonction ressemblera à :    condition_arret(b, row)
*/
bool condition_arret(const float *arr, const int size) {
    for(int i = 0; i < size; i++) {
        if(arr[i] < 0) return false;
    }

    return true;
}

/**
*/
void changement_de_base(Programme_Lineaire *p, X *solutions, int in_variable_index, int out_variable_index, int row, int column) {

    /// Ces variables ne sont là que pour m'éviter d'avoir à écrire p->... à chaque fois.
    float *z = p->objectif;
    float *b = p->b;
    Contrainte *contraintes = p->contraintes;

    printf("\t\tin column = %d, out row = %d\n", in_variable_index+1, out_variable_index+1);

    //Si l'indice de la variable entrante est < au nombre de variables de décisions,
    //alors c'est une des variables de décisions qui entre dans la base.
    if(in_variable_index < p->columns) {
        //la longueur de 'solutions' est p->columns, or
        // 0 <= out_variable_index <= p->columns + p->rows.
        //Pour un PL contenant donc plus de contraintes que de variables de décisions,
        //on ne peut pas utiliser directement out_variable_index,
        //on risquerait d'accéder à une zone qui n'est pas dans le tableau.
        int pos = out_variable_index >= p->columns ? (out_variable_index+1) % p->columns : out_variable_index;
        solutions[pos] = (X) {out_variable_index, in_variable_index};
    }


    float pivot = contraintes[out_variable_index].coeffs[in_variable_index];
    printf("\t\tpivot = %.2f\n", pivot);

    b[out_variable_index] /= pivot;
    float column_coeff = z[in_variable_index];

    for(int i = 0; i < column; ++i) {
        contraintes[out_variable_index].coeffs[i] /= pivot;
        z[i] -= column_coeff * contraintes[out_variable_index].coeffs[i];
    }

    printf("\t\tp-row devient : ");
    for(int i = 0; i < column; ++i) printf("%5.2f, ", contraintes[out_variable_index].coeffs[i]);
    printf("\n");
    printf("\t\tz-row devient : ");
    for(int i = 0; i < column; ++i)  printf("%5.2f, ", z[i]);
    printf("\n");


    for(int i = 0; i < row; ++i) {
        if(i == out_variable_index) continue;

        column_coeff = contraintes[i].coeffs[in_variable_index];
        b[i] -= column_coeff * b[out_variable_index];
        for(int j = 0; j < column; ++j)
            contraintes[i].coeffs[j] -= column_coeff * contraintes[out_variable_index].coeffs[j];
    }

    printf("\t\tles contraintes deviennent :\n");
    for(int i = 0; i < row; ++i) {
        if(i == out_variable_index) continue;

        printf("\t\t\t\t");
        for(int j = 0; j < column; ++j) printf("%5.2f, ", contraintes[i].coeffs[j]);
        printf("\n");
    }

    printf("\t\tb-row devient : ");
    for(int i = 0; i < row; ++i)    printf("%5.2f, ", b[i]);
    printf("\n");

    z = NULL;
    b = NULL;
    contraintes = NULL;
}

void transformation_avant_simplexe(Programme_Lineaire *p) {
    short n_columns_to_add = 0;
    for(int i = 0; i < p->rows; i++) {
        if(p->contraintes[i].type == '>') n_columns_to_add += 2;
        else ++n_columns_to_add;
    }
    size_t total_of_columns = p->columns + n_columns_to_add;

    p->objectif = (float *) realloc(p->objectif, total_of_columns);
    if(p->type == 'M') {
        for(int i = p->columns; i < total_of_columns; ++i)
            p->objectif[i] = 0.0f;
    }

    for(int i = 0; i < p->rows; ++i) {
        p->contraintes[i].coeffs = (float *) realloc(p->contraintes[i].coeffs, total_of_columns);

        if(p->contraintes[i].type == '<') {
            for(int j = p->columns; j < total_of_columns; ++j) {
                p->contraintes[i].coeffs[j] = (j == i+p->columns) ? 1.0f : 0.0f;
            }
        }
    }

    if(p->type == 'M') {
        for(int i = 0; i < p->columns; i++)
            p->objectif[i] *= -1;
    }
}

int indice_variable_sortante_simplexe(const float *b, const Contrainte *contraintes, const int row, const int in_variable_index) {
    float ratio = 0.0f, min_ratio = -1.0f;
    int index = 0;

    for(int i = 0; i < row; ++i) {
        ratio = b[i] / contraintes[i].coeffs[in_variable_index];
        if( ratio >= 0 && (min_ratio < 0 || ratio < min_ratio) ) {
            min_ratio = ratio;
            index = i;
        }
    }

    return index;
}

/**
*/
void methode_du_simplexe(Programme_Lineaire *p) {
    int row = p->rows, column = p->columns + p->rows;

    transformation_avant_simplexe(p);

    //le nombre de x que l'on souhaite est égal au nombre de variable de décisions du problème donc : p->columns au lieu de column.
    X *solutions = malloc(p->columns * sizeof(X));
    if(!solutions) {
        printf("Error: Couldn't create the array of solutions!\n");
        return;
    }

    for(int i = 0; i < p->columns; i++) {
        solutions[i].row = solutions[i].column = -1;
    }


    int iteration = 1;
    do{
        printf("\n\tIteration %d :\n", iteration);

        int in_variable_index = min(p->objectif, column) % column;
        int out_variable_index = indice_variable_sortante_simplexe(p->b, p->contraintes, row, in_variable_index);

        changement_de_base(p, solutions, in_variable_index, out_variable_index, row, column);

        ++iteration;
    }while( !condition_arret(p->objectif, column) );

//    sort(solutions, p->columns);
    printf("\n\tLes solutions sont :\n");
    for(int i = 0; i < p->columns; i++) {
        printf("\t\tx%d = %.2f\n", solutions[i].column+1, (solutions[i].row < 0) ? 0.0f : p->b[solutions[i].row]);
    }

    free(solutions);
    solutions = NULL;

    //Il y a de fortes chances pour que le programme se termine après la fin de cette fonction,
    //on met donc à jour les valeurs de internes de columns et rows pour que le
    //nettoyage du pl dans la mémoire soit fait correctement.
    p->columns = column;
    p->rows = row;

}

void transformation_avant_dual_simplexe(Programme_Lineaire *p) {
    for(int i = 0; i < p->rows; i++) {
        if(p->contraintes[i].type == '>') {
            for(int j = 0; i < p->columns; j++)
                p->contraintes[i].coeffs[j] *= -1;
        }
        else if(p->contraintes[i].type == '=') {


        }
    }

}


int indice_variable_entrante_dual_du_simplexe(const Programme_Lineaire *p, const int out_variable_index) {
    int index = 0;
    float cj, arj, ratio, min_ratio = -1.0f;
    for(int i = 0; i < p->columns; i++) {
        cj = p->objectif[i];
        //si ce coefficient est 0, alors il appartient à une variable qui est dans la base.
        if(cj == 0) continue;

        arj = p->contraintes[out_variable_index].coeffs[i];
        if(arj >= 0) continue;

        ratio = fabs(cj / arj);
        if(min_ratio < 0 || ratio < min_ratio) {
            min_ratio = ratio;
            index = i;
        }
    }

//    for(int i = 0; i < p->columns; i++) {
//        cj = p->objectif[i];
//        arj = p->contraintes[out_variable_index].coeffs[i];
//        ratio = fabs(cj / arj);
//
//        if(cj != 0 && arj < 0 && (min_ratio < 0 || ratio < min_ratio)) {
//            min_ratio = ratio;
//            index = i;
//        }
//    }

    return index;
}

//int indice_variable_entrante_dual_du_simplexe(const float *z, const int row, const int column, const float contraintes[row][column], const int out_variable_index) {
//    int index = 0;
//    float cj, arj, ratio, min_ratio = -1.0f;
//    for(int i = 0; i < column; i++) {
//        cj = z[i];
//        //si ce coefficient est 0, alors il appartient à une variable qui est dans la base.
//        if(cj == 0) continue;
//
//        arj = contraintes[out_variable_index][i];
//        if(arj >= 0) continue;
//
//        ratio = fabs(cj / arj);
//        if(min_ratio < 0 || ratio < min_ratio) {
//            min_ratio = ratio;
//            index = i;
//        }
//    }
//
////    for(int i = 0; i < column; i++) {
////        cj = z[i];
////        arj = contraintes[out_variable_index][i];
////        ratio = fabs(cj / arj);
////
////        if(cj != 0 && arj < 0 && (min_ratio < 0 || ratio < min_ratio)) {
////            min_ratio = ratio;
////            index = i;
////        }
////    }
//
//    return index;
//}

//void methode_duale_du_simplexe() {
//    int row = 2, column = 5;
//    float z[5] = {3.0f, 4.0f, 5.0f, 0.0f, 0.0f};
//
//    float contraintes[2][5] = {
//        {-1.0f, -2.0f, -3.0f, 1.0f, 0.0f},
//        {-2.0f, -2.0f, -1.0f, 0.0f, 1.0f}
//    };
//
//    float b[2] = {-5.0f, -6.0f};
//
//    //le nombre de x que l'on souhaite est égal au nombre de variable de décisions du problème donc : p->columns au lieu de column.
//    X *solutions = malloc(3 * sizeof(X));
//    if(!solutions) {
//        printf("Error: Couldn't create the array of solutions!\n");
//        return;
//    }
//
//    for(int i = 0; i < row; i++) {
//        solutions[i].row = solutions[i].column = -1;
//    }
//
//    int iteration = 1;
//    do{
//        printf("\n\tIteration %d :\n", iteration);
//
//        int out_variable_index = min(b, row);
//        int in_variable_index = indice_variable_entrante_dual_du_simplexe(z, row, column, contraintes, out_variable_index);
//
////        changement_de_base(p, solutions, in_variable_index, out_variable_index, row, column);
//
//        printf("\t\tin column = %d, out row = %d\n", in_variable_index+1, out_variable_index+1);
//
//        //Si l'indice de la variable entrante est < au nombre de variables de décisions,
//        //alors c'est une des variables de décisions qui entre dans la base.
//        if(in_variable_index < 3) {
//            //la longueur de 'solutions' est p->columns, or
//            // 0 <= out_variable_index <= p->columns + p->rows.
//            //Pour un PL contenant donc plus de contraintes que de variables de décisions,
//            //on ne peut pas utiliser directement out_variable_index,
//            //on risquerait d'accéder à une zone qui n'est pas dans le tableau.
//            int pos = out_variable_index >= 3 ? (out_variable_index+1) % 3 : out_variable_index;
//            solutions[pos] = (X) {out_variable_index, in_variable_index};
//        }
//
//
//        float pivot = contraintes[out_variable_index][in_variable_index];
//        printf("\t\tpivot = %.2f\n", pivot);
//
//        b[out_variable_index] /= pivot;
//        float column_coeff = z[in_variable_index];
//
//        for(int i = 0; i < column; ++i) {
//            contraintes[out_variable_index][i] /= pivot;
//            z[i] -= column_coeff * contraintes[out_variable_index][i];
//        }
//
//        printf("\t\tp-row devient : ");
//        for(int i = 0; i < column; ++i) printf("%5.2f, ", contraintes[out_variable_index][i]);
//        printf("\n");
//        printf("\t\tz-row devient : ");
//        for(int i = 0; i < column; ++i)  printf("%5.2f, ", z[i]);
//        printf("\n");
//
//
//        for(int i = 0; i < row; ++i) {
//            if(i == out_variable_index) continue;
//
//            column_coeff = contraintes[i][in_variable_index];
//            b[i] -= column_coeff * b[out_variable_index];
//            for(int j = 0; j < column; ++j)
//                contraintes[i][j] -= column_coeff * contraintes[out_variable_index][j];
//        }
//
//        printf("\t\tles contraintes deviennent :\n");
//        for(int i = 0; i < row; ++i) {
//            if(i == out_variable_index) continue;
//
//            printf("\t\t\t\t");
//            for(int j = 0; j < column; ++j) printf("%5.2f, ", contraintes[i][j]);
//            printf("\n");
//        }
//
//        printf("\t\tb-row devient : ");
//        for(int i = 0; i < row; ++i)    printf("%5.2f, ", b[i]);
//        printf("\n");
//
//        ++iteration;
//    }while( !condition_arret(b, 2) );
//
////    sort(solutions, 3);
//    printf("\n\tLes solutions sont :\n");
//    for(int i = 0; i < 3; i++) {
//        printf("\t\tx%d = %.2f\n", solutions[i].column+1, (solutions[i].row < 0) ? 0.0f : b[solutions[i].row]);
//    }
//
//    free(solutions);
//    solutions = NULL;
//}

//void methode_duale_du_simplexe(Programme_Lineaire *p) {
////    int rows = 2, column = 5;
//
//    transformation_avant_dual_simplexe(p);

//    //le nombre de x que l'on souhaite est égal au nombre de variable de décisions du problème donc : p->columns au lieu de column.
//    X *solutions = malloc(p->columns * sizeof(X));
//    if(!solutions) {
//        printf("Error: Couldn't create the array of solutions!\n");
//        return;
//    }
//
//    for(int i = 0; i < p->columns; i++) {
//        solutions[i].row = solutions[i].column = -1;
//    }
//
//    int iteration = 1;
//    do{
//        int out_variable_index = min(p->b, rows);
//        int in_variable_index = indice_variable_entrante_dual_du_simplexe(p, out_variable_index);
//
//        changement_de_base(p, solutions, in_variable_index, out_variable_index, row, column);
//
//        ++iteration;
//    }while( !condition_arret(b, 2) );
//
//    printf("\n\tLes solutions sont :\n");
//    for(int i = 0; i < p->columns; i++) {
//        printf("\t\tx%d = %.2f\n", solutions[i].column+1, (solutions[i].row < 0) ? 0.0f : p->b[solutions[i].row]);
//    }
//
//    free(solutions);
//    solutions = NULL;
//
//}
