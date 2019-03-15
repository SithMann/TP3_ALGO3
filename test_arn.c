#include "arn.h"
#include <stdio.h>



int main(int argc, char**argv){

	// Test creation et ajout
	
	t_arn a = arn_creer(5, NULL);
	afficher_arbre(a, INFIXE_INVERSE);
	a = arn_ajouter(a, 4); 
	afficher_arbre(a, INFIXE_INVERSE);
	a = arn_ajouter(a, 7);
	afficher_arbre(a, INFIXE_INVERSE);
	a = arn_ajouter(a, 1);
	afficher_arbre(a, INFIXE_INVERSE);
	a = arn_ajouter(a, 27);
	afficher_arbre(a, INFIXE_INVERSE);
	a = arn_ajouter(a, 3);
	afficher_arbre(a, INFIXE_INVERSE);


	printf("rotation droite : \n");
	rotation_gauche(a);
	afficher_arbre(a->pere, INFIXE_INVERSE);

	// Test suppression
/*
	arn_supprimer_val(a, 7);
	afficher_arbre(a, INFIXE_INVERSE);
	arn_supprimer_val(a, 27);
	afficher_arbre(a, INFIXE_INVERSE);
*/

	return 0;
}

