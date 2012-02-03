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

/*void efface_perso(Perso perso1,SDL_Surface* screen)
{
    SDL_Rect pos = perso1.positionP;
    SDL_Surface *masque = IMG_Load("image/mario_masque.bmp");
    pos.x -= perso1.sensxp;
    pos.y -= perso1.sensyp;
    SDL_BlitSurface(masque, NULL, screen, &pos);
    SDL_FreeSurface(masque);
}*/

void gestion_touche(Perso *perso1,int *continuer,int *a_atteri)
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
             *continuer = 0;
              break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_DOWN:
                perso1->sensyp = 1;
                break;
            case SDLK_UP:
                if(*a_atteri == 0)
                    perso1->sensyp = -4;
                *a_atteri = 1;
                break;
            case SDLK_SPACE:
                if(*a_atteri == 0)
                    perso1->sensyp = -4;
                *a_atteri = 1;
                break;
            case SDLK_RIGHT:
                if(*a_atteri == 0)
                    perso1->sensxp = 2;
                break;
            case SDLK_LEFT:
                if(*a_atteri == 0)
                    perso1->sensxp = -2;
                break;
            case SDLK_ESCAPE:
                *continuer = 0;
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}

void deplace_mario(Perso *perso1,Map *carte,int decalage,SDL_Surface* screen,int *a_atteri)
{
    perso1->positionP.x += perso1->sensxp;
    perso1->positionP.y += perso1->sensyp;
    int retour_test = test_collision(*perso1,*carte);
    if(scrollingPersoX(*perso1, carte, screen) == 0)
    {
        perso1->positionP.x -= perso1->sensxp;
    }
    if(scrollingPersoY(*perso1, carte, screen) == 0)
    {
        perso1->positionP.y -= perso1->sensyp;
    }
    else
    {
        if(retour_test == MUR_COLLISION)
        {
            perso1->positionP.x -= perso1->sensxp;
            if(*a_atteri == 1)
            {
                perso1->sensxp = 0;
                perso1->sensyp = 1;
            }
        }
        else if(retour_test == ATTERI)
        {
            perso1->positionP.x -= perso1->sensxp;
            *a_atteri = 0;
        }
    }
    if(retour_test == MUR_COLLISION)
    {
        perso1->positionP.y -= perso1->sensyp;
        perso1->sensyp = 1;
        if(*a_atteri == 1)
            perso1->sensxp = 0;
    }
    else if(retour_test == ATTERI)
    {
        perso1->positionP.y -= perso1->sensyp;
        perso1->sensyp = 0;
        *a_atteri = 0;
    }
    else if(retour_test == FIN_MAP)
    {
        carte->xscroll = 0;
        carte->yscroll = 0;
        perso1->positionP.x = carte->posDepart.x-(perso1->persoSprite->w/2);
        perso1->positionP.y = carte->posDepart.y-perso1->persoSprite->h;
    }
    else
    {
        if(perso1->sensyp > -1)
            perso1->sensyp = 1;
    }
}
void position_perso_tab(Perso perso1,int *posxmin,int *posymin,int *posxmax,int *posymax,Map carte)
{
    *posxmin = (perso1.positionP.x + carte.xscroll + LTILE)/LTILE;
    *posymin = (perso1.positionP.y + LTILE)/HTILE;
    *posxmax = (perso1.positionP.x + carte.xscroll + perso1.positionP.w + LTILE)/LTILE;
    *posymax = (perso1.positionP.y + carte.yscroll + perso1.positionP.h + HTILE)/HTILE;
}

int test_collision(Perso perso1,Map carte)
{
    int posxmin,posymin,posxmax,posymax,i,j;
    position_perso_tab(perso1,&posxmin,&posymin,&posxmax,&posymax,carte);
    for(i = posymin; i <= posymax; i++)
    {
        for(j = posxmin; j <= posxmax; j++)
        {
            if(carte.tableauMap[i-1][j-1] != 0)
            {
                switch(carte.tableauMap[i-1][j-1])
                {
                    case 1:
                        if(i == posymax)
                            return ATTERI;
                        return MUR_COLLISION;
                        break;
                    case 3:
                        return BONUS;
                        break;
                    case 4:
                        return TROU;
                        break;
                    case 7:
                        if(i == posymax)
                            return ATTERI;
                        return MUR_COLLISION;
                        break;
                }
            }
            if(i <= 0 || j <= 0)
            {
                return MUR_COLLISION;
            }
            if(i > carte.MONDE_HAUTEUR - 1|| j > carte.MONDE_LARGEUR - 1)
                return FIN_MAP;
        }
    }
    return 0;
}
