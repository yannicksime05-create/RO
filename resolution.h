#ifndef RESOLUTION_H
#define RESOLUTION_H

#include "pl.h"

/**
*   @brief Cette structure permet de représenter un point dans un repère orthonormé (O, x, y).
*   Elle est notamment destinée à être utilisée lorsque l'utilisateur choisi l'option graphique pour résoudre son programme.
*   Ses champs sont de type entier car l'algorithme qui donne les points essayera de toujours renvoyer des valeurs de ce type.
*/
typedef struct Point {
    /**
    *   L'abscisse du point.
    */
    int abs;
    /**
    *   L'ordonnée du point.
    */
    int ord;
} Point;

typedef struct X {
    /**
    *   L'indice auquel se trouve la solution dans le vecteur b.
    */
    int row;

    /**
    *   L'indice de la variable dont on veut la solution.
    */
    int column;

//    float value;
} X;

void methode_graphique(const Programme_Lineaire *p);
void transformation_avant_simplexe(Programme_Lineaire *p);
void methode_du_simplexe(Programme_Lineaire *p);

void methode_revisee_du_simplexe(Programme_Lineaire *p);

void transformation_avant_dual_simplexe(Programme_Lineaire *p);
void methode_duale_du_simplexe(Programme_Lineaire *p);

#endif // RESOLUTION_H
