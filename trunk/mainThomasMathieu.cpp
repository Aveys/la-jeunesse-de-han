#ifdef __cplusplus
#include <cstdlib>
#else
#include <stdlib.h>
#endif
#ifdef __APPLE__
#include <SDL/SDL.h>
#else
#include <SDL.h>
#include <SDL/SDL_ttf.h>
#endif

#include "constantes.h"
#include "map.h"
#include "menu.h"
#include "deplace_mario.h"
#include "jeu.h"
#include "editeur.h"

int main ( int argc, char** argv )
{  // initialize SDL video
   if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
   {  fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError() );
      return 1;
   }

   SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

   if(TTF_Init() == -1)
   {  fprintf(stderr, "Unable to init TTF: %s\n", SDL_GetError());
      return 1;
   }


   //Mathieu : Initilisation editeur/map
    Map carteJeu;
    strcpy(carteJeu.filenameLevel, "lvl.txt");
    strcpy(carteJeu.filenameTileSet, "starwars.png");
    carteJeu.level = 1;
    carteJeu.LARGEUR_TILE = 32;
    carteJeu.HAUTEUR_TILE = 32;
    carteJeu.MONDE_LARGEUR = 31;
    carteJeu.MONDE_HAUTEUR = 15;
    carteJeu.xscroll = 0;
    carteJeu.yscroll = 0;
    carteJeu.largeur_win = 640;
    carteJeu.hauteur_win = 480;
    ParamMap options;
    options.afficheGrille = 1;


   // create a new window
   SDL_Surface* screen = SDL_SetVideoMode(640, 480, 32,SDL_HWSURFACE|SDL_DOUBLEBUF);
   if ( !screen )
   {  fprintf(stderr, "Unable to set 640x480 video: %s\n", SDL_GetError());
      return 1;
   }
   SDL_WM_SetCaption("MarioCBA", 0);



   /*if(menu(screen) != 0)
=======
   if(menu(screen) != 0)
>>>>>>> .r51
   {  fprintf(stderr, "Erreur au niveau du menu : %s", SDL_GetError());
      return 1;
   }
   TTF_Quit();
   SDL_Quit();
   return 0;

<<<<<<< .mine
   //Mathieu:
   /*Lancement editeur*/
    editeur(screen, carteJeu, options);
    jeu(screen,carteJeu);
    //Mathieu : Gestion du perso (Pourquoi pas la faire passer dans un fichier ?)





    SDL_Quit();
    return 0;

}
