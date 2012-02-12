#ifndef DEF_JEU
#define DEF_JEU
#include <SDL_image.h>
#include "constantes.h"
#include "deplace_mario.h"
#include "map.h"
void associer_surface_pos(SDL_Surface *surface,SDL_Rect *position);
void initPosPerso(Perso *perso1,Map *carteJeu,SDL_Surface *screen);
void jeu(SDL_Surface* screen, Map carteJeu, ParamMap opt,SDL_Joystick *joystick);
void enregistrerJoueur(Joueur *j, SDL_Surface* screen,SDL_Joystick *joystick);
void nouvellePartie(SDL_Surface* screen, Joueur *j, Score *sc,SDL_Joystick *joystick);
void affichageFonduJeu(SDL_Surface *ecran, int temps, int *fonduOk, Map carte, ParamMap opt, SDL_Surface *fond);
#endif
