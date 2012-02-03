#ifndef REGLAGES_H_INCLUDED
#define REGLAGES_H_INCLUDED
//!Ataw -->
#define GRANDE_RESO "R�solution \n 1024x768"
#define MOYENNE_RESO "R�solution \n 800x600"
#define PETITE_RESO "R�solution \n 640x480"

#define PLEIN_ECRAN "plein �cran \n oui"
#define FENETREE "plein �cran \n non"

/*********************************************
* @name reglages                              *
* proc�dure coordonnant le menu des reglages *
*********************************************/
void reglages(SDL_Surface *ecran,SDL_Joystick *joystick);

/* *********************************************************
* bouton                                                   *
* proc�dure charg� d'afficher les boutons du menu r�glages *
***********************************************************/
/* Parametres :
* SDL_Surface *ecran : surface de la fenetre
* Bouton *bouton : structure contenant surface et texte
* SDL_Color couleurTexte : �num�ration n�cessaire � la coloration du texte du bouton
* int nbReso : indice de la resolution d'�cran
* bool actif : bool�en indiquant si le bouton est le bouton actif ou pas
*/
void bouton(SDL_Surface *ecran, Bouton *bouton, SDL_Color couleurTexte, int nbReso, bool actif);

/* ***************************************************************************************
* changeResolution                                                                       *
* fonction renvoyant le nouvel indice de r�solution d'�cran                              *                                                                                        *
*****************************************************************************************/
/* Parametres :
* bool sens : bool�en indiquant si on appuie sur la touche "fleche gauche" (TRUE) ou sur la "fleche droite" ou "entr�e" (FALSE)
* Fenetre *fenetre : structure contenant les informations sur la fenetre courante
* int nbReso : indice de resolution initial
*/
int changeResolution(bool sens, Fenetre *fenetre, int nbReso);

/* ***************************************************************************************************************
* saveConfig                                                                                                     *
* fonction charg�e d'enregistrer les informations de le fenetre (structure Fenetre) dans le fichier "config.txt" *
*****************************************************************************************************************/
void saveConfig(Fenetre fenetre);

/* **********************************************************************************************************************
* pleinEcran                                                                                                            *
* fonction renvoyant un bool�en (0 ou 1) selon si on est en mode fenetre (0) ou plein �cran  (1), l'inverse est renvoy� *
************************************************************************************************************************/
int pleinEcran(int boolEcran);

//!<--Ataw

#endif // REGLAGES_H_INCLUDED
