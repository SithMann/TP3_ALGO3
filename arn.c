/* Mise en oeuvre par pointeurs d'un arbre rouge noir */

#include "arn.h"



/****************************************
 * primitives de modification de la structure de l'arbre 
 ****************************************/
/* renvoie un arbre dont la racine vaut val, le pere vaut p */
t_arn arn_creer(int val, t_arn pere){
	t_arn a = malloc(sizeof(t_noeud));
	a->val = val;
	a->coul = ROUGE;
	a->pere = pere;
	a->sag = NULL;
	a->sad = NULL;
	return a;
}


static int arn_supprimer_feuille(t_arn s){
	if(arn_vide(s) || !arn_est_feuille(s))
		return 1;
	
	if(sag(pere(s)) == s){
		arn_modif_sag(pere(s), NULL);
	} else {
		arn_modif_sad(pere(s), NULL);
	}
	free(s);
	return 0;
}

/* Effectue la rotation gauche de l'arbre a (si possible) */
int rotation_gauche(t_arn a){
	if(!arn_vide(a) && !arn_est_feuille(a)){
		t_arn b = sad(a);
		if(b){
			b->pere = pere(a);
			arn_modif_sad(a,sag(b));
			arn_modif_pere(sag(b),a);
			arn_modif_sag(b,a);
			arn_modif_pere(a,b);
		return 1;
		}
	}
	return 0;
}

/* Effectue la rotation droite de l'arbre a (si possible) */
int rotation_droite(t_arn a){
	if(!arn_vide(a) && !arn_est_feuille(a)){
		t_arn b = sag(a);
		if(b){
			arn_modif_pere(b,pere(a));
			arn_modif_sag(a, sad(b));
			arn_modif_pere(sad(b), a);
			arn_modif_sad(b,a);
			arn_modif_pere(a,b);
			return 1;
		}
	}
	return 0;
}

/* Effectue la rotation gauche - droite de l'arbre a (si possible) */
int double_rotation_gauche_droite(t_arn a){
	if(!arn_vide(a) && !arn_est_feuille(a)){
		rotation_gauche(sag(a));
		rotation_droite(a);
		return 1;
	}
	return 0;
}

/* Effectue la rotation droite - gauche de l'arbre a (si possible) */
int double_rotation_droite_gauche(t_arn a){
	if(!arn_vide(a) && !arn_est_feuille(a)){
		rotation_droite(sad(a));
		rotation_gauche(a);
		return 1;
	}
	return 0;
}

static int post_ajout(t_arn p, t_arn nouv){

	// Si on n'a pas un probleme de double ROUGE, alors rien à faire
	if(couleur(p) != ROUGE || couleur(nouv) != ROUGE)
		return 0;
	
	/* résolution du problème de double rouge */
	if(arn_est_racine(p)){ // cas 1
		fprintf(stderr, "cas 1\n");
		arn_modif_couleur(p,NOIR);
	} else if(couleur(frere(p)) == ROUGE){ // cas 2
		fprintf(stderr, "cas 2\n");
		arn_modif_couleur(p,NOIR);
		arn_modif_couleur(frere(p), NOIR);
		arn_modif_couleur(pere(p),ROUGE);
	}
	else if(couleur(frere(p)) == NOIR || couleur(frere(p)) == EXTERNE){ //cas 3
		if (nouv == sag(p)){ //cas3a
			t_arn gp = pere(p);
			if(p == sag(gp)){
				fprintf(stderr, "cas 3a\n");
				rotation_droite(gp);
				arn_modif_couleur(p,NOIR);
			} else {
				fprintf(stderr, "cas 3a symétrique\n");
				rotation_gauche(gp);
				arn_modif_couleur(p,NOIR);
			}
			//  gp devient rouge
			arn_modif_couleur(gp, ROUGE);
			if(couleur(pere(gp)) == ROUGE && couleur(gp) == ROUGE){
				fprintf(stderr, "cas 3a -> on remonte d'un cran\n");
				post_ajout(gp,p);
			}

		} else { //cas3b
			t_arn gp = pere(p);
			if(p == sag(gp)){
				fprintf(stderr, "cas 3b\n");
				double_rotation_gauche_droite(p);
				arn_modif_couleur(nouv, NOIR);
			} else {
				fprintf(stderr, "cas 3b symétrique\n");
				double_rotation_droite_gauche(p);
				arn_modif_couleur(nouv, NOIR);
			}
			// gp devient rouge
			arn_modif_couleur(gp, ROUGE);
			if(couleur(pere(gp)) == ROUGE && couleur(gp) == ROUGE) {
				fprintf(stderr, "cas 3b -> on remonte d'un cran\n");
				post_ajout(gp,p);
			}
		}
	} else {
		fprintf(stderr, "ERREUR post_ajout..\n");
		exit(1);
	}
	return 0;
}


t_arn arn_ajouter(t_arn a, int v){

	//fprintf(stderr, "### DEBUT AJOUTER %d\n", v);
	if(arn_vide(a))
		return NULL;
	t_arn nouv = NULL;

	while(nouv == NULL){
		if(v<a->val){
			if(sag(a))	a=sag(a);
			else {
				a->sag = nouv = arn_creer(v, a);
				post_ajout(a, nouv);
			}
		} else if(v>a->val){
			if(sad(a))	a=sad(a);
			else {
				a->sad = nouv = arn_creer(v, a);
				post_ajout(a, nouv);
			}
		} else {
			return a;
		} 
	}
	while(!arn_est_racine(a)){
		a=pere(a);
	}
	//fprintf(stderr, "### FIN AJOUTER %d\n", v);
	return a;
}

t_arn ajouter_rec(t_arn a, int v){

	//fprintf(stderr, "### DEBUT AJOUTER REC %d\n", v);
	if(arn_vide(a))
		return NULL;
	t_arn nouv = NULL;

	if(v<a->val){
		if(sag(a))	ajouter_rec(a->sag, v);
		else {
			a->sag = nouv = arn_creer(v, a);
			post_ajout(a, nouv);
		}
	} else if(v>a->val){
		if(sad(a))	ajouter_rec(a->sad, v);
		else {
			a->sad = nouv = arn_creer(v, a);
			post_ajout(a, nouv);
		}
	} 

	while(!arn_est_racine(a)){
		a=pere(a);
	}
	//fprintf(stderr, "### FIN AJOUTER REC %d\n", v);
	return a;
}

/* regle le cas du double noir */
static int post_supprimer(t_arn s){

	if(!arn_vide(s)  && couleur(s) != DOUBLE_NOIR){
		fprintf(stderr, "post_supprimer: pas de double noir ...\n");
		//exit(0);
		return 1;
	} else if(arn_vide(s)){
		fprintf(stderr, "post_supprimer: s est vide ...\n");
	}
	t_arn p = pere(s);
	while(couleur(s) == NOIR){

		if(arn_est_racine(s)){
			fprintf(stderr, "cas 1\n");
			arn_modif_couleur(s,NOIR);
		} else if( s == sag(pere(s)) ){
			t_arn f = frere(s);
			if(couleur(f) == ROUGE){
				fprintf(stderr, "cas 3\n");
				rotation_gauche(p);
				arn_modif_couleur(p,ROUGE);
				arn_modif_couleur(f,NOIR);
			}

			if(couleur(sag(f)) == NOIR && couleur(sad(f)) == NOIR){
				fprintf(stderr, "cas 2a\n");
				arn_modif_couleur(s,NOIR);
				arn_modif_couleur(f,ROUGE);
				couleur(p)==ROUGE?arn_modif_couleur(p,NOIR):arn_modif_couleur(p,DOUBLE_NOIR)
			} else {
				if(couleur(sad(f)) == NOIR){
					fprintf(stderr, "cas 2c\n");
					rotation_droite(f);
					arn_modif_couleur(f,ROUGE);
					arn_modif_couleur(g,NOIR);
				}
				fprintf(stderr, "cas 2b\n");
					rotation_gauche(f);
					arn_modif_couleur(f,ROUGE);
					arn_modif_couleur(g,NOIR);
				break;
			}
		} else { 
			t_arn f = frere(s);
			if(couleur(f) == ROUGE){
				fprintf(stderr, "cas 3 symétrique\n");
				// TODO
			}
			
			if(couleur(sag(f)) == NOIR && couleur(sad(f)) == NOIR){
				fprintf(stderr, "cas 2a symétrique\n");
				// TODO
			} else {
				if(couleur(sag(f)) == NOIR){
					fprintf(stderr, "cas 2c symétrique\n");
					// TODO
				}
				fprintf(stderr, "cas 2b symétrique\n");
				// TODO
				break;
			}
		}
	}
	return 0;
}
/* Retourne le noeud ayant la valeur suivante (soit dans les fils droits, soit dans le pere */
t_arn arn_successeur(t_arn a){

	if(arn_vide(a)) return NULL;
	if(!arn_vide(sad(a))) return arn_min(sad(a));
	t_arn b = pere(a);
	while(!arn_vide(b) && a == sad(b)){
		a = b;
		b = pere(b);
	}
	return b;
}



static t_arn arn_supprimer(t_arn s){
	t_arn y, x;
	if(arn_vide(sag(s)) || arn_vide(sad(s))){
		y = s;
		fprintf(stderr, " y est s -> %d\n", y->val);
	}
	else{
		y = arn_successeur(s);
		fprintf(stderr, " y est le successeur -> %d\n", y->val);
	}
	// y pointe sur le noeud à supprimer
	// c'est soit s soit le successeur de s 

	if(!arn_vide(sag(y))){
		x = sag(y);
		fprintf(stderr, " x est sag -> %d\n", x->val);
	} else { 
		x = sad(y);
		fprintf(stderr, " x est sad -> %d\n", x?x->val:-1);
	}
	// x pointe sur le sag ou sad de y

	// suppression de y
	fprintf(stderr, " suppression de -> %d [déconnexion] \n", y->val);
	arn_modif_pere(x, pere(y));
	if( y == sag(pere(y))) arn_modif_sag(pere(y), x);
	else arn_modif_sad(pere(y), x);

	// si le noeud à supprimer n'est pas le noeud lui même, on recopie sa valeur dans s
	if( y != s ) { 
		fprintf(stderr, " on ne supprime pas le noeud lui même -> %d \n", y->val);
		int val;
		arn_val_racine(y, &val);
		fprintf(stderr, " s=%d prend la valeur de y=%d \n", s->val, y->val);
		arn_modif_val(s, val);
	} 

	afficher_arbre(racine(s), INFIXE_INVERSE);

	// si le noeud supprimé est un noeud rouge, il n’y a rien à
	// faire, l’arbre conserve ses propriétés
	// en revanche si le nœud supprimé est noir, 
	// il faut reorganiser l’arbre
	if ( couleur(y) == NOIR ){
		//arn_modif_couleur(x, DOUBLE_NOIR);
		arn_ajouter_couleur_noir(x);
		post_supprimer( x );
	}
	return y;
}



int arn_supprimer_val(t_arn a, int v){
	//fprintf(stderr, "### DEBUT SUPPRIMER %d\n", v);
	if(arn_vide(a))
		return 1;
	t_arn s = a;
	int fini=0;
	// recherche du noeud contenant la valeur à supprimer
	while(!fini){
		if(v<s->val){
			if(sag(s))	s=sag(s);
			else {
				return 1;
			}
		} else if(v>s->val){
			if(sad(s))	s=sad(s);
			else {
				return 1;
			}
		} else { //on l'a trouvé
			arn_supprimer(s);
			fini = 1;
		}
	}
	//fprintf(stderr, "### FIN SUPPRIMER %d\n", v);
	return 0;
}

 
/****************************************
 * primitives d'accès à la structure de l'arbre 
 ****************************************/
/* renvoie le pere s'il existe, NULL sinon */
t_arn pere(t_arn a){
	if(arn_vide(a))
		return NULL;
	return a->pere;
}

/* renvoie le sous-arbre gauche s'il existe, NULL sinon */
t_arn sag(t_arn a){
	if(arn_vide(a))
		return NULL;
	return a->sag;
}

/* renvoie le sous arbre droit s'il existe, NULL sinon */
t_arn sad(t_arn a){
	if(arn_vide(a))
		return NULL;
	return a->sad;
}

t_coul couleur(t_arn a){
	if(a)
		return a->coul;
	return EXTERNE;
}

t_arn frere(t_arn a){ /* renvoie le frere de l'arbre */
	if(!arn_est_racine(a)){
		t_arn gp = pere(a);
		if(a = sag(gp))
			return sad(gp);
		else return sag(gp);
	}
	return NULL;
}

t_arn racine(t_arn a){ /* renvoie la racine de l'arbre */
	while(!arn_vide(pere(a)))
		a=pere(a);
	return a;
}
/****************************************
 * primitives de vérification  
 ****************************************/
/* renvoie vrai si arbre vide, faux sinon */
int arn_vide(t_arn a){
	return a == NULL;
}
/*renvoie vrai si l'arbre est une racine
 * c-a-d s'il n'a pas de pere */
int arn_est_racine(t_arn a){
	if(arn_vide(a))
		return 0;
	else if(arn_vide(pere(a)))
		return 1;
	return 0;
}

/*renvoie vrai si le noeud est une feuille */
int arn_est_feuille(t_arn a){
	if(arn_vide(a))
		return 0;
	else if(arn_vide(sag(a)) && arn_vide(sad(a)))
		return 1;
	return 0;
}

/****************************************
 * primitives de modification 
 ****************************************/
/* met la valeur de la racine de l'arbre dans v
 * renvoie vrai si affectation, faux sinon */
int arn_val_racine(t_arn a, int* v){
	if(arn_vide(a))
		return 0;
	*v = a->val;
	return 1;
}

/* modifie la valeur de la racine de l'arbre 
 * renvoie vrai si modif effectuée, faux sinon */
int arn_modif_val(t_arn a, int val){
	if(arn_vide(a))
		return 0;
	a->val = val;
	return 1;
}
int arn_modif_pere(t_arn a, t_arn p){
	if(arn_vide(a))
		return 0;
	a->pere = p;
	return 1;
}
int arn_modif_sad(t_arn a, t_arn sad){
	if(arn_vide(a))
		return 0;
	a->sad = sad;
	return 1;
}
int arn_modif_sag(t_arn a, t_arn sag){
	if(arn_vide(a))
		return 0;
	a->sag = sag;
	return 1;
}
int arn_modif_couleur(t_arn a, t_coul coul){
	if(arn_vide(a))
		return 0;
	fprintf(stderr, "Modif couleur %d -> %c\n", a->val, (char)coul);
	a->coul = coul;
	return 1;
}


/****************************************
 * primitives d'affichage 
 ****************************************/

void afficher_val(t_arn a, char end){
	int val;
	if(arn_val_racine(a, &val)){
		printf("%d[%c]%c", val, (char)couleur(a), end);
	}
}


int afficher_arbre(t_arn a, t_type_parcours type){
	int res = 0;
	if(arn_vide(a)){
		printf("L'arbre est vide\n");
	}
	else {
		if(type == PREFIXE)
			afficher_arbre_prefixe(a, 0);
		else if(type == INFIXE)
			afficher_arbre_infixe(a, 0);
		else if(type == INFIXE_INVERSE)
			afficher_arbre_infixe_inverse(a, 0);
		else if(type == POSTFIXE)
			afficher_arbre_postfixe(a, 0);
		else {
			printf("Mauvais type de parcours d'arbre\n");
			res = 1;
		}
	}
	printf("\n");
	return res;
}

int parcours_prefixe(t_arn a, void (*fonc)(int*)){
	if(arn_vide(a)){
		printf("L'arbre est vide\n");
	} else{
		fonc(&(a->val));
		if(sag(a))
			parcours_prefixe(sag(a), fonc);
		if(sad(a))
			parcours_prefixe(sad(a), fonc);
	}
	return 0;
}

int afficher_arbre_prefixe(t_arn a, int prof){
	int val, i;
	if(arn_vide(a)){
		printf("L'arbre est vide\n");
	} else{
		for(i=0; i<prof; i++) printf("\t");
		afficher_val(a, '\n');
		if(sag(a))
			afficher_arbre_prefixe(sag(a), prof+1);
		if(sad(a))
			afficher_arbre_prefixe(sad(a), prof+1);
	}
	return 0;
}

int afficher_arbre_infixe(t_arn a, int prof){
	int val, i;
	if(arn_vide(a)){
		printf("L'arbre est vide\n");
	} else{
		if(sag(a))
			afficher_arbre_infixe(sag(a), prof+1);
		for(i=0; i<prof; i++) printf("\t");
		afficher_val(a, '\n');
		if(sad(a))
			afficher_arbre_infixe(sad(a), prof+1);
	}
	return 0;
}
int afficher_arbre_infixe_inverse(t_arn a, int prof){
	int val, i;
	if(arn_vide(a)){
		printf("L'arbre est vide\n");
	} else{
		if(sad(a))
			afficher_arbre_infixe_inverse(sad(a), prof+1);
		for(i=0; i<prof; i++) printf("\t");
		afficher_val(a, '\n');
		if(sag(a))
			afficher_arbre_infixe_inverse(sag(a), prof+1);
	}
	return 0;
}

int afficher_arbre_postfixe(t_arn a, int prof){
	int val, i;
	if(arn_vide(a)){
		printf("L'arbre est vide\n");
	} else{
		if(sag(a))
			afficher_arbre_postfixe(sag(a), prof+1);
		if(sad(a))
			afficher_arbre_postfixe(sad(a), prof+1);
		for(i=0; i<prof; i++) printf("\t");
		afficher_val(a, '\n');
	}
	return 0;
}


void afficher_ancetres(t_arn a, char end){
	a = pere(a);
	while(!arn_vide(a)){
		afficher_val(a, ' ');
		a = pere(a);
	}
	printf("%c", end);
}

t_arn arn_max(t_arn a){
	if(arn_vide(a)) return NULL;
	while(sad(a))
		a=sad(a);
	return a;
}
t_arn arn_min(t_arn a){
	if(arn_vide(a)) return NULL;
	while(sad(a))
		a=sad(a);
	return a;
}

static int arn_ajouter_couleur_noir(t_arn a){
	if(arn_vide(a) || couleur(a)==DOUBLE_NOIR) return 1;
	if(couleur(a) == ROUGE) arn_modif_couleur(a, NOIR);
	else arn_modif_couleur(a, DOUBLE_NOIR);
	return 0;
}


