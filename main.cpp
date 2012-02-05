/*********************************
* si erreur, vérifiez sdterr.txt *
*********************************/

#include "windows.h"
#include "constantes.h"
#include "map.h"
#include "deplace_mario.h"
#include "jeu.h"
#include "menu.h"
#include <fmodex/fmod.h>

/***************************************************
* fonction main                                    *
* initialise les modules nécessaires au programme  *
* appelle le menu si tout se passe bien            *
***************************************************/
int main ( int argc, char** argv )
{
    Fenetre fenetre;

    char *poubelle;
    poubelle = (char*)malloc(sizeof(char)*20);
    FILE *config = fopen("config.txt", "r");
    //fichier de configuration du jeu
    if(config == NULL)
    {
        MessageBox(NULL, (char*)"Impossible d'ouvrir config.txt", (char*)"erreur", MB_ICONERROR|MB_OK|MB_DEFBUTTON1|MB_SERVICE_NOTIFICATION);
        return 1;
    }
    // initialisation de SDL video
    if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK ) < 0 )
    {
        char erreur[60];
        sprintf(erreur, "Unable to init SDL: %s\n", SDL_GetError() );
        MessageBox(NULL, (char*)erreur, (char*)"erreur", MB_ICONERROR|MB_OK|MB_DEFBUTTON1|MB_SERVICE_NOTIFICATION);
        return 1;
    }
    SDL_putenv("SDL_VIDEO_CENTERED=center");
    //initialisation de SDL_ttf
    if(TTF_Init() == -1)
    {
        char erreur[60];
        sprintf(erreur, "Unable to init TTF: %s\n", SDL_GetError());
        MessageBox(NULL, (char*)erreur, (char*)"erreur", MB_ICONERROR|MB_OK|MB_DEFBUTTON1|MB_SERVICE_NOTIFICATION);
        return 1;
    }
    //on recupere les infos relatives à la fenetre
    fscanf(config, "%s = %i\n", poubelle, &fenetre.width);
    fscanf(config, "%s = %i\n", poubelle, &fenetre.height);
    fscanf(config, "%s = %i\n", poubelle, &fenetre.couleur);
    fscanf(config, "%s = %i", poubelle, &fenetre.fullscreen);
    free(poubelle);
    poubelle = NULL;
    fclose(config);

    // create a new window

    SDL_WM_SetIcon(IMG_Load("image/icon.bmp"), NULL);
    SDL_Surface* screen;
    if(!fenetre.fullscreen)
        screen = SDL_SetVideoMode(fenetre.width, fenetre.height, fenetre.couleur,SDL_HWSURFACE|SDL_DOUBLEBUF);
    else
        screen = SDL_SetVideoMode(fenetre.width, fenetre.height, fenetre.couleur,SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN);
    if ( !screen )
    {
        char erreur[60];
        sprintf(erreur, "Unable to set window: %s\n", SDL_GetError());
        MessageBox(NULL, (char*)erreur, (char*)"erreur", MB_ICONERROR|MB_OK|MB_DEFBUTTON1|MB_SERVICE_NOTIFICATION);
        return 1;
    }

    //si la fenetre s'est bien initialisée, on change le titre de la fenetre.

    SDL_WM_SetCaption("MarioCBA", NULL);
    SDL_Joystick *joystick = NULL;
    char erreur[60];
    sprintf(erreur,"%i joystick detecter",SDL_NumJoysticks());
    /*if(SDL_NumJoysticks() == 0)
        MessageBox(NULL,erreur , (char*)"erreur", MB_ICONERROR|MB_OK|MB_DEFBUTTON1|MB_SERVICE_NOTIFICATION);
    else
        MessageBox(NULL,erreur , (char*)"erreur", MB_ICONASTERISK|MB_OK|MB_DEFBUTTON1|MB_SERVICE_NOTIFICATION);*/
    SDL_JoystickEventState(SDL_ENABLE);
    joystick = SDL_JoystickOpen(0);


    //on appelle le menu.
    if(menu(screen,joystick) != 0)
    {
        MessageBox(NULL, (char*)"erreur menu", (char*)"erreur", MB_ICONERROR|MB_OK|MB_DEFBUTTON1|MB_SERVICE_NOTIFICATION);
        return 1;
    }

    //on quitte tout une fois le jeu fini.
    SDL_JoystickClose(joystick);
    TTF_Quit();
    SDL_Quit();
    return 0;

}
