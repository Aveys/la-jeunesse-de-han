#ifdef _cplusplus
#include <cstring>
#else
#include <strings.h>
#endif

#include "constantes.h"
#include "highscore.h"
#include <fmodex/fmod.h>


void highscore(SDL_Surface *ecran)
{
    FMOD_SYSTEM *system;
    FMOD_SOUND *musique;
    FMOD_System_Create(&system);
    FMOD_System_Init(system, 1, FMOD_INIT_NORMAL, NULL);
    FMOD_RESULT resultat;
    resultat = FMOD_System_CreateSound(system, "son/jeu.mp3", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &musique);
    FMOD_Sound_SetLoopCount(musique, -1);
    if (resultat != FMOD_OK)
    {
        fprintf(stderr, "Impossible de lire jeu.mp3\n");
        exit(EXIT_FAILURE);
    }
    FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE,musique, 0, NULL);
    //!Ataw-->
    FILE *scoreFile = fopen("score.txt", "rb");
    if(scoreFile == NULL)
    {
        fprintf(stderr, "Impossible d'ouvrir score.txt");
        exit(EXIT_FAILURE);
    }
    Texte pseudo[10], affScore[10];
    Score score[10];
    SDL_Surface *imageDeFond;
    SDL_Rect positionFond;
    positionFond.x=0;
    positionFond.y=0;
    for(int j = 0; j < 10; j++)
    {
        fread(&score[j], sizeof(Score), 1, scoreFile);
    }
    /*score = */tri_highscore(score, 10);
    for(int j = 0; j < 10; j++)
    {
        fprintf(stdout, "%i\n", score[j].tabscore);
    }
    fclose(scoreFile);
    if(ecran->w == 1024)
    {
        imageDeFond = IMG_Load("image/menu1024.bmp");
        SDL_BlitSurface(imageDeFond, 0, ecran, &positionFond);
        SDL_FreeSurface(imageDeFond);
    }
    else if (ecran->w == 800)
    {
        imageDeFond = IMG_Load("image/menu800.bmp");
        SDL_BlitSurface(imageDeFond, 0, ecran, &positionFond);
        SDL_FreeSurface(imageDeFond);
    }
    else
    {
        imageDeFond = IMG_Load("image/menu640.bmp");
        SDL_BlitSurface(imageDeFond, 0, ecran, &positionFond);
        SDL_FreeSurface(imageDeFond);
    }
    for(int i = 0; i < 10; i++)
    {
        afficheHighscore(score[i], ecran, pseudo[i], affScore[i], i);
    }
    SDL_Flip(ecran);

    SDL_Event event;
    bool done = false;
    while(!done)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            /*for(int i = 0; i < 10; i++)
              {
                SDL_FreeSurface(pseudo[i].titre);
                SDL_FreeSurface(affScore[i].titre);
              }*/
            SDL_Quit();
            TTF_Quit();
            exit(0);
            break;
        case SDL_KEYDOWN:
            /*for(int i = 0; i < 10; i++)
              {
                SDL_FreeSurface(pseudo[i].titre);
                SDL_FreeSurface(affScore[i].titre);
              }*/
            done = true;
            break;
        case SDL_JOYAXISMOTION:
            done = true;
            break;
        case SDL_JOYBUTTONDOWN:
            done = true;
            break;
        }
    }
    //!<--Ataw
    FMOD_Sound_Release(musique);
    FMOD_System_Close(system);
    FMOD_System_Release(system);

}


void sauvegardeScore(Score score)
{
    //!Ataw-->
    Score liste[10];
    bool insert = false;
    FILE *pfichier = fopen("score.txt","rb");
    if (pfichier != NULL)
    {
        for(int i = 0; !feof(pfichier); i++)
        {
            fread(&liste[i],sizeof(Score), 1, pfichier);
            if(score.tabscore > liste[i].tabscore)
                insert = true;
        }
        fclose(pfichier);
        tri_highscore(liste, 10);
        if(insert)
        {
            liste[9] = score;
            pfichier = fopen("score.txt", "wb");
            if(pfichier != NULL)
            {
                for(int i = 0; i < 10; i++)
                {
                    fwrite(&liste[i], sizeof(Score), 1, pfichier);
                }
                fclose(pfichier);
            }
        }

    }
    //!<--Ataw


}

void tri_highscore(Score s[], int nbJoueur)
{
    //!Ataw-->
    int i = 0, j;
    Score save;
    //algorithme de tri des scores.
    do
    {
        for(j = i; j < nbJoueur; j++)
        {
            if(s[i].tabscore < s[j].tabscore)
            {
                save = s[i];
                s[i] = s[j];
                s[j] = save;
            }
        }

        i++;
    }
    while(i != nbJoueur);

    //return s;
    //!<--Ataw
}

void afficheHighscore(Score scoreJoueur, SDL_Surface *ecran, Texte pseudo, Texte score, int i)
{
    TTF_Font *police = TTF_OpenFont("calibri.ttf", 18);
    SDL_Color couleur = {255, 0, 0};
    char titre[6];
    char scoreScore[5];
    strcpy(titre, scoreJoueur.pseudo);
    strcat(titre, " -");
    sprintf(scoreScore, "%i", scoreJoueur.tabscore);

    pseudo.titre = TTF_RenderText_Blended(police, titre, couleur);
    pseudo.pos.h = pseudo.titre->h;
    pseudo.pos.w = pseudo.titre->w;
    pseudo.pos.x = ecran->w/2 - (pseudo.pos.w + 5);
    pseudo.pos.y = ecran->h/4 + (i * (ecran->h/20));
    score.titre = TTF_RenderText_Blended(police, scoreScore, couleur);
    score.pos.h = score.titre->h;
    score.pos.w = score.titre->h;
    score.pos.x = ecran->w/2;
    score.pos.y = pseudo.pos.y;

    SDL_BlitSurface(pseudo.titre, 0, ecran, &pseudo.pos);
    SDL_BlitSurface(score.titre, 0, ecran, &score.pos);
    SDL_FreeSurface(pseudo.titre);
    SDL_FreeSurface(score.titre);
    TTF_CloseFont(police);
}

