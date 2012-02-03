//*******************************************
//Auteur : Mathieu MARTIN
//Desription : Outils pour afficher/editer les map
//Date : 24/01/2012
//*******************************************
#ifndef DEF_EDITEUR
#define DEF_EDITEUR
#include "constantes.h"
#include "map.h"
/*
Fonction : Permet de lancer editeur
Param�tre : -La SDL_Surface de ecran principal
            -La structure de la map
            -La structure des options
*/
void editeur(SDL_Surface *ecran, Map *carte, ParamMap *opt,SDL_Joystick *joystick);
/*
Fonction : Permet d'envoyer et de r�cup�rer des information pour la gestion des touches dans editeur
Param�tre : - La structure de la map
            - La structure ParamMap
            - La SDL_Surface principal de ecran
            - Le pointeur pour continuer ou quitter la boucle
            - Le pointeur pour les information de la souris en X
            - Le pointeur pour les information de la souris en Y
            - Le pointeur pour r�cup�rer l'appuie sur le clic gauche
            - Le pointeur pour r�cup�rer l'appuie sur le clic droite
            - Le pointeur pour r�cup�rer le type de bloc utilis�e
*/
void gestionToucheEditeur(Map *carte, ParamMap *opt, SDL_Surface *ecran,  int *continuer, int *mouseX, int *mouseY, int *clicGauche, int *clicDroite, int *typeBloc,SDL_Joystick *joystick);
/*
Fonction : Permet d'afficher le bloc actuellement selection, il suit le curseur de la souris
Param�tre : - La SDL_Surface de ecran principal
            - La structure de la map
            - Le pointeur de la souris en X
            - Le pointeur de la souris en Y
            - Le pointeur du type de bloc
*/
void aideBloc(SDL_Surface *screen, Map carte, int *mX, int *mY, int *typeBloc);
/*
Fonction : Permet d'afficher les information de interface
Param�tre : - La SDL_Surface de l'affichage principal
*/
void afficheInterface(SDL_Surface *screen, Map carte);
/*
Fonction : Affiche une boite de dialogue
Param�tre : - La texte � afficher
            - Le titre de la msgbox
*/
int SDLMessageBox(char *str, char *titre);
void creationNiveau(Map *carte);
/*
Fonction : Permet d'afficher en texte la version du jeu actuel
*/
void versionTexte();
#endif

