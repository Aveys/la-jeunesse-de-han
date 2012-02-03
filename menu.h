#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED
/* *******************************************************************
* structure bouton,                                                  *
* contient les surfaces/police n�cessaire � l'affichage des boutons. *
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
* affiche le menu et fais le lien entre les diff�rentes fonctions du jeu *
* selon le choix du joueur                                               *
* renvoie 0 s'il n'y a pas eu de probl�me                                *
*************************************************************************/
int menu(SDL_Surface *screen,SDL_Joystick *joystick);

/* **************************************************************
* menuBouton                                                    *
* proc�dure d'affichage d'un bouton                             *
* le parametre bouton.pos.y doit etre renseign� avant son appel *
****************************************************************/
/* Parametres:
* Bouton *bouton : structure Bouton contenant surface et texte
* SDL_Surface *ecran : surface de la fenetre
* SDL_Color couleurTexte : �num�ration n�cessaire � la couleur du texte
* FILE *fichier : fichier contenant les textes du menu
* bool actif : d�finit si le bouton est le bouton actif ou non
* int nb : indice du bouton (utile pour le texte)
*/
void menuBouton(Bouton * bouton, SDL_Surface *ecran, SDL_Color couleurTexte, FILE *fichier, bool actif, int nb);

/* *******************************************************************************************************
* lanceProgramme                                                                                         *
* fonction charg� de lancer la fonction associ� au bouton actif dans le menu                             *
* renvoie un bool�en qui vaut vrai si une fonction a �t� lanc�e et faux si aucune fonction n'est d�finie *
*********************************************************************************************************/
/* Parametres:
* int bActif : donne le numero du bouton actif
* SDL_Surface *ecran : surface de la fenetre
* Map *carteJeu : structure de la carte contenant les infos sur la carte
* ParamMap options : structure proposant des caract�ristiques de la map
*/
bool lanceProgramme(int bActif, SDL_Surface *ecran,Map *carteJeu,ParamMap options,SDL_Joystick *joystick);

/* **********************************************
* credit                                        *
* proc�dure charg�e d'afficher le menu "cr�dit" *
************************************************/
void credit(SDL_Surface *ecran);// lance le cr�dits
#endif // MENU_H_INCLUDED


