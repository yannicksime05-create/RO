#ifndef PL_H
#define PL_H

#include <stdio.h>

/**
*   @brief Cette structure permet de représenter une contrainte dans un programme linéaire.
*/
typedef struct Contrainte {
    /**
    *   Le tableau contenant les coefficients de la contrainte.
    *   La longueur de ce tableau correspond à l'attribut ::columns de la structure Programme_Lineaire.
    */
    float *coeffs;
    /**
    *   Le type de la contrainte : <, >, = .
    */
    char type;
} Contrainte;

/**
*   Cette structure permet de représenter un programme linéaire de la forme :
*       Max/min Z = c1 x u1 + c2 x u2 + ... + cn x un
*       s.à         a11 x u1 + a12 x u2 + ... + a1n x un < (>) b1
*                   ...
*                   an1 x u1 + an2 x u2 + ... + ann x un < (>) bn
*
*   Un exemple valant mille mots...
*       Max Z = 2x1 + 3x2
*       s.à     2x1 + x2 <= 4
*               x1 + 2x2 <= 5
*               x1, x2 >= 0
*
*   Les attributs de cette structure prendront, pour l'exemple précédent, les valeurs suivantes :
*       objectif = [2, 3]
*       contraintes = [ //Notez que la longueur du tableau des coeffs de la contraite est égale à columns.
*                       {[2, 1], '<'},      //1ère contrainte.
*                       {[1, 2], '<'}       //2ème contrainte.
*                     ]
*       b = [4, 5]
*       rows = 2
*       columns = 2
*       type = M (M pour maximisation et m pour minimisation).
*/
typedef struct Programme_Lineaire {
    /**
    *   Le tableau contenant les coefficients de la fonction objectif du problème à résoudre.
    *   Ce tableau a pour longueur ::columns.
    */
    float *objectif;
    /**
    *   Le tableau contenant les contraintes du problème.
    *   Ce tableau a pour longueur ::rows.
    */
    Contrainte *contraintes;
    /**
    *   Le tableau contenant le coté droit des contraintes.
    *   Ce tableau a pour longueur ::rows.
    */
    float *b;
    /**
    *   Le nombre de contraintes du problème à résoudre (le nombre de lignes de la matrice des contraintes).
    */
    int rows;
    /**
    *   Le nombre de variables de décisions du problème à résoudre (le nombre de colonnes de la matrice des contraintes).
    */
    int columns;
    /**
    *   Le type de problème à résoudre (minimisation / maximisation).
    */
    char type;
} Programme_Lineaire;

/** */
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

Programme_Lineaire *create();
Programme_Lineaire *pl_apartir_dun_fichier(const char *filename);

void gestion_du_pl(Programme_Lineaire *p);
void affichage_du_pl(const Programme_Lineaire *p, FILE *outputfile);
void save_to_file(const char *filename, const Programme_Lineaire *p, const X *solutions);
void clean(Programme_Lineaire *p);

#endif // PL_H
