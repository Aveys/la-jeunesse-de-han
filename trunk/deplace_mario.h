#ifndef DEF_DEPLACE_MARIO
#define DEF_DEPLACE_MARIO
#include "constantes.h"
#include "map.h"
#include <fmodex/fmod.h>

void decoupeBlaster(Perso *player);
void decoupePerso(Perso *perso1);
void affiche_laser(Balle **tire_mario,int *nb_balle,Map *carte,SDL_Surface *screen, Perso player,Joueur *joueur1);
void affiche_perso(Perso *perso1,SDL_Surface* screen,int a_atteri);
void gestion_touche(Perso *perso1, Map *carte, int *continuer,int *a_atteri,int *touche_enfonce,int *touche_a_ete_enfonce,int *new_balle,int *cpt_balle,int *jetPack,int *cptJetPack,SDL_Joystick *joystick,int *joysticktouche,Balle **tire_mario,int *nb_balle);
int deplace_mario(Perso *perso1,Map *carte,int decalage,SDL_Surface* screen,int *a_atteri,Joueur *j,int *continuer,FMOD_SYSTEM *system,FMOD_SOUND *piece,Score *s,Ennemi *tabEnnemi,int *reiinit_ennemi);
void position_perso_tab(SDL_Rect surface_a_tester,int *posxmin,int *posymin,int *posxmax,int *posymax,Map carte,int opt);
int test_collision(SDL_Rect surface_a_tester,Map *carte,int opt,int opt2,Joueur *joueur1,Perso *perso1,int no_perso);
void pause();
int CollisionDroiteSeg(Point A,Point B,Point O,Point P);
int CollisionSegSeg(Point A,Point B,Point O,Point P);
int Collision(Perso *perso1,Point P,int xscroll,int yscroll);
#endif
