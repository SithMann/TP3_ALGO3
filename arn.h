#ifndef __ARN_H__
#define __ARN_H__

#include<stdio.h>
#include<stdlib.h>

typedef enum {ROUGE='R', NOIR='N', DOUBLE_NOIR='D', EXTERNE='E'} t_coul;

typedef struct noeud {
	int val; 
	t_coul coul;
	struct noeud *pere, *sag, *sad;
} t_noeud;

typedef struct noeud* t_arn;

typedef enum type_parcours { PREFIXE, INFIXE, POSTFIXE, INFIXE_INVERSE } t_type_parcours;


/****************************************
 * primitives de modification de la structure de l'arbre 
 ****************************************/
t_arn arn_creer(int val, t_arn pere); /* renvoie l'arbre dont la racine vaut val */
static int arn_supprimer_feuille(t_arn);

static int post_ajout(t_arn, t_arn);
t_arn arn_ajouter(t_arn, int); 

static int post_supprimer(t_arn);
static t_arn arn_supprimer(t_arn);
int arn_supprimer_val(t_arn a, int v);

int rotation_gauche(t_arn a);
int rotation_droite(t_arn a);


/****************************************
 * primitives d'accès à la structure de l'arbre 
 ****************************************/
t_arn pere(t_arn); /* renvoie le pere s'il existe, NULL sinon */
t_arn sag(t_arn); /* renvoie le sous-arbre gauche s'il existe, NULL sinon */
t_arn sad(t_arn); /* renvoie le sous arbre droit s'il existe, NULL sinon */
t_coul couleur(t_arn); /* renvoie la couleur de la racine de l'arbre */
t_arn frere(t_arn); /* renvoie le frere de l'arbre */
t_arn racine(t_arn); /* renvoie la racine de l'arbre */

/****************************************
 * primitives de vérification  
 ****************************************/
int arn_vide(t_arn);			/* renvoie vrai si arbre vide, faux sinon */
int arn_est_racine(t_arn); /*renvoie vrai si le noeud est une racine */
int arn_est_feuille(t_arn); /*renvoie vrai si le noeud est une feuille */

/****************************************
 * primitives de modification de la valeur de la racine
 ****************************************/
int arn_modif_val(t_arn, int);	/* modifie la valeur de la racine de l'arbre */
int arn_val_racine(t_arn, int*); 		/* renvoie la valeur de la racine de l'arbre */
int arn_modif_pere(t_arn, t_arn); /* modifie le pere de l'arbre */
int arn_modif_sag(t_arn, t_arn); /* modifie le sag de l'arbre */
int arn_modif_sad(t_arn, t_arn); /* modifie le sad de l'arbre */
int arn_modif_couleur(t_arn, t_coul); /* modifie la couleur de l'arbre */

/****************************************
 * primitives d'affichage 
 ****************************************/
int afficher_arbre(t_arn, t_type_parcours);
int afficher_arbre_prefixe(t_arn, int);
int afficher_arbre_infixe(t_arn, int );
int afficher_arbre_infixe_inverse(t_arn, int);
int afficher_arbre_postfixe(t_arn, int);

void afficher_ancetres(t_arn a, char end);

int parcours_prefixe(t_arn, void (*)(int*));

t_arn arn_max(t_arn);
t_arn arn_min(t_arn);
static int arn_ajouter_couleur_noir(t_arn);

#endif

