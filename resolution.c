#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "resolution.h"

/**
*   \brief Cette fonction détermine et indique à l'utilisateur les points à tracer pour pouvoir résoudre son PL graphiquement.
*   Cette fonction n'est donc dès lors qu'à utiliser lorsque le programme linéaire de l'utilisateur est de degré 2.
*
*   Soit à déterminer les points permettant de tracer la contrainte : c1 * x1 + c2 * x2 = b
*
*   Pour ce faire, il faut :
*       - (dans un premier temps) remarquer que chaque coefficient (ci) de la contrainte permettra d'obtenir
*           l'abscisse et l'ordonnée d'un point du plan (c'est plutôt évident);
*
*       - et enfin, constater que quelque soit les circonstances (ci est un diviseur de b ou pas), les
*           coordonnées des points se trouvent à partir de la formule :      b = q * ci + r.
*
*       Explication:
*           Lorsqu'on essaie de déteminer x1 et x2 respectant la contrainte, le moyen le plus rapide est encore de mettre l'une
*           des deux variables à 0 et déterminer la valeur de l'autre en faisant b / ci, ce qui permet d'obtenir les points
*               - (0, b / ci) lorsque x1 = 0
*               - (b / ci, 0) lorsque x2 = 0
*
*           sauf que cette méthode coince si ci n'est pas un diviseur de b (notre but est d'obtenir des valeurs entières pour les coordonnées).
*           Ex:
*               x1 + 2 * x2 = 5. En appliquant la méthode précédente, on obtient les points :
*               - (0, 5 / 2) et
*               - (5, 0)
*
*               Le second point est bon (facilement représentable) mais pas le premier. À la place, on voudrait le point (1, 2) par exemple.
*
*           Pour résoudre ce 'problème', il suffit (encore une fois) d'appliquer la formule b = q * ci + r et de créer les points :
*               - (q, r) lorsque i = 1
*               - (r, q) lorsque i = 2
*           Ex:
*               x1 + 2 * x2 = 5 (l'exemple précédent).
*               - i = 1 => ci = 1,
*                   on a : 5 = 5 * 1 + 0        ce qui nous donne le point (5, 0)
*
*               - i = 2 => ci = 2
*                   on a : 5 = 2 * 2 + 1        ce qui nous donne le point (1, 2)
*
*               Le point (0, 5 / 2) de l'approche précédente est remplacé par (1, 2) qui est bien plus facile à représenter dans un plan.
*
*   Cette solution en plus d'être simple résoud donc bien notre 'problème' :)
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
*   \param *p       L'adresse du programme linéaire afin de pouvoir accéder à toutes ses données.
*/
void methode_graphique(Programme_Lineaire *p) {
    Point p1 = {0}, p2 = {0};

    for(int i = 0; i < p->rows; i++) {
        for(int j = 0; j < p->columns; j++) {
            /*
            *   j == 0, (coeffs[j] est le coefficient de x1).
            */
            if(j == 0) {
                //FIXME!!! -x1 + x2 = 1     (1, 2) (0, 1)
                p1.abs = (int) floor(p->b[i] / p->contraintes[i].coeffs[j]);
                p1.ord = ((int) p->b[i]) % ((int) p->contraintes[i].coeffs[j]);
            }
            /*
            *   j == 1, (coeffs[j] est le coefficient de x2).
            */
            else {
                p2.ord = (int) floor(p->b[i] / p->contraintes[i].coeffs[j]);
                p2.abs = ((int) p->b[i]) % ((int) p->contraintes[i].coeffs[j]);
            }
        }

        //À ce niveau on connait déjà tous les points utiles pour tracer la contrainte. Il ne reste donc plus qu'à les afficher.
        printf("Pour la contrainte %d, tracez les points :\n", i+1);
        printf("\t\tx1 = %d, x2 = %d\n", p1.abs, p1.ord);
        printf("\t\tx1 = %d, x2 = %d\n", p2.abs, p2.ord);
    }
}


void sort(X *solutions, int size) {
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
int min(float *b, int size) {
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

void transformation_avant_simplexe(Programme_Lineaire *p) {
    if(p->type == 'M') {
        for(int i = 0; i < p->columns; i++)
            p->objectif[i] *= -1;

        return;
    }
}

bool condition_arret_simplexe(float *z, int size) {
    for(int i = 0; i < size; i++) {
        if(z[i] < 0) return false;
    }

    return true;
}

//int indice_variable_sortante_simplexe(int row, int column, float *b, float contraintes[row][column], int in_variable_index) {
//    float ratio = 0.0f, min_ratio = -1.0f;
//    int index = 0;
//
////    for(int i = 0; i < row; ++i) {
////        ratio = b[i] / contraintes[i][in_variable_index];
////
////        if(ratio >= 0) {
////            if(min_ratio < 0 || ratio < min_ratio) {
////                min_ratio = ratio;
////                index = i;
////            }
////        }
////    }
//
//    for(int i = 0; i < row; ++i) {
//        ratio = b[i] / contraintes[i][in_variable_index];
//        if( ratio >= 0 && (min_ratio < 0 || ratio < min_ratio) ) {
//            min_ratio = ratio;
//            index = i;
//        }
//    }
//
//    return index;
//}
//
//void methode_du_simplexe() {
////    int row = 3, column = 5;
////    float z[5] = {-10.0f, -9.0f, 0.0f, 0.0f, 0.0f};
////    float contraintes[3][5] = {
////        {5.0f, 4.0f, 1.0f, 0.0f, 0.0f},
////        {4.0f, 6.0f, 0.0f, 1.0f, 0.0f},
////        {2.0f, 1.0f, 0.0f, 0.0f, 1.0f}
////    };
////    float b[3] = {200.0f, 230.0f, 70.0f};
////    int max_iteration = 4;
//    X solutions[2] = {0};
//
//    int row = 4, column = 6;
//    float z[6] = {-5.0f, -4.0f, 0.0f, 0.0f, 0.0f, 0.0f};
//    float contraintes[4][6] = {
//        {6.0f, 4.0f, 1.0f, 0.0f, 0.0f, 0.0f},
//        {1.0f, 2.0f, 0.0f, 1.0f, 0.0f, 0.0f},
//        {-1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f},
//        {0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f}
//    };
//    float b[4] = {24.0f, 6.0f, 1.0f, 2.0f};
////    int max_iteration = 3;
//
//
//    int iteration = 1;
//    do{
//        printf("\n\tIteration %d :\n", iteration);
//
//        int in_variable_index = min(z, column) % column;
//        int out_variable_index = indice_variable_sortante_simplexe(row, column, b, contraintes, in_variable_index);
//        printf("\t\tin column = %d, out row = %d\n", in_variable_index+1, out_variable_index+1);
//
//        if(in_variable_index <= column - row)
//            solutions[iteration-1] = (X) {out_variable_index, in_variable_index, b[out_variable_index]};
//
//        float pivot = contraintes[out_variable_index][in_variable_index];
//        printf("\t\tpivot = %.2f\n", pivot);
//
//        b[out_variable_index] /= pivot;
//        float colum_coeff = z[in_variable_index];
//
//        for(int i = 0; i < column; ++i) {
//            contraintes[out_variable_index][i] /= pivot;
//            z[i] -= colum_coeff * contraintes[out_variable_index][i];
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
//            colum_coeff = contraintes[i][in_variable_index];
//            b[i] -= colum_coeff * b[out_variable_index];
//            for(int j = 0; j < column; ++j)
//                contraintes[i][j] -= colum_coeff * contraintes[out_variable_index][j];
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
//    }while( !condition_arret_simplexe(z, column) );
////    }while(iteration != max_iteration);
//
//    printf("\n\tLes solutions sont :\n");
//    for(int i = 0; i < 2; i++) {
//        printf("\t\tx%d = %.2f\n", solutions[i].column+1, solutions[i].value);
//    }
//
//}

int indice_variable_sortante_simplexe(float *b, Contrainte *contraintes, int row, int in_variable_index) {
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

void methode_du_simplexe(Programme_Lineaire *p) {
    int row = p->rows, column = p->columns + p->rows;

    p->objectif = (float *) realloc(p->objectif, column);
    for(int i = p->columns; i < column; ++i) p->objectif[i] = 0.0f;
    float *z = p->objectif;

    for(int i = 0; i < p->rows; ++i) {
        p->contraintes[i].coeffs = (float *) realloc(p->contraintes[i].coeffs, column);

        for(int j = p->columns; j < column; ++j) {
            p->contraintes[i].coeffs[j] = (j == i+p->columns) ? 1.0f : 0.0f;
        }
    }
    Contrainte *contraintes = p->contraintes;

    float *b = p->b;

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

        int in_variable_index = min(z, column) % column;
        int out_variable_index = indice_variable_sortante_simplexe(b, contraintes, row, in_variable_index);
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
        float colum_coeff = z[in_variable_index];

        for(int i = 0; i < column; ++i) {
            contraintes[out_variable_index].coeffs[i] /= pivot;
            z[i] -= colum_coeff * contraintes[out_variable_index].coeffs[i];
        }

        printf("\t\tp-row devient : ");
        for(int i = 0; i < column; ++i) printf("%5.2f, ", contraintes[out_variable_index].coeffs[i]);
        printf("\n");
        printf("\t\tz-row devient : ");
        for(int i = 0; i < column; ++i)  printf("%5.2f, ", z[i]);
        printf("\n");


        for(int i = 0; i < row; ++i) {
            if(i == out_variable_index) continue;

            colum_coeff = contraintes[i].coeffs[in_variable_index];
            b[i] -= colum_coeff * b[out_variable_index];
            for(int j = 0; j < column; ++j)
                contraintes[i].coeffs[j] -= colum_coeff * contraintes[out_variable_index].coeffs[j];
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

        ++iteration;
    }while( !condition_arret_simplexe(z, column) );

    sort(solutions, p->columns);
    printf("\n\tLes solutions sont :\n");
    for(int i = 0; i < p->columns; i++) {
        printf("\t\tx%d = %.2f\n", solutions[i].column+1, (solutions[i].row < 0) ? 0.0f : b[solutions[i].row]);
    }

    z = NULL;
    contraintes = NULL;
    b = NULL;

    free(solutions);
    solutions = NULL;

    //Il y a de fortes chances pour que le programme se termine après la fin de cette fonction,
    //on met donc à jour les valeurs de internes de columns et rows pour que le
    //nettoyage du pl dans la mémoire soit fait correctement.
    p->columns = column;
    p->rows = row;

}


//void variable_entrante_et_sortante_dual_du_simplexe(const Programme_Lineaire *p, int *in, int *out) {
//    *out = min(p->b, p->rows);
//
//
//
//    for(int i = 0; i < p->columns; ++i) {
//                old_ratio = 0.0f;
//        float cj = p->objectif[i], arj = p->contraintes[*out].coeffs[i];
//        if(arj < 0) {
//            old_ratio = fabs(cj / arj);
//        }
//    }
//}
//
//void methode_duale_du_simplexe(Programme_Lineaire *p) {
//    int rows = 2, column = 5;
//    float z[column] = {3, 4, 5, 0, 0};
////    float s1[n] = {-1, -2, -3, 1, 0};
////    float s2[n] = {-2, -2, -1, 0, 1};
//
//    float contraintes[rows][column] = {
//        {-1, -2, -3, 1, 0},
//        {-2, -2, -1, 0, 1}
//    };
//
//    float b[rows] = {-5, -6};
//
//    int out_variable_index = min(b, rows), in_variable_index;
//
//    float cj, arj, old_ratio = 0.0f, new_ratio = 0.0f;
//    bool skip_this_iteration = true;
//    for(int i = 0; i < column; ++i) {
//        cj = z[i];
//        arj = contraintes[out_variable_index][i];
//
//        //cj != 0 && arj < 0
//        if(cj && arj < 0) {
//            old_ratio = new_ratio;
//            new_ratio = fabs(cj / arj);
//            if(!skip_this_iteration) {
//                if(new_ratio < old_ratio) in_variable_index = i;
//            }
//
//            if(i == 0) skip_this_iteration = false;
//        }
//    }
//
//
//
//
//
////    if(p->type == 'M') {
////        for(int i = 0; i < p->columns; ++i)
////            p->objectif[i] *= -1;
////    }
////
////
////    for(int i = 0; i < p->rows; ++i) {
////        if(p->contraintes[i].type == '>') {
////            for(int j = 0; j < p->columns; ++j) {
////                p->contraintes[i].coeffs[j] *= -1;
////                p->b[j] *= -1;
////            }
////        }
////
////    }
////
////    float *tmp = p->objectif;
////    int old_column = p->columns, new_column = p->columns + p->rows;
////    p->objectif = (float *) calloc(new_column, sizeof(float));
////    p->columns = new_column;
////    for(int i = 0; i < old_column; ++i) {
////        p->objectif[i] = tmp[i];
////    }
////    free(tmp);
////
////    for(int i = 0; i < p->rows; ++i) {
////        tmp = p->contraintes[i].coeffs;
////        p->contraintes[i].coeffs = (float *) calloc(new_column, sizeof(float));
////        for(int j = 0; j < old_column; ++j)
////            p->contraintes[i].coeffs[j] = tmp[j];
////
////        free(tmp);
////        p->contraintes[i].coeffs[i + old_column] = 1;
////    }
////
////
////
////    int in, out;
////    variable_entrante_et_sortante_dual_du_simplexe(p, &in, &out);
//
//
//
//
//
//
//
//
//
//}
