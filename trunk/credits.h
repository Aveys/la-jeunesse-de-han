#ifndef CREDITS_H_INCLUDED
#define CREDITS_H_INCLUDED

/********************************************************************************************************
* @name credits                                                                                          *
* Fonction d'affichage principale du crédits avec une temporisation d'affichage des différents éléments *
********************************************************************************************************/
void credits(SDL_Surface *ecran);

/* ********************************************************************************************
* initecran                                                                                   *
* Permet de réafficher l'écran et le texte crédits à chaque changements de "page" des credits *
**********************************************************************************************/
void initecran(SDL_Surface *ecran);

#endif // CREDITS_H_INCLUDED
