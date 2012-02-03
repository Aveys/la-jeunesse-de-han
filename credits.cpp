#ifdef _cplusplus
#include <cstring>
#else
#include <string.h>
#endif
#include <fmodex/fmod.h>
#include "constantes.h"
#include "menu.h"

void initecran(SDL_Surface *ecran);

void credits(SDL_Surface *ecran)
{
    /* Initialisation de la musique utilisée dans les crédits avec la librairie FMOD */
    FMOD_SYSTEM *system;
    FMOD_SOUND *musique;
    FMOD_System_Create(&system);
    FMOD_System_Init(system, 1, FMOD_INIT_NORMAL, NULL);
    FMOD_RESULT resultat;
    SDL_Event event;
    resultat = FMOD_System_CreateSound(system, "son/dangerman.mp3", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &musique);
    FMOD_Sound_SetLoopCount(musique, -1);
    int temps_actuel,temps_precedent;
    if (resultat != FMOD_OK)
    {
        fprintf(stderr, "Impossible de lire jeu.mp3\n");
        exit(EXIT_FAILURE);
    }
    FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE,musique, 0, NULL);

    /* Initialisation de l'image de fond et du texte*/
    SDL_Surface *texte=NULL;
    SDL_Rect  positionTexte;
    TTF_Font *police20 = NULL;
    TTF_Font *police25 = NULL;
    TTF_Font *police29 = NULL;
    SDL_Color couleurVerte = {0, 255, 0};
    SDL_Color couleurBleu = {0, 198, 198};
    //SDL_EnableKeyRepeat(100,20);
    police20 = TTF_OpenFont("calibri.ttf", 20);
    police25 = TTF_OpenFont("calibri.ttf", 25);
    police29 = TTF_OpenFont("calibri.ttf", 29);


    initecran(ecran);

    texte=TTF_RenderText_Blended(police29, "Developpeurs :", couleurVerte);
    positionTexte.y = ecran->h/4+2*texte->h;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);
    texte=TTF_RenderText_Blended(police25, "Natacha Laborde", couleurBleu);
    positionTexte.y = ecran->h/4+3*texte->h+10;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);
    texte=TTF_RenderText_Blended(police25, "Mathieu Martin", couleurBleu);
    positionTexte.y = ecran->h/4+4*texte->h+10;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);
    texte=TTF_RenderText_Blended(police25, "Thomas Rovayaz", couleurBleu);
    positionTexte.y = ecran->h/4+5*texte->h+10;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);
    texte=TTF_RenderText_Blended(police25, "Nathanael Couret", couleurBleu);
    positionTexte.y = ecran->h/4+6*texte->h+10;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);
    texte=TTF_RenderText_Blended(police25, "Arthur Veys", couleurBleu);
    positionTexte.y = ecran->h/4+7*texte->h+10;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);

    SDL_Flip(ecran); // Affichage de la page développeurs

    //Attente
    temps_precedent = SDL_GetTicks();
    do
    {
        temps_actuel = SDL_GetTicks();
        while(SDL_PollEvent(&event));

    }
    while(temps_actuel-temps_precedent < 2000);
    initecran(ecran);

    texte=TTF_RenderText_Blended(police29, "Musique :", couleurVerte);
    positionTexte.y = ecran->h/4+2*texte->h;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);
    texte=TTF_RenderText_Blended(police25, "Star Wars Main Theme ", couleurBleu);
    positionTexte.y = ecran->h/4+3*texte->h+15;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);
    texte=TTF_RenderText_Blended(police20, "Original de John Williams", couleurBleu);
    positionTexte.y = ecran->h/4+4*texte->h+30;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);
    texte=TTF_RenderText_Blended(police20, "remix 8bit par NESsongsrus", couleurBleu);
    positionTexte.y = ecran->h/4+5*texte->h+30;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);

    SDL_Flip(ecran);

    temps_precedent = SDL_GetTicks();
    do
    {
        temps_actuel = SDL_GetTicks();
        while(SDL_PollEvent(&event));
    }
    while(temps_actuel-temps_precedent < 3000);
    // initecran(ecran);

    texte=TTF_RenderText_Blended(police25, "Imperial March 8bit ", couleurBleu);
    positionTexte.y = ecran->h/4+7*texte->h+20;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);
    texte=TTF_RenderText_Blended(police20, "Original de John Williams", couleurBleu);
    positionTexte.y = ecran->h/4+8*texte->h+55;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);
    texte=TTF_RenderText_Blended(police20, "Remix par 8bitsbrothers", couleurBleu);
    positionTexte.y = ecran->h/4+9*texte->h+55;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);

    SDL_Flip(ecran);

    temps_precedent = SDL_GetTicks();
    do
    {
        temps_actuel = SDL_GetTicks();
        while(SDL_PollEvent(&event));
    }
    while(temps_actuel-temps_precedent < 3000);

    texte=TTF_RenderText_Blended(police25, "Dangerman (Credits) par", couleurBleu);
    positionTexte.y = ecran->h/4+9*texte->h+60;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);
    texte=TTF_RenderText_Blended(police20, "Sleepytimejesse", couleurBleu);
    positionTexte.y = ecran->h/4+10*texte->h+110;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);

    SDL_Flip(ecran);

    temps_precedent = SDL_GetTicks();
    do
    {
        temps_actuel = SDL_GetTicks();
        while(SDL_PollEvent(&event));
    }
    while(temps_actuel-temps_precedent < 1500);

    texte=TTF_RenderText_Blended(police20, "Bruitages par", couleurBleu);
    positionTexte.y = ecran->h/4+11*texte->h+140;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);
    texte=TTF_RenderText_Blended(police20, "Arthur Veys", couleurBleu);
    positionTexte.y = ecran->h/4+12*texte->h+145;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);

    SDL_Flip(ecran);//affichage de la page "Musique"

    temps_precedent = SDL_GetTicks();
    do
    {
        temps_actuel = SDL_GetTicks();
        while(SDL_PollEvent(&event));
    }
    while(temps_actuel-temps_precedent < 3000);

    initecran(ecran);

    texte=TTF_RenderText_Blended(police29, "Images :", couleurVerte);
    positionTexte.y = ecran->h/4+2*texte->h;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);
    texte=TTF_RenderText_Blended(police20, "Dark Vador Pixel ", couleurBleu);
    positionTexte.y = ecran->h/4+3*texte->h+33;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);
    texte=TTF_RenderText_Blended(police20, " par Jose Eduardo Contreras Moral", couleurBleu);
    positionTexte.y = ecran->h/4+4*texte->h+33;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);
    texte=TTF_RenderText_Blended(police20, "http://bit.ly/yBVZH9 ", couleurBleu);
    positionTexte.y = ecran->h/4+5*texte->h+33;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);

    SDL_Flip(ecran);

    temps_precedent = SDL_GetTicks();
    do
    {
        temps_actuel = SDL_GetTicks();
        while(SDL_PollEvent(&event));
    }
    while(temps_actuel-temps_precedent < 3000);

    texte=TTF_RenderText_Blended(police20, "Han Solo pixel", couleurBleu);
    positionTexte.y = ecran->h/4+6*texte->h+40;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);
    texte=TTF_RenderText_Blended(police20, " par Anders, a.k.a. El Ducko", couleurBleu);
    positionTexte.y = ecran->h/4+7*texte->h+40;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);
    texte=TTF_RenderText_Blended(police20, "http://bit.ly/yfvWZ9", couleurBleu);
    positionTexte.y = ecran->h/4+8*texte->h+40;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);

    SDL_Flip(ecran);

    temps_precedent = SDL_GetTicks();
    do
    {
        temps_actuel = SDL_GetTicks();
        while(SDL_PollEvent(&event));
    }
    while(temps_actuel-temps_precedent < 3000);

    texte=TTF_RenderText_Blended(police20, "Dropship", couleurBleu);
    positionTexte.y = ecran->h/4+9*texte->h+50;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);
    texte=TTF_RenderText_Blended(police20, " par ~robotriot", couleurBleu);
    positionTexte.y = ecran->h/4+10*texte->h+50;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);
    texte=TTF_RenderText_Blended(police20, "http://bit.ly/y7morb", couleurBleu);
    positionTexte.y = ecran->h/4+11*texte->h+50;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);

    SDL_Flip(ecran);

    temps_precedent = SDL_GetTicks();
    do
    {
        temps_actuel = SDL_GetTicks();
        while(SDL_PollEvent(&event));
    }
    while(temps_actuel-temps_precedent < 3000);
    texte=TTF_RenderText_Blended(police20, "Observation Deck", couleurBleu);
    positionTexte.y = ecran->h/4+12*texte->h+60;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);
    texte=TTF_RenderText_Blended(police20, " par *z-design", couleurBleu);
    positionTexte.y = ecran->h/4+13*texte->h+60;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);
    texte=TTF_RenderText_Blended(police20, "http://bit.ly/x9eoIU", couleurBleu);
    positionTexte.y = ecran->h/4+14*texte->h+60;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);

    SDL_Flip(ecran);

    temps_precedent = SDL_GetTicks();
    do
    {
        temps_actuel = SDL_GetTicks();
        while(SDL_PollEvent(&event));
    }
    while(temps_actuel-temps_precedent < 3000);

    initecran(ecran);

    texte=TTF_RenderText_Blended(police29, "Remerciements :", couleurVerte);
    positionTexte.y = ecran->h/4+2*texte->h-5;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);
    texte=TTF_RenderText_Blended(police20, "Georges Lucas pour son univers", couleurBleu);
    positionTexte.y = ecran->h/4+3*texte->h+20;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);

    SDL_Flip(ecran);

    temps_precedent = SDL_GetTicks();
    do
    {
        temps_actuel = SDL_GetTicks();
        while(SDL_PollEvent(&event));
    }
    while(temps_actuel-temps_precedent < 1500);

    texte=TTF_RenderText_Blended(police20, "Ludovic Skybavay pour son aide precieuse", couleurBleu);
    positionTexte.y = ecran->h/4+4*texte->h+20;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);

    SDL_Flip(ecran);

    temps_precedent = SDL_GetTicks();
    do
    {
        temps_actuel = SDL_GetTicks();
        while(SDL_PollEvent(&event));
    }
    while(temps_actuel-temps_precedent < 1500);

    texte=TTF_RenderText_Blended(police20, "Skype pour nos nuits endiablees de code", couleurBleu);
    positionTexte.y = ecran->h/4+5*texte->h+20;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);

    SDL_Flip(ecran);

    temps_precedent = SDL_GetTicks();
    do
    {
        temps_actuel = SDL_GetTicks();
        while(SDL_PollEvent(&event));
    }
    while(temps_actuel-temps_precedent < 1500);

    texte=TTF_RenderText_Blended(police20, "Le departement INFO de l'IUT d'Annecy", couleurBleu);
    positionTexte.y = ecran->h/4+6*texte->h+20;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);

    SDL_Flip(ecran);

    temps_precedent = SDL_GetTicks();
    do
    {
        temps_actuel = SDL_GetTicks();
        while(SDL_PollEvent(&event));
    }
    while(temps_actuel-temps_precedent < 1300);

    texte=TTF_RenderText_Blended(police20, "Jeremy Flaman pour l'inspiration", couleurBleu);
    positionTexte.y = ecran->h/4+7*texte->h+100;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);
    texte=TTF_RenderText_Blended(police20, "humoristique de ce credits", couleurBleu);
    positionTexte.y = ecran->h/4+8*texte->h+100;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);

    SDL_Flip(ecran);//affichage de la page "Remerciements"

    temps_precedent = SDL_GetTicks();
    do
    {
        temps_actuel = SDL_GetTicks();
        while(SDL_PollEvent(&event));
    }
    while(temps_actuel-temps_precedent < 3000);

    initecran(ecran);

    texte=TTF_RenderText_Blended(police29, "Les sites Source :", couleurVerte);
    positionTexte.y = ecran->h/4+2*texte->h-5;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);
    texte=TTF_RenderText_Blended(police20, "Le Site du zero : SDL ", couleurBleu);
    positionTexte.y = ecran->h/4+3*texte->h+20;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);

    SDL_Flip(ecran);

    temps_precedent = SDL_GetTicks();
    do
    {
        temps_actuel = SDL_GetTicks();
        while(SDL_PollEvent(&event));
    }
    while(temps_actuel-temps_precedent < 1500);

    texte=TTF_RenderText_Blended(police20, "Developpez.com", couleurBleu);
    positionTexte.y = ecran->h/4+4*texte->h+20;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);

    SDL_Flip(ecran);

    temps_precedent = SDL_GetTicks();
    do
    {
        temps_actuel = SDL_GetTicks();
        while(SDL_PollEvent(&event));
    }
    while(temps_actuel-temps_precedent < 1500);

    texte=TTF_RenderText_Blended(police20, "ChiptuneUp! pour FamiTracker(Tracker SNES)", couleurBleu);
    positionTexte.y = ecran->h/4+5*texte->h+20;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);

    SDL_Flip(ecran);

    temps_precedent = SDL_GetTicks();
    do
    {
        temps_actuel = SDL_GetTicks();
        while(SDL_PollEvent(&event));
    }
    while(temps_actuel-temps_precedent < 1500);

    texte=TTF_RenderText_Blended(police20, "Dark Maul,Dark Vador, Palpatine (*joke*)", couleurBleu);
    positionTexte.y = ecran->h/4+6*texte->h+20;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);

    SDL_Flip(ecran);//affichage de la page "Sites sources"

    temps_precedent = SDL_GetTicks();
    do
    {
        temps_actuel = SDL_GetTicks();
        while(SDL_PollEvent(&event));
    }
    while(temps_actuel-temps_precedent < 3000);
    initecran(ecran);

    texte=TTF_RenderText_Blended(police29, "Jeu fait avec amour", couleurBleu);
    positionTexte.y = ecran->h/4+2*texte->h+50;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);
    texte=TTF_RenderText_Blended(police29, "pour la semaine speciale AP2 ", couleurBleu);
    positionTexte.y = ecran->h/4+3*texte->h+50;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);
    texte=TTF_RenderText_Blended(police29, "et nos joueurs !", couleurBleu);
    positionTexte.y = ecran->h/4+4*texte->h+50;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    SDL_FreeSurface(texte);

    SDL_Flip(ecran);//affichage de la derniere page de "remerciement"

    temps_precedent = SDL_GetTicks();
    do
    {
        temps_actuel = SDL_GetTicks();
        while(SDL_PollEvent(&event));
    }
    while(temps_actuel-temps_precedent < 4650);


    TTF_CloseFont(police20);
    TTF_CloseFont(police25);
    TTF_CloseFont(police29);

    FMOD_Sound_Release(musique);
    FMOD_System_Close(system);
    FMOD_System_Release(system);
    //SDL_Flip(ecran);

}

void initecran(SDL_Surface *ecran)
{
    SDL_Surface *imageDeFond = NULL,*texte=NULL;
    SDL_Rect  positionFond,positionTexte;
    positionFond.x=0;
    positionFond.y=0;
    TTF_Font *police = NULL;
    SDL_Color couleurRouge = {255, 0, 0};
    police = TTF_OpenFont("calibri.ttf", 32);
    texte=TTF_RenderText_Blended(police, "Credits", couleurRouge);

    // selection de l'image selon la résolution
    if(ecran->w == 1024)
    {
        imageDeFond = IMG_Load("image/menu1024.bmp");
        SDL_BlitSurface(imageDeFond, 0, ecran, &positionFond);
    }
    else if (ecran->w == 800)
    {
        imageDeFond = IMG_Load("image/menu800.bmp");
        SDL_BlitSurface(imageDeFond, 0, ecran, &positionFond);
    }
    else
    {
        imageDeFond = IMG_Load("image/menu640.bmp");
        SDL_BlitSurface(imageDeFond, 0, ecran, &positionFond);
    }
    positionTexte.y = ecran->h/4+texte->h-10;
    positionTexte.x = ecran->w/2-texte->w/2;
    SDL_BlitSurface(texte,0,ecran,&positionTexte);
    //SDL_Flip(ecran);//affichage crédits + fond
    SDL_FreeSurface(imageDeFond);
    SDL_FreeSurface(texte);
    TTF_CloseFont(police);
}
