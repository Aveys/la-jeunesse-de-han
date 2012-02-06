#include "jeu.h"
#include "ennemie.h"
#include <math.h>
/*
    initialise la structure Ennemi grace au donnee des positions des ennemis de la map
*/
void initEnnemi(Map carte, Ennemi *enne,int i,SDL_Surface* screen)
{
    enne->EnnemiSprite = IMG_Load("image/perso/ennemieD.bmp");
    enne->positionE.x = carte.posDepEnnemi[i].x - enne->EnnemiSprite->w;
    enne->positionE.y = carte.posDepEnnemi[i].y - enne->EnnemiSprite->h;
    SDL_BlitSurface(enne->EnnemiSprite, NULL, screen, &enne->positionE);
    enne->positionE.w = enne->EnnemiSprite->w;
    enne->positionE.h = enne->EnnemiSprite->h;
    enne->sensye = 2;
    enne->sensxe = 0;
    enne->balleEnnemi.positionB.x = 0;
    enne->balleEnnemi.positionB.y = 0;
    enne->balleEnnemi.imageBalle = (char*)"image/perso/laser.bmp";
    enne->balleEnnemi.balleSprite = IMG_Load(enne->balleEnnemi.imageBalle);
}
/*
    deplace et gere l'ennemi dans la map
*/
void deplace_ennemi(Ennemi *ennemi1,Map carte,SDL_Surface* screen,Perso perso1)
{
    ennemi1->positionE.x += ennemi1->sensxe;
    ennemi1->positionE.y += ennemi1->sensye;
    if(ennemi1->sensye != 0)
    {
        if(test_collision(ennemi1->positionE,&carte,1,1,NULL,NULL,2) == ATTERI) //test si le mob a bien atteri avant de bouger
        {
            ennemi1->sensye = 0;    //si oui alors il bouge plus que en X
            ennemi1->sensxe = 2;
        }
    }
    if(test_collision(ennemi1->positionE,&carte,3,1,NULL,NULL,2) != 0) //si le mob est en collision avec un element du decor alors il fait demi tour
    {
        //MatHack:Deux sens pour l'ennemie
        if(ennemi1->sensxe < 1)
        {
            SDL_FreeSurface(ennemi1->EnnemiSprite);
            ennemi1->EnnemiSprite = IMG_Load("image/perso/ennemieD.bmp");
            associer_surface_pos(ennemi1->EnnemiSprite,&ennemi1->positionE);
        }
        else if(ennemi1->sensxe > 1)
        {
            SDL_FreeSurface(ennemi1->EnnemiSprite);
            ennemi1->EnnemiSprite = IMG_Load("image/perso/ennemieG.bmp");
            associer_surface_pos(ennemi1->EnnemiSprite,&ennemi1->positionE);
        }

        ennemi1->positionE.x -= ennemi1->sensxe;
        ennemi1->sensxe *= -1;
    }
    //test pour ne pas tomber dans un trou
    ennemi1->positionE.y += 10;    //position pour tester la case suivante
    ennemi1->positionE.x += ennemi1->sensxe;
    if(test_collision(ennemi1->positionE,&carte,1,1,NULL,NULL,2) != ATTERI)    //si il n'y a pas de mur en dessous alors il fait demi tour
    {
        ennemi1->positionE.x -= ennemi1->sensxe;
        ennemi1->sensxe *= -1;
    }
    ennemi1->positionE.y -= 10;
    ennemi1->positionE.x -= ennemi1->sensxe;
    /*SDL_Rect pos_ecran;
    pos_ecran.x = carte.xscroll;
    pos_ecran.y = carte.yscroll;
    pos_ecran.w = screen->w;
    pos_ecran.h = screen->h;
    fprintf(stderr,"%i,%i,%i,%i\n",pos_ecran.x,pos_ecran.y,pos_ecran.w,pos_ecran.h);
    fprintf(stderr,"%i,%i,%i,%i\n",ennemi1->balleEnnemi.positionB.x,ennemi1->balleEnnemi.positionB.y,ennemi1->balleEnnemi.positionB.w,ennemi1->balleEnnemi.positionB.h);
    if(collision_bloque(ennemi1->balleEnnemi.positionB,pos_ecran) != 1 || test_collision(ennemi1->balleEnnemi.positionB,&carte,0,0,NULL,&perso1) != 0)
    {
        ennemi1->balleEnnemi.x = 0;
        ennemi1->balleEnnemi.y = 0;
    }
    if(collision_bloque(ennemi1->positionE,pos_ecran) == 1)
        ennemi_tire(ennemi1,perso1,carte,screen);*/
    affiche_ennemi(*ennemi1,screen,carte);
}
void affiche_ennemi(Ennemi ennemi1,SDL_Surface* screen,Map carte)
{
    ennemi1.positionE.x -= carte.xscroll;
    ennemi1.positionE.y -= carte.yscroll;
    SDL_BlitSurface(ennemi1.EnnemiSprite, NULL, screen, &ennemi1.positionE);
}
void ennemi_tire(Ennemi *ennemie1,Perso perso1,Map carte,SDL_Surface *screen)
{
    if(ennemie1->balleEnnemi.x == 0 && ennemie1->balleEnnemi.y == 0)
    {
        perso1.positionP.x += carte.xscroll;
        perso1.positionP.y += carte.yscroll;
        ennemie1->balleEnnemi.x = (double)ennemie1->positionE.x - carte.xscroll;
        ennemie1->balleEnnemi.y = (double)ennemie1->positionE.y - carte.yscroll + 10;
        double distance = sqrt((ennemie1->positionE.y-perso1.positionP.y)*(ennemie1->positionE.y-perso1.positionP.y)+(ennemie1->positionE.x-perso1.positionP.x)*(ennemie1->positionE.x-perso1.positionP.x));
        if(perso1.positionP.x - ennemie1->positionE.x < 0)
        {
            ennemie1->balleEnnemi.vitesseX = (perso1.positionP.x - ennemie1->positionE.x)/distance;
            while(abs(ennemie1->balleEnnemi.vitesseX) <= abs(ennemie1->sensxe)+1)
                ennemie1->balleEnnemi.vitesseX--;
        }
        else if(perso1.positionP.x - ennemie1->positionE.x > 0)
        {
            ennemie1->balleEnnemi.vitesseX = (perso1.positionP.x - ennemie1->positionE.x)/distance;
            while(abs(ennemie1->balleEnnemi.vitesseX) <= abs(ennemie1->sensxe)+1)
                ennemie1->balleEnnemi.vitesseX++;
        }
        else
            ennemie1->balleEnnemi.vitesseX = 0;
        if(perso1.positionP.y - ennemie1->positionE.y > 0)
            ennemie1->balleEnnemi.vitesseY = (perso1.positionP.y - ennemie1->positionE.y)/distance;
        else if(perso1.positionP.y - ennemie1->positionE.y < 0)
            ennemie1->balleEnnemi.vitesseY = (perso1.positionP.y - ennemie1->positionE.y)/distance;
        else
            ennemie1->balleEnnemi.vitesseY = 0;
    }
    if(ennemie1->balleEnnemi.vitesseX != 0 && ennemie1->balleEnnemi.vitesseY != 0)
    {
        ennemie1->balleEnnemi.x += ennemie1->balleEnnemi.vitesseX;
        ennemie1->balleEnnemi.y += ennemie1->balleEnnemi.vitesseY;
        ennemie1->balleEnnemi.positionB.x = (int)ennemie1->balleEnnemi.x;
        ennemie1->balleEnnemi.positionB.y = (int)ennemie1->balleEnnemi.y;
        SDL_BlitSurface(ennemie1->balleEnnemi.balleSprite, NULL, screen, &ennemie1->balleEnnemi.positionB);
    }
}
/*
    test de collision entre le perso les ennemis et les blasters
*/
void test_collision_ennemi(Ennemi **ennemi1,Perso *perso1,Balle **mario_tire,int *nbr_balle,Map *carte,int *a_atteri,int *continuer,Joueur *joueur,SDL_Surface *screen)
{
    int i,j,k,retour;
    //boucle pour tester tt les ennemis
    for(i = 0; i < carte->nb_ennemi; i++)
    {
        //test collision ennemi et perso
        ennemi1[0][i].positionE.x -= carte->xscroll;   //reajustement par rapport au scrolling
        ennemi1[0][i].positionE.y -= carte->yscroll+5;
        retour = collision_bloque(ennemi1[0][i].positionE,perso1->positionP);
        if(retour != 0)    //test collision entre bloc perso et bloc ennemi
        {
            ennemi1[0][i].positionE.x += carte->xscroll;
            ennemi1[0][i].positionE.y += carte->yscroll+5;
            if(perso1->sensyp > 0 && (retour == 1 || retour == 2))
            {
                joueur->score += 20;
                for(k = i+1; k < (carte->nb_ennemi); k++)
                {
                    ennemi1[0][k-1] = ennemi1[0][k];  //deplacement du tableau pour ecraser l'ennemi a supprimer
                }
                carte->nb_ennemi--;
                ennemi1[0] = (Ennemi*)realloc(ennemi1[0],(carte->nb_ennemi)*sizeof(Ennemi));    //reallocation du tab d'ennemi
                i--;
            }
            else
            {
                //et on replace le perso en lui enlevant un vie
                initPosPerso(perso1,carte,screen);
                *a_atteri = 1;
                if(joueur->nbvie == 0)
                    *continuer = 0;
                joueur->nbvie--;
            }
        }
        else
        {
            ennemi1[0][i].positionE.x += carte->xscroll;
            ennemi1[0][i].positionE.y += carte->yscroll+5;
            //boucle pour tester les blasters par rapport aux ennemis
            for(j = 0; j < (*nbr_balle); j++)
            {
                ennemi1[0][i].positionE.x -= carte->xscroll;   //reajustement par rapport au scrolling
                ennemi1[0][i].positionE.y -= carte->yscroll;
                if(collision_bloque(mario_tire[0][j].positionB,ennemi1[0][i].positionE) != 0)  //test de collision entre le bloc ennemi et le bloc blaster
                {
                    joueur->score += 20;
                    for(k = i+1; k < (carte->nb_ennemi); k++)
                    {
                        ennemi1[0][k-1] = ennemi1[0][k];  //deplacement du tableau pour ecraser l'ennemi a supprimer
                    }
                    carte->nb_ennemi--;
                    ennemi1[0] = (Ennemi*)realloc(ennemi1[0],(carte->nb_ennemi)*sizeof(Ennemi));    //reallocation du tab d'ennemi
                    i--;

                    for(k = j+1; k < (*nbr_balle); k++)
                    {
                        mario_tire[0][k-1] = mario_tire[0][k];    //deplacement du tableau pour ecraser la balle a supprimer
                    }
                    (*nbr_balle)--;
                    mario_tire[0] = (Balle*)realloc(mario_tire[0],(*nbr_balle)*sizeof(Balle)); // reallocation du tableau de blaster
                    j--;
                }
                else
                {
                    ennemi1[0][i].positionE.x += carte->xscroll;   //retour aux vraies valeurs
                    ennemi1[0][i].positionE.y += carte->yscroll;
                }
            }
        }
    }
}
/*
    test de collision entre 2 rectangles
*/
int collision_bloque(SDL_Rect bloc1,SDL_Rect bloc2)
{
    //si le point en bas a droite est dans le bloc 2 alors return 1
    if(bloc1.x+bloc1.w >= bloc2.x && bloc1.x+bloc1.w <= bloc2.x+bloc2.w)
        if(bloc1.y+bloc1.h >= bloc2.y && bloc1.y+bloc1.h <= bloc2.y+bloc2.h)
            return 4;
    //si le point en bas a gauche est dans le bloc 2 alors return 1
    if(bloc1.x >= bloc2.x && bloc1.x <= bloc2.x+bloc2.w)
        if(bloc1.y+bloc1.h >= bloc2.y && bloc1.y+bloc1.h <= bloc2.y+bloc2.h)
            return 3;
    //si le point en haut a droite est dans le bloc 2 alors return 1
    if(bloc1.x+bloc1.w >= bloc2.x && bloc1.x+bloc1.w <= bloc2.x+bloc2.w)
        if(bloc1.y >= bloc2.y && bloc1.y <= bloc2.y+bloc2.h)
            return 2;
    //si le point en haut a gauche est dans le bloc 2 alors return 1
    if(bloc1.x >= bloc2.x && bloc1.x <= bloc2.x+bloc2.w)
        if(bloc1.y >= bloc2.y && bloc1.y <= bloc2.y+bloc2.h)
            return 1;
    return 0;
}
