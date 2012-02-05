#ifdef _cplusplus
#include <cstring>
#else
#include <string.h>
#endif

#include "constantes.h"
#include "menu.h"
#include "editeur.h"
#include "reglages.h"
#include "credits.h"
#include "jeu.h"
#include "highscore.h"
#include "credits.h"
#include <fmodex/fmod.h>

int menu(SDL_Surface *ecran,SDL_Joystick *joystick)
{
    //chargements de fmod et de la musique de fond
    FMOD_SYSTEM *system;
    FMOD_SOUND *musique;
    FMOD_System_Create(&system);
    FMOD_System_Init(system, 1, FMOD_INIT_NORMAL, NULL);
    FMOD_RESULT resultat;
    resultat = FMOD_System_CreateSound(system, "son/menu.mp3", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &musique);
    FMOD_Sound_SetLoopCount(musique, -1);
    if (resultat != FMOD_OK)
    {
        fprintf(stderr, "Impossible de lire menu.mp3\n");
        exit(EXIT_FAILURE);
    }
    FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE,musique, 0, NULL);
    FMOD_CHANNELGROUP *canal;
    //!fin fmod
    //graphique
    SDL_Surface *imageDeFond = NULL;
    SDL_Rect  positionFond;
    positionFond.x=0;
    positionFond.y=0;
    Map carteJeu;
    carteJeu.level = 0;
    initMap(&carteJeu);

    ParamMap options;
    options.afficheGrille = 1;
    options.editeur = 0;
    strcpy(options.filenameGrille,"image/editeur/tools/grille32.bmp");
    chargeMap(&carteJeu);
    //MatHack:On enleve le curseur, comme sa utilisateur cherchera pas a cliquer sur le menu
    SDL_ShowCursor(0);

    //! fin graphique

    bool done = false;
    SDL_Event event;

    Bouton menu[6];
    int bActif = 0, i;
    int touche_enfonce = 0;
    FILE *textMenu = NULL;
    // boucle principale du menu (et du programme)
    while (!done)
    {
        SDL_FillRect(ecran, 0, 0xFFFFFF);

        textMenu = fopen("textMenu.txt", "rb");
        if(textMenu == NULL)
        {
            fprintf(stderr, "Impossible d'ouvrir menu.txt\n");
            return 1;
        }
        SDL_Color blanc = {255, 255, 255};
        SDL_Color rouge = {255, 0, 0};
        //selon la résolution on affiche la bonne image de fond
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

        for(i = 0; i < 6 && !feof(textMenu); i++)
        {
            //on affiche les boutons avec un design différent pour le bouton actif

            if(bActif == i)
            {

                menuBouton(&menu[i], ecran, rouge, textMenu, true, i);
            }
            else
            {
                menuBouton(&menu[i], ecran, blanc, textMenu, false, i);
            }

        }
        fclose(textMenu);
        //Affiche la versin en cour
        versionTexte();
        SDL_Flip(ecran);

        //on vérifie si un évènement a eu lieu
        SDL_PollEvent(&event);
        // on vérifie l'évènement

        if(SDL_NumJoysticks() > 0)
        {
            if (event.jaxis.axis == 0 && event.jaxis.value < -3200)
            {
                // Vers la gauche
                done = true;
            }
            else if (event.jaxis.axis == 0 && event.jaxis.value > 3200)
            {
                // Vers la droite
                if(bActif!=1)
                {
                    FMOD_System_GetMasterChannelGroup(system, &canal);
                    FMOD_ChannelGroup_Stop(canal);
                }
                //si lanceprogramme retourne true (quitter) on sort de la boucle
                if(lanceProgramme(bActif, ecran,&carteJeu, options,joystick))
                    done = true;
                else
                    event.type = 0;
                if(bActif!=1)
                    FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE,musique, 0, NULL);
            }
            else if (event.jaxis.axis == 1 && event.jaxis.value < -3200)
            {
                // Vers le haut
                if(bActif > 0)
                    bActif--;
                else
                    bActif = 5;
            }
            else if (event.jaxis.axis == 1 && event.jaxis.value > 3200)
            {
                // Vers le bas
                if(bActif < 5)
                    bActif++;
                else
                    bActif = 0;
            }
        }
        switch (event.type)
        {
            // on quitte si l'utilisateur tente de fermer la fenetre
        case SDL_QUIT:
            done = true;
            break;
            //sinon selon la touche enfoncée
        case SDL_KEYDOWN:
            // on quitte si l'utilisateur clique sur echap
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                done = true;
                break;
                //si on appuie sur la touche du haut on décrémente l'indice du menu ou on le replace en bas
            case SDLK_UP:
                if(touche_enfonce == 0)
                {
                    if(bActif > 0)
                        bActif--;
                    else
                        bActif = 5;
                    touche_enfonce = 1;
                }
                break;
                //si on appuie sur la touche du bas on incrémente l'indice du menu ou le replace en haut
            case SDLK_DOWN:
                if(touche_enfonce == 0)
                {
                    if(bActif < 5)
                        bActif++;
                    else
                        bActif = 0;
                    touche_enfonce = 1;
                }
                break;
                //si on appuie sur entrée
            case SDLK_RETURN:
                if(bActif!=1)
                {
                    FMOD_System_GetMasterChannelGroup(system, &canal);
                    FMOD_ChannelGroup_Stop(canal);
                }
                //si lanceprogramme retourne true (quitter) on sort de la boucle
                if(lanceProgramme(bActif, ecran,&carteJeu, options,joystick))
                    done = true;
                else
                    event.type = 0;
                if(bActif!=1)
                    FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE,musique, 0, NULL);

                break;
            default:
                break;
            }
            break;
        case SDL_KEYUP:
            if(event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN)
                touche_enfonce = 0;
            break;
        } // fin switch

    } // fin du while principale
    FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE,musique, 0, NULL);
    FMOD_Sound_Release(musique);
    FMOD_System_Close(system);
    FMOD_System_Release(system);

    return 0;
}

void menuBouton(Bouton * bouton, SDL_Surface *ecran, SDL_Color couleurTexte, FILE *fichier, bool actif, int nb)
{
    //MatHack: Permet de faire un style [****]
    char titre[100];
    int tailleFont;
    strcpy(titre, "");
    if(actif)
    {
        strcpy(titre, ""); //Nettoye la chaine
        strcat(titre, "[");
        strcat(titre, bouton->titre);
        strcat(titre, "]");
        tailleFont = 28;
    }
    else
    {
        //MatHack:Si non on met la chaine normal
        strcpy(titre, bouton->titre);
        tailleFont = 18;
    }


    //cette fonction initialise les boutons en colonne (*nb pour les décaler)
    bouton->police = TTF_OpenFont("calibri.ttf", tailleFont);
    fread(bouton->titre, sizeof(char), sizeof(bouton->titre), fichier);


    bouton->texte = TTF_RenderText_Blended(bouton->police, titre, couleurTexte);

    bouton->pos.x = (ecran->w/2) - bouton->pos.w/2;
    bouton->pos.y = (ecran->w/4 - 25) + (nb * ecran->h/10);

    //MatHack: Désactive le fond rouge
    /*if(!actif)
        SDL_FillRect(bouton->fond, 0, 0xFF0000);
    else
        SDL_FillRect(bouton->fond, 0, 0x0000FF);*/
    SDL_BlitSurface(bouton->texte, 0, ecran, &bouton->pos);
    SDL_FreeSurface(bouton->texte);
    TTF_CloseFont(bouton->police);
}


bool lanceProgramme(int bActif, SDL_Surface *ecran,Map *carteJeu,ParamMap options,SDL_Joystick *joystick)
{
    switch(bActif)
    {
        //selon le bouton actif on lance le programme correspondant
    case 0:
        jeu(ecran,*carteJeu, options,joystick); //jouer
        break;
    case 1:
        reglages(ecran,joystick); //réglages
        break;
    case 2:
        editeur(ecran, carteJeu, &options,joystick); //éditeur de map
        break;
    case 3:
        return true; //quitter
        break;
    case 4:
        credits(ecran); //menu crédits
        break;
    case 5:
        highscore(ecran); //menu highscore
        break;
    default:
        return false;
        break;
    }
    return false;
}
