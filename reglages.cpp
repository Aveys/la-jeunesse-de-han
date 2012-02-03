/************************
* reglages.cpp          *
* reglages graphique    *
* modifie le 25/01/12   *
************************/

#ifdef _cplusplus
#include <cstring>
#else
#include <string.h>
#endif

#include "constantes.h"
#include "menu.h"
#include "reglages.h"

void reglages(SDL_Surface *ecran,SDL_Joystick *joystick)
{

    SDL_Surface *imageDeFond = NULL,*texte=NULL;
    SDL_Rect  positionFond,positionTexte;
    positionFond.x=0;
    positionFond.y=0;

    TTF_Font *police = NULL;

    //!Ataw-->
    FILE *config;
    config = fopen("config.txt", "r");
    if(config != NULL)
    {
        bool done = false;
        SDL_Event event;

        Fenetre fenetre;
        int nbReso;
        char *poubelle;
        poubelle = (char*)malloc(sizeof(char)*30);

        //on récupère les informations sur la fenetre dans config.txt
        fscanf(config, "%s = %i\n", poubelle, &fenetre.width);
        fscanf(config, "%s = %i\n", poubelle, &fenetre.height);
        fscanf(config, "%s = %i\n", poubelle, &fenetre.couleur);
        fscanf(config, "%s = %i", poubelle, &fenetre.fullscreen);
        free(poubelle);
        poubelle = NULL;
        fclose(config);

        //on définit l'indice de résolution d'écran
        if(fenetre.width == 640)
            nbReso = 1;
        else if(fenetre.width == 800)
            nbReso = 2;
        else
            nbReso = 3;
        SDL_Color couleurRouge = {255, 0, 0};
        police = TTF_OpenFont("calibri.ttf", 32);
        texte=TTF_RenderText_Blended(police, "Réglages", couleurRouge);
        positionTexte.y = ecran->h/4+50;
        positionTexte.x = ecran->w/2-texte->w/2;

        //bouton de la résolution d'écran
        Bouton resolution;
        //bouton du mode d'image
        Bouton fullscreen;
        //bouton retour
        Bouton retour;
        strcpy(retour.titre, "Retour");

        SDL_Color rougeActif = {255, 0, 0};
        int nbMenu = 0; //indice du menu

        while(!done)
        {
            SDL_putenv("SDL_VIDEO_CENTERED=center");
            resolution.pos.y = ecran->h/2;
            fullscreen.pos.y = ecran->h/2+ecran->h/10;
            retour.pos.y = ecran->h/2+ecran->h/5;
            //selon la résolution on modifie le titre du bouton
            if(nbReso == 1)
                strcpy(resolution.titre, PETITE_RESO);
            else if(nbReso == 2)
                strcpy(resolution.titre, MOYENNE_RESO);
            else
                strcpy(resolution.titre, GRANDE_RESO);

            //selon le mode d'image on modifie le titre du bouton
            if(fenetre.fullscreen)
                strcpy(fullscreen.titre, PLEIN_ECRAN);
            else
                strcpy(fullscreen.titre, FENETREE);

            SDL_FillRect(ecran, 0, 0xFFFFFF);
            //selon la résolution on affiche l'image de fond à la bonne taille
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
            positionTexte.y = ecran->h/4;
            positionTexte.x = ecran->w/2-texte->w/2;
            SDL_BlitSurface(texte,0,ecran,&positionTexte);


            //selon le menu actif on colore le bon bouton
            if(nbMenu == 0)
            {
                //bouton resolution
                bouton(ecran, &resolution, rougeActif, nbReso, 1);
                bouton(ecran, &fullscreen, rougeActif, nbReso, 0);
                bouton(ecran, &retour, rougeActif, nbReso, 0);
            }
            else if(nbMenu == 1)
            {
                //bouton plein écran
                bouton(ecran, &resolution, rougeActif, nbReso, 0);
                bouton(ecran, &fullscreen, rougeActif, nbReso, 1  );
                bouton(ecran, &retour, rougeActif, nbReso, 0);
            }
            else
            {
                //bouton retour
                bouton(ecran, &resolution, rougeActif, nbReso, 0);
                bouton(ecran, &fullscreen, rougeActif, nbReso, 0);
                bouton(ecran, &retour, rougeActif, nbReso, 1);
            }

            SDL_Flip(ecran);

            //on attend une entrée clavier
            SDL_WaitEvent(&event);
            switch(event.type)
            {
                //si l'utilisateur souhaite quitter le programme on ferme la fentre
            case SDL_QUIT:
                saveConfig(fenetre);
                TTF_Quit();
                SDL_Quit();
                exit(0);
                break;
            case SDL_JOYAXISMOTION:
                if (event.jaxis.axis == 0 && event.jaxis.value < -3200)
                {
                    /* Vers la gauche */
                    //si on appuie sur la touche gauche
                    if(nbMenu == 0)//on change la résolution...
                        nbReso = changeResolution(true, &fenetre, nbReso);
                    else if(nbMenu == 1)//... ou le mode plein écran
                        fenetre.fullscreen = pleinEcran(fenetre.fullscreen);

                    if(!fenetre.fullscreen)//on applique les changements
                        ecran = SDL_SetVideoMode(fenetre.width, fenetre.height, fenetre.couleur, SDL_HWSURFACE|SDL_DOUBLEBUF);
                    else
                        ecran = SDL_SetVideoMode(fenetre.width, fenetre.height, fenetre.couleur, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN);
                }
                else if (event.jaxis.axis == 0 && event.jaxis.value > 3200)
                {
                    /* Vers la droite */
                    //si on appuie sur la fleche droite
                    if(nbMenu == 0)//on change la résolution...
                        nbReso = changeResolution(false, &fenetre, nbReso);
                    else if(nbMenu == 1)//...on le mode plein écran
                        fenetre.fullscreen = pleinEcran(fenetre.fullscreen);

                    if(!fenetre.fullscreen)//on applique les changements
                        ecran = SDL_SetVideoMode(fenetre.width, fenetre.height, fenetre.couleur, SDL_HWSURFACE|SDL_DOUBLEBUF);
                    else
                        ecran = SDL_SetVideoMode(fenetre.width, fenetre.height, fenetre.couleur, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN);
                    saveConfig(fenetre);
                }
                else if (event.jaxis.axis == 1 && event.jaxis.value < -3200)
                {
                    /* Vers le haut */
                    if(nbMenu > 0)
                        nbMenu--;
                    else
                        nbMenu = 2;
                }
                else if (event.jaxis.axis == 1 && event.jaxis.value > 3200)
                {
                    /* Vers le bas */
                    if(nbMenu < 2)
                        nbMenu++;
                    else
                        nbMenu = 0;
                }
                break;
            case SDL_JOYBUTTONDOWN:
                if (event.jbutton.button == 0)
                {
                    //si on appuie sur echap, on enregistre puis on quitte le menu
                    saveConfig(fenetre);
                    done = true;
                }
                else if (event.jbutton.button == 1)
                {
                    if(nbMenu == 0)//on change la résolution...
                        nbReso = changeResolution(false, &fenetre, nbReso);
                    else if(nbMenu == 1)//...on le mode plein écran
                        fenetre.fullscreen = pleinEcran(fenetre.fullscreen);
                    else
                        saveConfig(fenetre);
                    done = true;

                    if(!fenetre.fullscreen)//on applique les changements
                        ecran = SDL_SetVideoMode(fenetre.width, fenetre.height, fenetre.couleur, SDL_HWSURFACE|SDL_DOUBLEBUF);
                    else
                        ecran = SDL_SetVideoMode(fenetre.width, fenetre.height, fenetre.couleur, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN);
                    saveConfig(fenetre);
                }
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    //touche du haut on décrémente l'indice de menu ou on le replace en bas
                case SDLK_UP:
                    if(nbMenu > 0)
                        nbMenu--;
                    else
                        nbMenu = 2;
                    break;
                    //touche du bas on incrémente l'indice de menu ou on le replace en haut
                case SDLK_DOWN:
                    if(nbMenu < 2)
                        nbMenu++;
                    else
                        nbMenu = 0;
                    break;
                case SDLK_LEFT:
                    //si on appuie sur la touche gauche
                    if(nbMenu == 0)//on change la résolution...
                        nbReso = changeResolution(true, &fenetre, nbReso);
                    else if(nbMenu == 1)//... ou le mode plein écran
                        fenetre.fullscreen = pleinEcran(fenetre.fullscreen);

                    if(!fenetre.fullscreen)//on applique les changements
                        ecran = SDL_SetVideoMode(fenetre.width, fenetre.height, fenetre.couleur, SDL_HWSURFACE|SDL_DOUBLEBUF);
                    else
                        ecran = SDL_SetVideoMode(fenetre.width, fenetre.height, fenetre.couleur, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN);
                    break;
                case SDLK_RIGHT:
                    //si on appuie sur la fleche droite
                    if(nbMenu == 0)//on change la résolution...
                        nbReso = changeResolution(false, &fenetre, nbReso);
                    else if(nbMenu == 1)//...on le mode plein écran
                        fenetre.fullscreen = pleinEcran(fenetre.fullscreen);

                    if(!fenetre.fullscreen)//on applique les changements
                        ecran = SDL_SetVideoMode(fenetre.width, fenetre.height, fenetre.couleur, SDL_HWSURFACE|SDL_DOUBLEBUF);
                    else
                        ecran = SDL_SetVideoMode(fenetre.width, fenetre.height, fenetre.couleur, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN);
                    saveConfig(fenetre);
                    break;
                case SDLK_ESCAPE:
                    //si on appuie sur echap, on enregistre puis on quitte le menu
                    saveConfig(fenetre);
                    done = true;
                    break;
                case SDLK_RETURN:
                    if(nbMenu == 0)//on change la résolution...
                        nbReso = changeResolution(false, &fenetre, nbReso);
                    else if(nbMenu == 1)//...on le mode plein écran
                        fenetre.fullscreen = pleinEcran(fenetre.fullscreen);
                    else
                        saveConfig(fenetre);
                    done = true;

                    if(!fenetre.fullscreen)//on applique les changements
                        ecran = SDL_SetVideoMode(fenetre.width, fenetre.height, fenetre.couleur, SDL_HWSURFACE|SDL_DOUBLEBUF);
                    else
                        ecran = SDL_SetVideoMode(fenetre.width, fenetre.height, fenetre.couleur, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN);
                    saveConfig(fenetre);
                    break;
                default:
                    break;
                }
                break;
            }
        }
    }
    SDL_FreeSurface(texte);
    TTF_CloseFont(police);
    //!<--Ataw
}


int changeResolution(bool sens, Fenetre *fenetre, int nbReso)
{
    //!Ataw-->
    if(sens)//si on appuie sur la touche gauche on décrémente l'indice de résolution et on change la résolution
    {
        if(nbReso == 1)
        {
            fenetre->width = 1024;
            fenetre->height = 768;
            return 3;
        }
        else if(nbReso == 2)
        {
            fenetre->width = 640;
            fenetre->height = 480;
            return nbReso -= 1;
        }
        else if(nbReso == 3)
        {
            fenetre->width = 800;
            fenetre->height = 600;
            return nbReso -= 1;
        }
    }
    else//si on appuie sur la touche droite on incrémente l'indice de résolution et on change la résolution
    {
        if(nbReso == 1)
        {
            fenetre->width = 800;
            fenetre->height = 600;
            return nbReso += 1;
        }
        else if(nbReso == 2)
        {
            fenetre->width = 1024;
            fenetre->height = 768;
            return nbReso += 1;
        }
        else if(nbReso == 3)
        {
            fenetre->width = 640;
            fenetre->height = 480;
            return 1;
        }
    }
    return 1; //on retourne la résolution 800x600 si il y a un problème dans la fonction
    //!<--Ataw
}


void bouton(SDL_Surface *ecran, Bouton *bouton, SDL_Color couleurTexte, int nbReso, bool actif)
{
    //!Ataw-->
    SDL_Color couleur = {255,255,255};
    //MatHack: Permet de montrer la selection
    if(!actif) //Si desactiver
    {
        couleurTexte = couleur;

    }
    //MatHack: Permet de faire un style [****]
    char titre[100];
    int tailleFont;
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


    if(nbReso == 1)//selon la résolution on applique une une taille de police différente
        bouton->police = TTF_OpenFont("calibri.ttf", tailleFont-8);

    else if(nbReso == 2)
        bouton->police = TTF_OpenFont("calibri.ttf", tailleFont-4);

    else if(nbReso == 3)
        bouton->police = TTF_OpenFont("calibri.ttf", tailleFont);




    bouton->texte = TTF_RenderText_Blended(bouton->police, titre, couleurTexte);
    //on crée la surface texte puis on initialise la structure SDL_Rect du bouton.
    bouton->pos.w = bouton->texte->w + 5;
    bouton->pos.h = bouton->texte->h;
    bouton->pos.x = ecran->w/2 - bouton->pos.w/2;
    //on blitte sur l'écran
    SDL_BlitSurface(bouton->texte, 0, ecran, &bouton->pos);
    TTF_CloseFont(bouton->police);
    SDL_FreeSurface(bouton->texte);
    //!<--Ataw
}


void saveConfig(Fenetre fenetre)
{
    //!Ataw-->
    FILE *config;//on ouvre le fichier config
    config = fopen("config.txt", "w");
    if(config != NULL)
    {
        //on enregistre les données relative à la fenetre
        fprintf(config, "screenWidth = %i\n", fenetre.width);
        fprintf(config, "screenHeight = %i\n", fenetre.height);
        fprintf(config, "color = %i\n", fenetre.couleur);
        fprintf(config, "fullscreen = %i", fenetre.fullscreen);
        fclose(config);
    }
    //!<--Ataw
}

int pleinEcran(int boolEcran)
{
    //!Ataw-->
    //on modifie la valeur de fenetre.fullscreen
    if(boolEcran)
        return 0;
    else
        return 1;
    //!<--Ataw
}
