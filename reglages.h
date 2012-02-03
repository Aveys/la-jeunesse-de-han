#ifndef REGLAGES_H_INCLUDED
#define REGLAGES_H_INCLUDED
//!Ataw -->
#define GRANDE_RESO "Résolution \n 1024x768"
#define MOYENNE_RESO "Résolution \n 800x600"
#define PETITE_RESO "Résolution \n 640x480"

#define PLEIN_ECRAN "plein écran \n oui"
#define FENETREE "plein écran \n non"

/*********************************************
* @name reglages                              *
* procédure coordonnant le menu des reglages *
*********************************************/
void reglages(SDL_Surface *ecran,SDL_Joystick *joystick);

/* *********************************************************
* bouton                                                   *
* procédure chargé d'afficher les boutons du menu réglages *
***********************************************************/
/* Parametres :
* SDL_Surface *ecran : surface de la fenetre
* Bouton *bouton : structure contenant surface et texte
* SDL_Color couleurTexte : énumération nécessaire à la coloration du texte du bouton
* int nbReso : indice de la resolution d'écran
* bool actif : booléen indiquant si le bouton est le bouton actif ou pas
*/
void bouton(SDL_Surface *ecran, Bouton *bouton, SDL_Color couleurTexte, int nbReso, bool actif);

/* ***************************************************************************************
* changeResolution                                                                       *
* fonction renvoyant le nouvel indice de résolution d'écran                              *                                                                                        *
*****************************************************************************************/
/* Parametres :
* bool sens : booléen indiquant si on appuie sur la touche "fleche gauche" (TRUE) ou sur la "fleche droite" ou "entrée" (FALSE)
* Fenetre *fenetre : structure contenant les informations sur la fenetre courante
* int nbReso : indice de resolution initial
*/
int changeResolution(bool sens, Fenetre *fenetre, int nbReso);

/* ***************************************************************************************************************
* saveConfig                                                                                                     *
* fonction chargée d'enregistrer les informations de le fenetre (structure Fenetre) dans le fichier "config.txt" *
*****************************************************************************************************************/
void saveConfig(Fenetre fenetre);

/* **********************************************************************************************************************
* pleinEcran                                                                                                            *
* fonction renvoyant un booléen (0 ou 1) selon si on est en mode fenetre (0) ou plein écran  (1), l'inverse est renvoyé *
************************************************************************************************************************/
int pleinEcran(int boolEcran);

//!<--Ataw

#endif // REGLAGES_H_INCLUDED
