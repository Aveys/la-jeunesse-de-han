#ifndef HIGHSCORE_H_INCLUDED
#define HIGHSCORE_H_INCLUDED
/* Texte
* *titre : surface du texte
* pos : structure de position du texte
*/
typedef struct
{
    SDL_Surface *titre;
    SDL_Rect pos;
} Texte;

/***************************************************************************************
* @name highscore                                                                       *
* proc�dure charg�e de compter le nombre de scores pr�sents dans le fichier score.txt, *
* puis d'allouer le tableau de score selon le nombre d'entr�es afin de les stocker.    *
* coordonne l'affichage des scores                                                     *
***************************************************************************************/
void highscore(SDL_Surface *ecran);

/* *******************************************************************************************************
* sauvegardeScore (*//**fonctionnelle mais non impl�ment�e*//*)                                     *                                                            *
* r�cup�re le score courant du joueur,                                                                   *
* le compare aux highscores,                                                                             *
* s'il est plus grand que l'un d'eux on �limine le dernier score et on �crit le nouveau dans le fichier. *
* les scores ne sont pas tri�s dans l'ordre � l'int�rieur                                                *
*********************************************************************************************************/
void sauvegardeScore(Score score);

/* *******************************************************************
* tri_highscore                                                      *
* r�cup�re en argument le tableau des scores et le nombre de scores, *
* trie la s�rie de scores du plus grand au plus petit                *
*********************************************************************/
void tri_highscore(Score s[], int nbJoueur);

/* ***************************************************************************************************************
* afficheHighscore                                                                                               *
* r�cup�re les scores et cr�e des surfaces d'affichages pour le pseudo et le score du joueur dans l'ordre du tri *
*****************************************************************************************************************/
/* Parametres:
* Score scoreJoueur : structure contenant les informations sur le score
* SDL_Surface *ecran : surface de la fenetre
* Texte pseudo : structure contenant la structure position et la surface du pseudo
* Texte score : structure contenant la structure position et la surface du score
* int i : indice du bouton (n�cessaire pour que les scores ne se superposent pas
*/
void afficheHighscore(Score scoreJoueur, SDL_Surface *ecran, Texte pseudo, Texte score, int i);

#endif
