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
#include "deplace_mario.h"
#include "highscore.h"
#include "ennemie.h"
#include "jeu.h"
#include "editeur.h"
#include <windows.h>
#include <math.h>
#include <fmodex/fmod.h>
void decoupeBlaster(Perso *player)
{
    int i;
    for(i=0; i<3; i++)
    {
        player->posSpriteBlaster[i].x = 22 * i;
        player->posSpriteBlaster[i].y = 0;
        player->posSpriteBlaster[i].h = 8;
        player->posSpriteBlaster[i].w = 66;
    }
}
//MatHack: Decoupage du sprite perso
void decoupePerso(Perso *perso1)
{
    int i;
    //Stand
    perso1->posSprite[0].x = 0;
    perso1->posSprite[0].y = 0;
    perso1->posSprite[0].w = 25;
    perso1->posSprite[0].h = 56;
    //De 1 a 8 = RunD de 9 a 16 = RunG
    for(i=1; i<16; i++)
    {
        perso1->posSprite[i].x = 52*i;
        perso1->posSprite[i].y = 0;
        perso1->posSprite[i].w = 52;
        perso1->posSprite[i].h = 56;
    }
}

void affiche_laser(Balle **tire_mario,int *nb_balle,Map *carte,SDL_Surface *screen, Perso player,Joueur *joueur1)
{
    //Decoupage sprite
    //image 1
    int i,j;
    for(i = 0; i < (*nb_balle); i++)
    {
        SDL_BlitSurface(tire_mario[0][i].balleSprite, &player.posSpriteBlaster[player.frameBlaster], screen, &tire_mario[0][i].positionB);
        tire_mario[0][i].positionB.x += tire_mario[0][i].vitesseX;
        //tire_mario[i].positionB.y += tire_mario[i].vitesseY;
        if(test_collision(tire_mario[0][i].positionB,carte,2,0,joueur1,&player,1) != 0 || tire_mario[0][i].positionB.x <= 0 || tire_mario[0][i].positionB.x > screen->w)
        {
            (*nb_balle)--;
            for(j = i+1; j < (*nb_balle); j++)
            {
                tire_mario[0][j-1] = tire_mario[0][j];    //deplacement du tableau pour ecraser la balle a supprimer
            }
            tire_mario[0] = (Balle*)realloc(tire_mario[0],(*nb_balle)*sizeof(Balle)); // reallocation du tableau de blaster
        }
        else
        {
            //printf("\nframeBlaster:%i\n", player.frameBlaster);
            if(player.frameBlaster > 2)
                player.frameBlaster = 0;
            player.frameBlaster++;
        }
    }
}
/*
    Gere les events de clavier : droite/gauche/haut/bas du perso + le blaster + le tir et le jet pack
*/
void gestion_touche(Perso *perso1, Map *carte, int *continuer,int *a_atteri,int *touche_enfonce,int *touche_a_ete_enfonce,int *new_balle,int *cpt_balle,int *jetPack,int *cptJetPack,SDL_Joystick *joystick,int *joystiskTouche,Balle **tire_mario,int *nb_balle)
{
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type)
    {
    case SDL_QUIT:
        if(SDLMessageBox((char*)"Voulez vous quitter le jeu ?",joystick))
        {
            TTF_Quit();
            SDL_Quit();
            exit(EXIT_SUCCESS);
        }
        break;
    case SDL_JOYBUTTONDOWN:
        if (event.jbutton.button == 5)
        {
            if(*cpt_balle > FRAMERATE_BALLE && perso1->dir != 0)    //creation d'un nouveau tir
            {
                (*nb_balle)++;
                tire_mario[0] = (Balle*)realloc(tire_mario[0],(*nb_balle)*sizeof(Balle)); //realloc tab avec 1 case en plus
                tire_mario[0][*nb_balle-1].positionB.x = perso1->positionP.x;   //init du X blaster a la valeur du X du perso
                if(perso1->dir < 0)      //si le perso va vers la gauche
                {
                    tire_mario[0][*nb_balle-1].vitesseX = perso1->dir*10;       //init vitesse
                    //Tire en diagonale désactiver pour le moment
                    //if(a_atteri == 1)
                    // mario_tire[0][*nb_balle-yscroll1].vitesseY = -10;     //si le perso est en l'air on tire en diagonale
                    //else
                    //tire_mario[0][*nb_balle-1].vitesseY = 0;   //sinon tire horizontale
                    tire_mario[0][*nb_balle-1].positionB.y = perso1->positionP.y+8;    //init du Y du blaster
                    tire_mario[0][*nb_balle-1].positionB.x -= 20;      //-20 pour le design
                    tire_mario[0][*nb_balle-1].imageBalle = (char*)"image/perso/blaster_tire_g.gif";
                }
                else if(perso1->dir > 0) //si le perso va vers la droite
                {
                    tire_mario[0][*nb_balle-1].vitesseX = perso1->dir*10;
                    tire_mario[0][*nb_balle-1].positionB.x += 50;
                    //Tire en diagonale désactiver pour le moment
                    //if(a_atteri == 1)
                    // tire_mario[0][*nb_balle-1].vitesseY = -10;
                    //else
                    //tire_mario[0][*nb_balle-1].vitesseY = 0;
                    tire_mario[0][*nb_balle-1].positionB.y = perso1->positionP.y+8;
                    tire_mario[0][*nb_balle-1].imageBalle = (char*)"image/perso/blaster_tire_d.gif";
                }
                tire_mario[0][*nb_balle-1].balleSprite = IMG_Load(tire_mario[0][*nb_balle-1].imageBalle); //chargement de l'image
                associer_surface_pos(tire_mario[0][*nb_balle-1].balleSprite,&tire_mario[0][*nb_balle-1].positionB);
                cpt_balle = 0;
                *new_balle=1;
            }
        }
        else if (event.jbutton.button == 6)
        {
            //retour menu
            if(SDLMessageBox((char*)"Voulez vous retourner au menu ?",joystick))
            {
                *continuer = 0;
            }
        }
        else if (event.jbutton.button == 4)
        {
            //init jet pack
            *jetPack = 1;
        }
        else if (event.jbutton.button == 7)
        {
            pause();
        }
        break;
    case SDL_JOYBUTTONUP:
        if (event.jbutton.button == 4)
        {
            //init jet pack
            *jetPack = 0;
        }
        break;
    case SDL_KEYDOWN:
        switch(event.key.keysym.sym)
        {
        case SDLK_DOWN:
            perso1->sensyp = 8;
            break;
        case SDLK_UP:
            if(*a_atteri == 0 || (*jetPack == 1 && *cptJetPack > 0))    //si il a atteri ou si le jet pack est actif alors on peut faire sauter le perso
            {
                perso1->sensyp = -6;
                perso1->frameActu = 1;
            }
            *a_atteri = 1;
            perso1->dir = 0;
            break;
        case SDLK_SPACE:
            if(*cpt_balle > FRAMERATE_BALLE && perso1->dir != 0)    //creation d'un nouveau tir
            {
                (*nb_balle)++;
                tire_mario[0] = (Balle*)realloc(tire_mario[0],(*nb_balle)*sizeof(Balle)); //realloc tab avec 1 case en plus
                tire_mario[0][*nb_balle-1].positionB.x = perso1->positionP.x;   //init du X blaster a la valeur du X du perso
                if(perso1->dir < 0)      //si le perso va vers la gauche
                {
                    tire_mario[0][*nb_balle-1].vitesseX = perso1->dir*10;       //init vitesse
                    //Tire en diagonale désactiver pour le moment
                    //if(a_atteri == 1)
                    // mario_tire[0][*nb_balle-yscroll1].vitesseY = -10;     //si le perso est en l'air on tire en diagonale
                    //else
                    //tire_mario[0][*nb_balle-1].vitesseY = 0;   //sinon tire horizontale
                    tire_mario[0][*nb_balle-1].positionB.y = perso1->positionP.y+8;    //init du Y du blaster
                    tire_mario[0][*nb_balle-1].positionB.x -= 20;      //-20 pour le design
                    tire_mario[0][*nb_balle-1].imageBalle = (char*)"image/perso/blaster_tire_g.gif";
                }
                else if(perso1->dir > 0) //si le perso va vers la droite
                {
                    tire_mario[0][*nb_balle-1].vitesseX = perso1->dir*10;
                    tire_mario[0][*nb_balle-1].positionB.x += 50;
                    //Tire en diagonale désactiver pour le moment
                    //if(a_atteri == 1)
                    // tire_mario[0][*nb_balle-1].vitesseY = -10;
                    //else
                    //tire_mario[0][*nb_balle-1].vitesseY = 0;
                    tire_mario[0][*nb_balle-1].positionB.y = perso1->positionP.y+8;
                    tire_mario[0][*nb_balle-1].imageBalle = (char*)"image/perso/blaster_tire_d.gif";
                }
                tire_mario[0][*nb_balle-1].balleSprite = IMG_Load(tire_mario[0][*nb_balle-1].imageBalle); //chargement de l'image
                associer_surface_pos(tire_mario[0][*nb_balle-1].balleSprite,&tire_mario[0][*nb_balle-1].positionB);
                cpt_balle = 0;
                *new_balle=1;
            }
            break;
        case SDLK_RIGHT:
            perso1->ok++;
            //perso1->frameActu = 1;
            if(*touche_enfonce == 0)
                perso1->ok = perso1->vitesseAnim;
            if(perso1->ok >= perso1->vitesseAnim)
            {
                if(perso1->frameActu >= 8)
                    perso1->frameActu = 1;
                perso1->ok = 0;
                perso1->frameActu += 1;

            }
            perso1->sensxp = perso1->vitesse;   //le perso va vers la droite
            //carte->posFond.x -=5;
            /*if(perso1->dir != 1 && strcmp(perso1->imagePerso,"image/perso/persoPara.bmp") != 0)
            {
                perso1->point_tab[0].x = 2;
                perso1->point_tab[0].y = 11;
                perso1->point_tab[1].x = 34;
                perso1->point_tab[1].y = 5;
                perso1->point_tab[2].x = 48;
                perso1->point_tab[2].y = 24;
                perso1->point_tab[3].x = 52;
                perso1->point_tab[3].y = 56;
                perso1->point_tab[4].x = 46;
                perso1->point_tab[4].y = 55;
                perso1->point_tab[5].x = 29;
                perso1->point_tab[5].y = 48;
                perso1->point_tab[6].x = 2;
                perso1->point_tab[6].y = 11;
            }*/
            perso1->dir = 1;
            *touche_enfonce = 1;    //sert pour la détection de 2 touches simultanement
            break;
        case SDLK_LEFT:
            perso1->sensxp = -perso1->vitesse;      //perso va vers la gauche
            perso1->ok++;
            //perso1->frameActu = 9;
            if(*touche_enfonce == 0)
                perso1->ok = perso1->vitesseAnim;
            if(perso1->ok >= perso1->vitesseAnim)
            {
                if(perso1->frameActu >= 15 || perso1->frameActu < 8)
                    perso1->frameActu = 8;
                perso1->ok = 0;
                perso1->frameActu += 1;
            }
            //carte->posFond.x +=5;
            /*if(perso1->dir != -1 && strcmp(perso1->imagePerso,"image/perso/persoPara.bmp") != 0)
            {
                perso1->point_tab[0].x = 9;
                perso1->point_tab[0].y = 7;
                perso1->point_tab[1].x = 48;
                perso1->point_tab[1].y = 7;
                perso1->point_tab[2].x = 37;
                perso1->point_tab[2].y = 55;
                perso1->point_tab[3].x = 0;
                perso1->point_tab[3].y = 50;
                perso1->point_tab[4].x = 1;
                perso1->point_tab[4].y = 40;
                perso1->point_tab[5].x = 7;
                perso1->point_tab[5].y = 25;
                perso1->point_tab[6].x = 9;
                perso1->point_tab[6].y = 7;
            }*/
            perso1->dir = -1;
            *touche_enfonce = -1;
            break;
            //MatHack: Demande si l'on veut quitter le jeu
        case SDLK_ESCAPE:   //retour menu
            if(SDLMessageBox((char*)"Voulez vous retourner au menu ?",joystick))
                *continuer = 0;
            break;
        case SDLK_c:        //init jet pack
            *jetPack = 1;
            break;
            //MatHack: Pause du jeu
        case SDLK_p:
            pause();
            break;
        default:
            break;
        }
        break;
    case SDL_KEYUP:
        if(event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_RIGHT) //si on relache droite ou gauche alors touche_enfonce passe a 0
        {
            *touche_enfonce = 0;

        }
        else if(event.key.keysym.sym == SDLK_c) //si on relache c alors on arrete le jet pack
        {
            *jetPack = 0;
        }
        break;
    case SDL_JOYAXISMOTION:
        //fprintf(stderr,"%i,%i\n",SDL_JoystickGetAxis(joystick,0),SDL_JoystickGetAxis(joystick,1));
        if(SDL_JoystickGetAxis(joystick,0) < -JOY)
        {
            // Vers la gauche
            perso1->sensxp = -perso1->vitesse;      //perso va vers la gauche
            perso1->ok++;
            //perso1->frameActu = 9;
            if(*touche_enfonce == 0)
                perso1->ok = perso1->vitesseAnim;
            if(perso1->ok >= perso1->vitesseAnim)
            {
                if(perso1->frameActu >= 15 || perso1->frameActu < 8)
                    perso1->frameActu = 8;
                perso1->ok = 0;
                perso1->frameActu += 1;
            }
            //carte->posFond.x +=5;
            perso1->dir = -1;
            *touche_enfonce = -1;

        }
        else if(SDL_JoystickGetAxis(joystick,0) > JOY)
        {
            // Vers la droite
            perso1->ok++;
            //perso1->frameActu = 1;
            if(*touche_enfonce == 0)
                perso1->ok = perso1->vitesseAnim;
            if(perso1->ok >= perso1->vitesseAnim)
            {
                if(perso1->frameActu >= 8)
                    perso1->frameActu = 1;
                perso1->ok = 0;
                perso1->frameActu += 1;

            }
            perso1->sensxp = perso1->vitesse;   //le perso va vers la droite
            //carte->posFond.x -=5;
            perso1->dir = 1;
            *touche_enfonce = 1;    //sert pour la détection de 2 touches simultanement
        }
        else
        {
            *touche_enfonce = 0;
            fprintf(stderr,"%i\n",SDL_JoystickGetAxis(joystick,0));
        }
        if(event.jaxis.axis == 1 && SDL_JoystickGetAxis(joystick,1) < -JOY)
        {
            // Vers le haut
            if(*a_atteri == 0 || (*jetPack == 1 && *cptJetPack > 0))    //si il a atteri ou si le jet pack est actif alors on peut faire sauter le perso
            {
                perso1->sensyp = -8;
                perso1->frameActu = 1;
            }
            *a_atteri = 1;

        }
        else if(event.jaxis.axis == 1 && SDL_JoystickGetAxis(joystick,1) > JOY)
        {

            // Vers le bas
            perso1->sensyp = 8;
        }
        break;
    case SDL_JOYHATMOTION:  /* Handle Joyball Motion */
        if ( event.jhat.value & SDL_HAT_UP )
        {
            if(*a_atteri == 0 || (*jetPack == 1 && *cptJetPack > 0))    //si il a atteri ou si le jet pack est actif alors on peut faire sauter le perso
            {
                perso1->sensyp = -6;
                perso1->frameActu = 1;
            }
            *a_atteri = 1;
            perso1->dir = 0;
        }
        else if ( event.jhat.value & SDL_HAT_DOWN )
        {
            perso1->sensyp = 8;
        }
        if ( event.jhat.value & SDL_HAT_LEFT )
        {
            perso1->sensxp = -perso1->vitesse;      //perso va vers la gauche
            perso1->ok++;
            //perso1->frameActu = 9;
            if(*touche_enfonce == 0)
                perso1->ok = perso1->vitesseAnim;
            if(perso1->ok >= perso1->vitesseAnim)
            {
                if(perso1->frameActu >= 15 || perso1->frameActu < 8)
                    perso1->frameActu = 8;
                perso1->ok = 0;
                perso1->frameActu += 1;
            }
            //carte->posFond.x +=5;
            /*if(perso1->dir != -1 && strcmp(perso1->imagePerso,"image/perso/persoPara.bmp") != 0)
            {
                perso1->point_tab[0].x = 9;
                perso1->point_tab[0].y = 7;
                perso1->point_tab[1].x = 48;
                perso1->point_tab[1].y = 7;
                perso1->point_tab[2].x = 37;
                perso1->point_tab[2].y = 55;
                perso1->point_tab[3].x = 0;
                perso1->point_tab[3].y = 50;
                perso1->point_tab[4].x = 1;
                perso1->point_tab[4].y = 40;
                perso1->point_tab[5].x = 7;
                perso1->point_tab[5].y = 25;
                perso1->point_tab[6].x = 9;
                perso1->point_tab[6].y = 7;
            }*/
            perso1->dir = -1;
            *touche_enfonce = -1;
        }
        else if ( event.jhat.value & SDL_HAT_RIGHT )
        {
            perso1->ok++;
            //perso1->frameActu = 1;
            if(*touche_enfonce == 0)
                perso1->ok = perso1->vitesseAnim;
            if(perso1->ok >= perso1->vitesseAnim)
            {
                if(perso1->frameActu >= 8)
                    perso1->frameActu = 1;
                perso1->ok = 0;
                perso1->frameActu += 1;

            }
            perso1->sensxp = perso1->vitesse;   //le perso va vers la droite
            //carte->posFond.x -=5;
            /*if(perso1->dir != 1 && strcmp(perso1->imagePerso,"image/perso/persoPara.bmp") != 0)
            {
                perso1->point_tab[0].x = 2;
                perso1->point_tab[0].y = 11;
                perso1->point_tab[1].x = 34;
                perso1->point_tab[1].y = 5;
                perso1->point_tab[2].x = 48;
                perso1->point_tab[2].y = 24;
                perso1->point_tab[3].x = 52;
                perso1->point_tab[3].y = 56;
                perso1->point_tab[4].x = 46;
                perso1->point_tab[4].y = 55;
                perso1->point_tab[5].x = 29;
                perso1->point_tab[5].y = 48;
                perso1->point_tab[6].x = 2;
                perso1->point_tab[6].y = 11;
            }*/
            perso1->dir = 1;
            *touche_enfonce = 1;    //sert pour la détection de 2 touches simultanement
        }
        else
            *touche_enfonce = 0;
        break;
    default:
        break;
    }
}

//MatHack:Fonction qui permet la pause du jeux
void pause()
{
    SDL_Surface *fondPause=NULL, *textePause=NULL;
    SDL_Rect posPFond, posTextePause;
    SDL_Event eventPause;

    //Fond
    posPFond.x = 0;
    posPFond.y = 0;
    fondPause = SDL_CreateRGBSurface(SDL_HWSURFACE, SDL_GetVideoSurface()->w, SDL_GetVideoSurface()->h, 32, 0, 0, 0, 0);
    SDL_FillRect(fondPause, NULL, SDL_MapRGB(fondPause->format, 0, 0, 0) );

    //Intilise Texte
    TTF_Font *font=NULL;
    font = TTF_OpenFont("calibri.ttf", SDL_GetVideoSurface()->w/12.5);
    SDL_Color couleur = {255, 255, 255};
    //Texte
    posTextePause.x =  SDL_GetVideoSurface()->w/2 - 80;
    posTextePause.y =  SDL_GetVideoSurface()->h/2 - (80/2);

    SDL_EnableKeyRepeat(200,1);
    int continuer = 1;
    int blitter = 0;

    while(continuer)
    {
        SDL_PollEvent(&eventPause);
        switch (eventPause.type)
        {
        case SDL_QUIT:
            continuer = 0;
            break;
        case SDL_KEYDOWN:
            switch(eventPause.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                continuer = 0;
                break;
            case SDLK_p:
                continuer = 0;
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
        if(blitter==0)
        {
            SDL_SetAlpha(fondPause, SDL_SRCALPHA, 128);
            SDL_BlitSurface(fondPause, NULL, SDL_GetVideoSurface(), &posPFond);
            //Texte
            textePause=TTF_RenderText_Blended(font, "Pause", couleur);
            SDL_BlitSurface(textePause, 0, SDL_GetVideoSurface(), &posTextePause);
            //Refesh
            SDL_Flip(SDL_GetVideoSurface());
            blitter=1;
        }
    }
    //Free mémoire
    TTF_CloseFont(font);
    SDL_FreeSurface(textePause);
    SDL_FreeSurface(fondPause);
    SDL_EnableKeyRepeat(1,1);


}
/*
    deplace mario permet de gerer le déplacement et les collisions du perso
*/
int deplace_mario(Perso *perso1,Map *carte,int decalage,SDL_Surface* screen,int *a_atteri,Joueur *j,int *continuer,FMOD_SYSTEM *system,FMOD_SOUND *piece,Score *s,Ennemi *tabEnnemi,int *reinit_ennemi)
{
    perso1->positionP.x += perso1->sensxp;  //position du perso suivantes
    perso1->positionP.y += perso1->sensyp;
    /*int i,k;
    SDL_LockSurface(perso1->persoSprite);  // Vérouillage de la surface
    Uint8* pPixels = (Uint8*)perso1->persoSprite->pixels;  // Récupération d'un pointeur vers les pixels de la surface
    Uint8 r, g, b,a;    // Variables servant à stocker la couleur du pixel choisi
    for(i = 0; i < perso1->positionP.h; i++)
    {
        for(k = 0; k < perso1->positionP.w; k++)
        {
            r = *(pPixels+4*(k*perso1->positionP.w+(i+1)));
            g = *(pPixels+4*(k*perso1->positionP.w+(i+1))+1);
            b = *(pPixels+4*(k*perso1->positionP.w+(i+1))+2);
            a = *(pPixels+4*(k*perso1->positionP.w+(i+1))+3);
            //fprintf(stderr,"%i,%i,%i,%i",r,g,b,a);
        }
        //fprintf(stderr,"\n");
    }
    SDL_UnlockSurface(perso1->persoSprite); //Dévérouillage de la surface
    fprintf(stderr,"\n");*/
    int retour_test = test_collision(perso1->positionP,carte,0,0,NULL,perso1,0);  //test de la valeur de la case des positions du perso
    if(retour_test == FIN_LVL)
    {
        //MatHack:Pour eviter les bug de chargement de niveau qui n'existe pas on quitte sur le menu si le niveau suivant n'est pas possible
        if(carte->level<carte->nbNiveau-1)
        {
            /*char test[100];
            sprintf(test, "Level:%i, NbNiveau:%i", carte->level, carte->nbNiveau);
            MessageBox(NULL, test, "Test", MB_ICONASTERISK|MB_OK|MB_DEFBUTTON1|MB_SERVICE_NOTIFICATION);*/
            *a_atteri = 1;
            carte->level++;
            initMap(carte);
            chargeMap(carte);
            initPosPerso(perso1,carte,screen);
            *reinit_ennemi = 1;
            //MatHack:Affichage du prochaine niveau
            SDL_Surface *niveauSuiFond = NULL, *niveauSuiText = NULL;
            SDL_Rect posFond, posTexte;
            posFond.x = 0;
            posFond.y = 0;
            niveauSuiFond = SDL_CreateRGBSurface(SDL_HWSURFACE, screen->w, screen->h, 32, 0, 0, 0, 0);
            SDL_FillRect(niveauSuiFond, NULL, SDL_MapRGB(niveauSuiFond->format, 0, 0, 0) );
            SDL_BlitSurface(niveauSuiFond, NULL, screen, &posFond);
            SDL_Flip(screen);
            //Intilise Texte
            TTF_Font *font=NULL;
            font = TTF_OpenFont("calibri.ttf", screen->w/12.5);
            SDL_Color couleur = {255, 255, 255};
            //Texte
            posTexte.x = screen->w/2 - ((4*80)/2);
            posTexte.y = screen->h/2 - (80/2);
            char niveau[20];
            sprintf(niveau, "Niveau %i", carte->level+1);
            niveauSuiText=TTF_RenderText_Blended(font, niveau, couleur);
            SDL_BlitSurface(niveauSuiText, 0, screen, &posTexte);
            SDL_Flip(screen);

            //Libération mémoire
            SDL_FreeSurface(niveauSuiFond);
            SDL_FreeSurface(niveauSuiText);
            TTF_CloseFont(font);
            SDL_Delay(1500);

            perso1->positionP.x = carte->posDepart.x;
            perso1->positionP.y = carte->posDepart.y;
            perso1->imagePerso = (char*)"image/perso/persoPara.bmp";       //saut en parachute
            SDL_FreeSurface(perso1->persoSprite);
            perso1->persoSprite = IMG_Load(perso1->imagePerso);
            carte->nb_ennemi = 0;
            /*tabEnnemi = (Ennemi*)malloc(sizeof(Ennemi)*carte->nb_ennemi);
            int i;
            for(i = 0;i < carte->nb_ennemi;i++)
                initEnnemi(*carte,&tabEnnemi[i],i,screen);*/
        }
        else
        {
            *continuer = 0;
        }

        return 0;
    }
    if(scrollingPersoX(*perso1, carte, screen) == 0)    //si le scrolling est en mvt
    {
        perso1->positionP.x -= perso1->sensxp;
        if(retour_test == MUR_COLLISION)    //si le perso est dans le mur
        {
            carte->xscroll -= perso1->sensxp;   //on reviens a la position precedente du scrolling et du perso
            perso1->positionP.y -= perso1->sensyp;
            perso1->positionP.x -= perso1->sensxp;
            perso1->sensxp = 0;     //on stop le deplacement
        }
        else if(retour_test == ATTERI)  //si il a atteri
        {
            carte->xscroll -= perso1->sensxp;   //on reviens a la position precedente du scrolling et du perso
            perso1->positionP.y -= perso1->sensyp;
            if(*a_atteri != 0)
            {
                perso1->positionP.x -= perso1->sensxp;
                perso1->sensxp = 0;     //on stop le deplacement
            }
            perso1->sensyp = 0;
            *a_atteri = 0;
        }
        else if(retour_test == BONUS)   //si il attrape une brique bonus
        {
            FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, piece, 0, NULL);
            j->nbpiece++;   //+1piece
            j->score += 100;    //+100score
            perso1->sensyp = 1; //le perso redescend
        }

    }
    else    //si il n'est pas en scrolling horizontale
    {
        if(retour_test == MUR_COLLISION)
        {
            perso1->positionP.x -= perso1->sensxp;  //retour pos x precedente
            if(*a_atteri == 1)  //si il est en l'air
            {
                if(perso1->sensyp < 0)  //si il va vers le haut
                    perso1->positionP.y -= perso1->sensyp;  //alors pos y precedente
                perso1->sensxp = 0;     //direction vers le bas
                perso1->sensyp = 1;
            }
        }
        else if(retour_test == ATTERI)
        {
            if(*a_atteri != 0)
            {
                perso1->positionP.x -= perso1->sensxp;
                perso1->sensxp = 0;     //on stop le deplacement
            }
            perso1->positionP.y -= perso1->sensyp;
            perso1->sensyp = 0;
            *a_atteri = 0;
        }
    }
    if(scrollingPersoY(*perso1, carte, screen) == 0)    //si la map est en scrolling verticale
    {
        perso1->positionP.y -= perso1->sensyp;  //on retourne mario a la pos precedente car cest la map qui bouge
        /*if(retour_test == MUR_COLLISION)    //si le perso est dans le mur
        {
            perso1->positionP.y -= perso1->sensyp;
            perso1->positionP.x -= perso1->sensxp;
            perso1->sensxp = 0;     //on stop le deplacement
        }
        else if(retour_test == ATTERI)  //si il a atteri
        {
            perso1->positionP.x -= perso1->sensxp;
            perso1->positionP.y -= perso1->sensyp;
            if(*a_atteri != 0)
            {
                perso1->sensxp = 0;     //on stop le deplacement
            }
            perso1->sensyp = 0;
            *a_atteri = 0;
        }
        else if(retour_test == BONUS)   //si il attrape une brique bonus
        {
            FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, piece, 0, NULL);
            j->nbpiece++;   //+1piece
            j->score += 100;    //+100score
            perso1->sensyp = 1; //le perso redescend
        }*/
    }
    else
    {
        if(retour_test == MUR_COLLISION)
        {
            perso1->positionP.x -= perso1->sensxp;
            perso1->positionP.y -= perso1->sensyp;
            if(*a_atteri == 1)
            {
                perso1->sensyp = -1;    //si il est en l'air alors il remonte
            }
        }
        else if(retour_test == ATTERI)
        {
            perso1->positionP.y -= perso1->sensyp;
            perso1->sensyp = 0;
            if(*a_atteri != 0)
            {
                perso1->positionP.x -= perso1->sensxp;
                perso1->sensxp = 0;     //on stop le deplacement
            }
            *a_atteri = 0;
        }
    }
    //partie sans scrolling ni verticale ni horizontale
    if(retour_test == MUR_COLLISION)
    {
        perso1->positionP.y -= perso1->sensyp;
        perso1->positionP.x -= perso1->sensxp;
        perso1->sensyp = 1;     //perso vers le bas
        if(*a_atteri == 1)      //si il est en l'air
        {
            perso1->sensxp = 0; //stop le deplacement horizontale
            if(test_collision(perso1->positionP,carte,1,0,NULL,perso1,0) == ATTERI)   //test si le perso a atteri car il detecte qu'une collision a la fois
            {
                perso1->positionP.y -= perso1->sensyp;
                perso1->sensyp = 0;
                *a_atteri = 0;
            }
        }
    }
    else if(retour_test == ATTERI)
    {
        perso1->positionP.y -= perso1->sensyp;
        perso1->sensyp = 0;
        if(*a_atteri != 0)
        {
            perso1->positionP.x -= perso1->sensxp;
            perso1->sensxp = 0;     //on stop le deplacement
        }
        *a_atteri = 0;
    }
    else if(retour_test == BONUS)
    {
        FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, piece, 0, NULL);
        j->nbpiece++;
        perso1->positionP.y -= perso1->sensyp;
        perso1->sensyp = 1;     //perso vers le bas
        j->score += 10;//Ajoute 10 quand prend un bonus
    }
    else if(retour_test == FIN_MAP) //si il tombe dans un trou
    {
        initPosPerso(perso1,carte,screen);
        *a_atteri = 1;
        if(j->nbvie <= 1)
        {
            //MatHack
            //Affiche gameover
            //Fond
            sauvegardeScore(*s);
            SDL_Surface *gameOverFond = NULL, *gameOverText = NULL;
            SDL_Rect posFond, posTexte;
            posFond.x = 0;
            posFond.y = 0;
            gameOverFond = SDL_CreateRGBSurface(SDL_HWSURFACE, screen->w, screen->h, 32, 0, 0, 0, 0);
            SDL_FillRect(gameOverFond, NULL, SDL_MapRGB(gameOverFond->format, 0, 0, 0) );
            SDL_BlitSurface(gameOverFond, NULL, screen, &posFond);
            SDL_Flip(screen);
            //Intilise Texte
            TTF_Font *font=NULL;
            font = TTF_OpenFont("calibri.ttf", screen->w/12.5);
            SDL_Color couleur = {255, 255, 255};
            //Texte
            posTexte.x = screen->w/2 - ((5*80)/2);
            posTexte.y = screen->h/2 - (80/2);
            gameOverText=TTF_RenderText_Blended(font, "Game Over", couleur);
            SDL_BlitSurface(gameOverText, 0, screen, &posTexte);
            SDL_Flip(screen);

            //Libération mémoire
            SDL_FreeSurface(gameOverFond);
            SDL_FreeSurface(gameOverText);
            TTF_CloseFont(font);
            SDL_Delay(2000);
            *continuer = 0;
        }

        j->nbvie--;

    }
    else    //si il n'y a rien alors on fait l'effet de la gravité vers le bas
    {
        if(perso1->sensyp > -1)
        {
            perso1->sensyp = 1;
        }

    }
    return 0;
}
/*
    Permet de determiner les n° des cases dans le tableau de la map autour de la surface envoyée en paramètre
*/
void position_perso_tab(SDL_Rect surface_a_tester,int *posxmin,int *posymin,int *posxmax,int *posymax,Map carte,int opt)
{
    if(opt == 0)    //avec scrolling (perso et blaster)
    {
        *posxmin = (surface_a_tester.x + carte.xscroll + LTILE)/LTILE;
        *posymin = (surface_a_tester.y + carte.yscroll + LTILE)/HTILE;
        *posxmax = (surface_a_tester.x + carte.xscroll + surface_a_tester.w + LTILE)/LTILE;
        *posymax = (surface_a_tester.y + carte.yscroll + surface_a_tester.h + HTILE)/HTILE;
    }
    else    //sans scrolling (ennemis)
    {
        *posxmin = (surface_a_tester.x + LTILE)/LTILE;
        *posymin = (surface_a_tester.y + LTILE)/HTILE;
        *posxmax = (surface_a_tester.x + surface_a_tester.w + LTILE)/LTILE;
        *posymax = (surface_a_tester.y + surface_a_tester.h + HTILE)/HTILE;
    }
}
/*
    retourne la valeur de la case ou se trouve la surface envoyée en paramètre
*/
int test_collision(SDL_Rect surface_a_tester,Map *carte,int opt,int opt2,Joueur *joueur1,Perso *perso1,int no_perso)
{
    int posxmin,posymin,posxmax,posymax,i,j;
    //Point p;
    position_perso_tab(surface_a_tester,&posxmin,&posymin,&posxmax,&posymax,*carte,opt2);   //determination des cases a tester
    if(opt == 1)    //si on veut tester si le perso a atteri alors on test que les cases en bas de la surface
        posymin = posymax;
    if(opt == 3)    //si on veut tester si le perso a atteri alors on test que les cases en bas de la surface
        posymax--;
    for(i = posymin ; i <= posymax; i++)    //boucle verticale
    {
        for(j = posxmin; j <= posxmax; j++) //boucle horizontale
        {
            if(i <= 0 || j <= 0)    //si ca sort de la map
            {
                return MUR_COLLISION;
            }
            else if(i > carte->MONDE_HAUTEUR - 1|| j > carte->MONDE_LARGEUR - 1)    //si il est en dessous de la map
                return FIN_MAP;
            else if(carte->tableauMap[i-1][j-1] != 0)   //si la case n'est pas vide
            {
                /*p.x = (j-1)*LTILE-1;
                p.y = (i-1)*HTILE-1;
                if(no_perso == 1 || Collision(perso1,p,carte->xscroll,carte->yscroll) == 1)
                {*/
                switch(carte->tableauMap[i-1][j-1])
                {
                case tileMUR:   //mur
                    if(i == posymax)    //si la case est sur le bas du perso alors il atteri
                    {
                        if(no_perso != 0)
                            return ATTERI;
                        else if(perso1->positionP.y+carte->yscroll+perso1->persoSprite->h-perso1->sensyp <= (i-1)*HTILE)
                            return ATTERI;
                    }
                    return MUR_COLLISION;
                    break;
                case tileReserve:   //mur destructible
                    if(opt == 2)    //si il est touche par un missile alors il se detruit
                    {
                        carte->tableauMap[i-1][j-1] = 0;
                        joueur1->score += 5;    //score +5
                        if(i == posymax)   //sinon comme un mur normal
                            return ATTERI;
                    }
                    else if(i == posymax)    //si la case est sur le bas du perso alors il atteri
                    {
                        if(no_perso != 0)
                            return ATTERI;
                        else if(perso1->positionP.y+carte->yscroll+perso1->persoSprite->h-perso1->sensyp <= (i-1)*HTILE)
                            return ATTERI;
                    }
                    return MUR_COLLISION;
                    break;
                case tileSOL:   //si le perso est en bas de la map
                    if(i == posymax)
                        return ATTERI;
                    return MUR_COLLISION;
                    break;
                case tileBONUS:     //si il a attraper un bonus
                    /*if(i != posymax || perso1->sensyp < 0)    //si il a taper avec la tete
                    {*/
                    carte->tableauMap[i-1][j-1] = 0;    //suppression de la case
                    return BONUS;
                    /*}
                    else if(i == posymax)    //si la case est sur le bas du perso alors il atteri
                    {
                        tmp = (surface_a_tester.x+surface_a_tester.w+carte->xscroll) - (j-1)*LTILE;
                        tmp2 = (surface_a_tester.x+carte->xscroll) - j*LTILE;
                        if(no_perso == 0 && ((perso1->dir > 0 && tmp > 0 && tmp < LTILE+surface_a_tester.w) || (perso1->dir < 0 && tmp2 < 0 && tmp2+surface_a_tester.w < LTILE)))
                            return ATTERI;
                    }
                    return MUR_COLLISION;*/
                    break;
                case tileFIN:     //fin lvl
                    return FIN_LVL;
                    break;
                case 'f':     //fin lvl
                    return FIN_LVL;
                    break;
                }
                // }
            }
        }
    }
    return 0;
}
int CollisionDroiteSeg(Point A,Point B,Point O,Point P)
{
    Vecteur AO,AP,AB;
    AB.x = B.x - A.x;
    AB.y = B.y - A.y;
    AP.x = P.x - A.x;
    AP.y = P.y - A.y;
    AO.x = O.x - A.x;
    AO.y = O.y - A.y;
    if ((AB.x*AP.y - AB.y*AP.x)*(AB.x*AO.y - AB.y*AO.x)<0)
        return 1;
    else
        return 0;
}
int CollisionSegSeg(Point A,Point B,Point O,Point P)
{
    if (CollisionDroiteSeg(A,B,O,P)==0)
        return 0;  // inutile d'aller plus loin si le segment [OP] ne touche pas la droite (AB)
    Vecteur AB,OP;
    AB.x = B.x - A.x;
    AB.y = B.y - A.y;
    OP.x = P.x - O.x;
    OP.y = P.y - O.y;
    float k = -(A.x*OP.y-O.x*OP.y-OP.x*A.y+OP.x*O.y)/(AB.x*OP.y-AB.y*OP.x);
    if (k<0 || k>1)
        return 0;
    else
        return 1;
}

int Collision(Perso *perso1,Point O,int xscroll,int yscroll)
{
    int i;
    Point A;
    Point B;
    Point P;
    for(i=0; i<NB_PT; i++)
    {
        P.x = O.x+LTILE+1;
        P.y = O.y+1;
        A.x = perso1->point_tab[i].x;
        A.y = perso1->point_tab[i].y;
        if (i==NB_PT-1)  // si c'est le dernier point, on relie au premier
            B = perso1->point_tab[0];
        else           // sinon on relie au suivant.
            B = perso1->point_tab[i+1];
        A.x += (float)perso1->positionP.x+xscroll;
        B.x += (float)perso1->positionP.x+xscroll;
        A.y += (float)perso1->positionP.y+yscroll;
        B.y += (float)perso1->positionP.y+yscroll;
        if(CollisionSegSeg(A,B,O,P))
        {
            return 1;
        }
        O.x += LTILE;
        O.y += HTILE;
        if(CollisionSegSeg(A,B,P,O))
        {
            return 1;
        }
        P.x -= LTILE;
        P.y += HTILE;
        if(CollisionSegSeg(A,B,P,O))
        {
            return 1;
        }
        O.x -= LTILE;
        O.y -= HTILE;
        if(CollisionSegSeg(A,B,O,P))
        {
            return 1;
        }
        P.y -= HTILE;

    }
    return 0;
}


