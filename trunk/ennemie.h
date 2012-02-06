#ifndef DEF_ENNEMIE
#define DEF_ENNEMIE
#include "constantes.h"
#include "deplace_mario.h"
void initEnnemi(Map carte, Ennemi *enne,int i,SDL_Surface* screen);
void deplace_ennemi(Ennemi *ennemi1,Map carte,SDL_Surface* screen,Perso perso1);
void ennemi_tire(Ennemi *ennemie1,Perso perso1,Map carte,SDL_Surface *screen);
void affiche_ennemi(Ennemi ennemi1,SDL_Surface* screen,Map carte);
void test_collision_ennemi(Ennemi **ennemi1,Perso *perso1,Balle **mario_tire,int *nbr_balle,Map *carte,int *a_atteri,int *continuer,Joueur *joueur,SDL_Surface *screen);
int collision_bloque(SDL_Rect bloc1,SDL_Rect bloc2);
#endif

