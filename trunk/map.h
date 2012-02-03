#ifndef DEF_MAP
#define DEF_MAP
#include "constantes.h"

//Enumération des different bloc possible
void initMap(Map *carteJeu);
//Fonction qui permet de chager la map du fichier vers le tableau
//Recupère la structure map
void chargeMap(Map *carte);
void sauvegardeMap(Map carte);
//Fonctio qui permet d'afficher les tile en fonction du tableau
//Recupère la structure map
void afficheMap(Map carte, SDL_Surface *ecran, ParamMap opt);
int scrollingPersoX(Perso mario, Map *carte, SDL_Surface* screen);
int scrollingPersoY(Perso mario, Map *carte, SDL_Surface* screen);
void ScrollingDebordement(Map *carte, SDL_Surface *screen);
/*
Fonction : Permet de lister les niveau qui sont dans niveau/gestionNiveau
*/
void listeDesNiveau(Map *carte);


#endif






