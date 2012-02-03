#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED
/* *******************************************************************
* structure bouton,                                                  *
* contient les surfaces/police nécessaire à l'affichage des boutons. *
*********************************************************************/
/* Parametres
* pos : structure de position
* *texte : surface du texte
* *police : pointeur de police (fichier .ttf)
* titre : chaine contenant le texte du bouton
*/
typedef struct
{
    SDL_Rect pos;
    SDL_Surface *texte;
    TTF_Font *police;
    char titre[60];
} Bouton;

/*************************************************************************
* @name menu                                                              *
* affiche le menu et fais le lien entre les différentes fonctions du jeu *
* selon le choix du joueur                                               *
* renvoie 0 s'il n'y a pas eu de problème                                *
*************************************************************************/
int menu(SDL_Surface *screen,SDL_Joystick *joystick);

/* **************************************************************
* menuBouton                                                    *
* procédure d'affichage d'un bouton                             *
* le parametre bouton.pos.y doit etre renseigné avant son appel *
****************************************************************/
/* Parametres:
* Bouton *bouton : structure Bouton contenant surface et texte
* SDL_Surface *ecran : surface de la fenetre
* SDL_Color couleurTexte : énumération nécessaire à la couleur du texte
* FILE *fichier : fichier contenant les textes du menu
* bool actif : définit si le bouton est le bouton actif ou non
* int nb : indice du bouton (utile pour le texte)
*/
void menuBouton(Bouton * bouton, SDL_Surface *ecran, SDL_Color couleurTexte, FILE *fichier, bool actif, int nb);

/* *******************************************************************************************************
* lanceProgramme                                                                                         *
* fonction chargé de lancer la fonction associé au bouton actif dans le menu                             *
* renvoie un booléen qui vaut vrai si une fonction a été lancée et faux si aucune fonction n'est définie *
*********************************************************************************************************/
/* Parametres:
* int bActif : donne le numero du bouton actif
* SDL_Surface *ecran : surface de la fenetre
* Map *carteJeu : structure de la carte contenant les infos sur la carte
* ParamMap options : structure proposant des caractéristiques de la map
*/
bool lanceProgramme(int bActif, SDL_Surface *ecran,Map *carteJeu,ParamMap options,SDL_Joystick *joystick);

/* **********************************************
* credit                                        *
* procédure chargée d'afficher le menu "crédit" *
************************************************/
void credit(SDL_Surface *ecran);// lance le crédits
#endif // MENU_H_INCLUDED


