//*******************************************
//Auteur : Mathieu MARTIN
//Desription : Outils pour afficher/editer les map
//Date : 24/01/2012
//*******************************************
#include "editeur.h"
#include "jeu.h"
#include "windows.h"
//!Fonction qui permet de démarrer l'éditeur
void editeur(SDL_Surface *ecran, Map *carte, ParamMap *opt,SDL_Joystick *joystick)
{
    //On recharge toujour la map quand on arrive dans editeur
    chargeMap(carte);
    //Déclaration des variables
    int continuer = 1;
    int mouseX;
    int mouseY;
    int typeBloc=1;
    int clicGauche=0;
    int clicDroite=0;
    carte->xscroll = 0;
    carte->yscroll = 0;

    //Interface
    SDL_Surface *interfaceFond;
    SDL_Rect posInterface;
    interfaceFond = SDL_CreateRGBSurface(SDL_HWSURFACE, ecran->w, InterfaceHauteur, 32, 0,0,0,0);
    SDL_FillRect(interfaceFond, NULL, SDL_MapRGB(interfaceFond->format, 0,0,0));
    posInterface.x = 0;
    posInterface.y = ecran->h - InterfaceHauteur;
    //Option de affichage
    SDL_ShowCursor(0);
    SDL_WM_SetCaption("Map editor", NULL);
    opt->editeur = 1;
    opt->helpInterface = 1;
    //Chargement de la map dans du fichier lvl.txt vers tableau
    //si on recharge pas bug entre editeur => ingame => editeur
    //Fonction d'affichage et d'édition
    //Debug
    if(debugOutputMap==1)
        printf("\nOption editeur : %i\n", opt->editeur);
    //Active la répétition des touche
    SDL_EnableKeyRepeat(200,10);
    //Affichage du fond
    SDL_Surface *fond;
    carte->posFond.x = 0;
    carte->posFond.y = 0;
    fond = IMG_Load("image/map/fondV4.gif");
    //fond = SDL_CreateRGBSurface(SDL_HWSURFACE, ecran->w, ecran->h, 32, 0,0,0,0);
    //SDL_FillRect(fond, NULL, SDL_MapRGB(fond->format, 0, 0,0));
    //Boucle principale de editeur
    int fonduOk = 1;
    affichageFonduJeu(ecran, 10, &fonduOk, *carte, *opt, fond);
    while(continuer)
    {
        //if(recharge==1)
        //chargeMap(carte);
        //Permet le scrolling de la map
        ScrollingDebordement(carte, ecran);
        //Fond
        //SDL_BlitSurface(fond, NULL, ecran, &posFond);
        //Fond uni
        SDL_BlitSurface(fond, NULL, ecran, &carte->posFond);

        //Affichage des tiles
        afficheMap(*carte, ecran, *opt);
        //Affiche interface d'utilisation
        if(opt->helpInterface == 1)
        {
            SDL_BlitSurface(interfaceFond, NULL, ecran, &posInterface);
            afficheInterface(ecran, *carte);
        }
        //Gestion des mouvement
        gestionToucheEditeur(carte, opt, ecran, &continuer, &mouseX, &mouseY, &clicGauche, &clicDroite, &typeBloc,joystick);
        //Permet d'afficher le bloc à posser
        aideBloc(ecran, *carte, &mouseX, &mouseY, &typeBloc);
        //Affiche la versin en cour
        versionTexte();
        //Refresh de editeur
        SDL_Flip(ecran);


    };
    //Libère la mémoire du fond
    SDL_FreeSurface(interfaceFond);
    SDL_FreeSurface(fond);
    //Désactive le keyboard
    SDL_EnableKeyRepeat(0,0);
    //Remet le bon nom de la fenetre
    SDL_WM_SetCaption("MarioCBA", NULL);
    //Désactive editeur
    opt->editeur = 0;
}

//Permet d'afficher interface de editeur
void afficheInterface(SDL_Surface *screen, Map carte)
{
    //Intilise
    TTF_Font *font=NULL;
    SDL_Surface *texte=NULL;
    SDL_Rect posTexte;
    font = TTF_OpenFont("calibri.ttf", 20);
    SDL_Color couleur = {255, 255, 255};
    char texteNiveau[30];
    //Texte
    //Niveau en cour
    posTexte.x = screen->w - 100;
    posTexte.y = screen->h - 30;
    int niveau = carte.level+1;
    sprintf(texteNiveau, "Niveau : %i", niveau);
    texte=TTF_RenderText_Blended(font, texteNiveau, couleur);
    SDL_BlitSurface(texte, 0, screen, &posTexte);
    SDL_FreeSurface(texte);
    //Texte explication
    posTexte.x = 20;
    posTexte.y = screen->h - 130;
    texte=TTF_RenderText_Blended(font, "1 Mur incassable | 2 Pièce | 3 Sol | 4 Mur cassable | 5 Spawn joueur | 6 Spawn monstre | 7 Fin", couleur);
    SDL_BlitSurface(texte, 0, screen, &posTexte);
    SDL_FreeSurface(texte);
    posTexte.y = screen->h - 100;
    texte=TTF_RenderText_Blended(font, "s:Sauvegarde du niveau | n:Nouveau niveau | +/-:Changer de niveau | KP 8/2: Ajout/Suppression de ligne", couleur);
    SDL_BlitSurface(texte, 0, screen, &posTexte);
    SDL_FreeSurface(texte);
    posTexte.x = 20;
    posTexte.y = screen->h - 70;
    texte=TTF_RenderText_Blended(font, "KP 4/6 Ajout/Suppression de colonne | Fleche directionnel:Scrolling | c:Effacement de la map | h:Afficher/Masquer l'aide", couleur);
    SDL_BlitSurface(texte, 0, screen, &posTexte);
    SDL_FreeSurface(texte);
    //Free mémoire
    TTF_CloseFont(font);
}

void aideBloc(SDL_Surface *screen, Map carte, int *mX, int *mY, int *typeBloc)
{
    //Affichage du bloc de selection en cour
    SDL_Surface *surfaceBloc; //Pour affichage des bloc
    SDL_Rect posBloc; //Pour la position des bloc
    SDL_Rect posTile[carte.nbTile]; //Alloue un tableau au nombre de tiles utiliser
    surfaceBloc = IMG_Load(carte.filenameTileSet);
    //Decoupage des sprite du decors
    int i;
    for(i=0; i<carte.nbTile; i++)
    {
        posTile[i].x = LTILE * i;
        posTile[i].y = 0;
        posTile[i].w = LTILE;
        posTile[i].h = HTILE;
    }
    //Gestion de alpha
    SDL_SetAlpha(surfaceBloc, SDL_SRCALPHA, 128);
    posBloc.y = *mY-(posTile[*typeBloc].h/2);
    posBloc.x = *mX-(posTile[*typeBloc].w/2);
    SDL_BlitSurface(surfaceBloc,  &posTile[*typeBloc-1], screen, &posBloc);
    SDL_FreeSurface(surfaceBloc);

    //Bloc mouse qui aide le positionnement
    SDL_Surface *mouseBloc;
    SDL_Rect posMouseBloc;
    mouseBloc = IMG_Load("image/editeur/tools/mouse.bmp");
    posMouseBloc.y = *mY-(mouseBloc->h/2);
    posMouseBloc.x = *mX-(mouseBloc->w/2);
    SDL_BlitSurface(mouseBloc, NULL, screen, &posMouseBloc);
    SDL_FreeSurface(mouseBloc);
}
//Fonction qui permet de crée de nouveau niveau
void creationNiveau(Map *carte)
{
    //!Ajout du niveau dans le fichier de gestion des niveau
    //!==========================================
    char newNiveau[30];
    int numNiveau = carte->nbNiveau+1;
    sprintf(newNiveau, "niveau%i", numNiveau);
    char filename[50];
    sprintf(filename, "niveau/%s", newNiveau);
    //MessageBox(NULL, filename, "Test", MB_ICONQUESTION|MB_OK|MB_DEFBUTTON1|MB_SERVICE_NOTIFICATION);
    FILE* fgestNiveau = fopen("niveau/gestionNiveau", "a");
    if(fgestNiveau == NULL)
    {
        MessageBox(NULL,"Impossible d'ouvrir la gestion des niveau", "Erreur",
                   MB_ICONERROR|MB_OK|MB_DEFBUTTON1|MB_SERVICE_NOTIFICATION);
        exit(EXIT_FAILURE);
    }
    fprintf(fgestNiveau, "%i %s %s\n", numNiveau, newNiveau, "image/map/minimap/level2.bmp");
    fclose(fgestNiveau);
    //!==========================================
    //!Ajout du fichier du niveau
    //!==========================================
    FILE* nNiveau = fopen(filename, "w+");
    fprintf(nNiveau, "0\n");
    fclose(nNiveau);
    //!==========================================
    //!On fait un mise à jour des niveau
    //On reliste les niveau
    listeDesNiveau(carte);
}

void gestionToucheEditeur(Map *carte, ParamMap *opt, SDL_Surface *ecran,  int *continuer, int *mouseX, int *mouseY, int *clicGauche, int *clicDroite, int *typeBloc,SDL_Joystick *joystick)
{
    int i, j;
    SDL_Event event;
    int vitesse = 5;
    while(SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            if(SDLMessageBox((char*)"Voulez vous quitter le jeu ?"))
            {
                TTF_Quit();
                SDL_Quit();
                exit(EXIT_SUCCESS);
            }
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
                //Gestion du scrolling manuel (vitesse du perso)
            case SDLK_LEFT:
                carte->xscroll -= vitesse;
                break;
            case SDLK_RIGHT:
                carte->xscroll += vitesse;
                break;
            case SDLK_UP:
                carte->yscroll -= vitesse;
                break;
            case SDLK_DOWN:
                carte->yscroll += vitesse;
                break;
                //Demande à utilisateur
            case SDLK_ESCAPE:
                /*switch(SDLMessageBox((char*)"Voulez vous sauvegarder avant le retour au menu ?", (char*)"Choix"))
                {
                case IDYES:
                    sauvegardeMap(*carte);*/
                *continuer = 0;
                /* break;
                                case IDNO:
                 *continuer = 0;
                 break;
                                default:
                 break;
                                }*/
                break;
                //!Gestion des niveau
                //!======================================
            case SDLK_KP_PLUS:
                if(carte->level<carte->nbNiveau-1)
                    carte->level++;
                //On recharge pour changer de niveau
                chargeMap(carte);
                break;
            case SDLK_KP_MINUS:
                if(carte->level>0)
                    carte->level--;
                //On recharge pour changer de niveau
                chargeMap(carte);
                break;
                //!======================================
                //!Gestion du tableau (add/del ligne)
                //!======================================
                //!hack pour lundi !
                //Add ligne
            case SDLK_KP6:
                //Hack:Pour demo de lundi
                if(carte->MONDE_LARGEUR>=2) //On limite la supprésion à 1ligne apparente 2 car ligne de \n
                {
                    carte->MONDE_LARGEUR++;
                    for(i=0; i<carte->MONDE_HAUTEUR; i++)
                    {
                        for(j=carte->MONDE_LARGEUR-2; j<carte->MONDE_LARGEUR-1; j++)
                        {
                            carte->tableauMap[i][j] = 0; //On initilise la colonne ajouter à 0
                            carte->tableauMap[i][j+1] = 8; //8 = Saut de ligne sur la dernière colonne = colonne invisible
                        }
                    }
                }
                //Reallocation du tableau avec ajout
                /*carte->MONDE_LARGEUR += 5;
                carte->tableauMap = (int**)realloc( carte->tableauMap, sizeof(int*)*carte->MONDE_LARGEUR);
                //Si plantage mémoreif(carte->tableauMap == NULL)
                if(carte->tableauMap != NULL)
                {
                    // allouer les nouvelles colonnes.
                    carte->tableauMap[carte->MONDE_LARGEUR] = (int*)malloc(sizeof(int)*carte->MONDE_HAUTEUR);
                    for(i=0; i<carte->MONDE_HAUTEUR; i++)
                    {
                        for(j=carte->MONDE_LARGEUR-2; j<carte->MONDE_LARGEUR-1; j++)
                        {
                            carte->tableauMap[i][j] = 2;
                        }
                    }

                    for(i=0; i<carte->MONDE_HAUTEUR; i++)
                    {
                        for(j=carte->MONDE_LARGEUR-1; j<carte->MONDE_LARGEUR; j++)
                        {
                            carte->tableauMap[i][j] = 8;
                        }
                    }
                    //SDL_Delay(500);
                }*/
                break;
                //Del ligne
            case SDLK_KP4:
                //Hack:Pour demo de lundi
                if(carte->MONDE_LARGEUR>=3) //On limite la supprésion à 1ligne apparente 2 car ligne de \n
                {
                    carte->MONDE_LARGEUR--;
                    for(i=0; i<carte->MONDE_HAUTEUR; i++)
                    {
                        for(j=carte->MONDE_LARGEUR-2; j<carte->MONDE_LARGEUR-1; j++)
                        {
                            carte->tableauMap[i][j] = 0; //On initilise la colonne ajouter à 0
                            carte->tableauMap[i][j+1] = 8; //8 = Saut de ligne sur la dernière colonne = colonne invisible
                        }
                    }
                }
                /*//Reallocation du tableau avec suppresion
                carte->MONDE_LARGEUR -= 5;
                carte->tableauMap = (int**)realloc( carte->tableauMap, sizeof(int*)*carte->MONDE_LARGEUR);
                // allouer les nouvelles colonnes.
                if(carte->tableauMap != NULL)
                {
                    //Si problème sur la mémoire lors du realloc
                    carte->tableauMap[carte->MONDE_LARGEUR] = (int*)malloc(sizeof(int)*carte->MONDE_HAUTEUR);
                    //SDL_Delay(100);
                    for(i=0; i<carte->MONDE_HAUTEUR; i++)
                    {
                        for(j=carte->MONDE_LARGEUR-2; j<carte->MONDE_LARGEUR-1; j++)
                        {
                            carte->tableauMap[i][j] = 2;
                        }
                    }
                    //SDL_Delay(100);
                    for(i=0; i<carte->MONDE_HAUTEUR; i++)
                    {
                        for(j=carte->MONDE_LARGEUR-1; j<carte->MONDE_LARGEUR; j++)
                        {
                            carte->tableauMap[i][j] = 8;
                        }
                    }
                }*/
                break;
                //Add hauteur
            case SDLK_KP2:
                if(carte->MONDE_HAUTEUR>=1) //On limite la supprésion à 1ligne apparente 2 car ligne de \n
                {
                    carte->MONDE_HAUTEUR++;
                    for(i=carte->MONDE_HAUTEUR-1; i<carte->MONDE_HAUTEUR; i++)
                    {
                        for(j=0; j<carte->MONDE_LARGEUR-1; j++)
                        {
                            carte->tableauMap[i][j] = 0; //On initilise la colonne ajouter à 0
                        }
                        for(j=carte->MONDE_LARGEUR-1; j<carte->MONDE_LARGEUR; j++)
                        {
                            carte->tableauMap[i][j] = 8; //On initilise la colonne ajouter à 0
                        }

                    }
                }

                break;
                //Del hauteur
            case SDLK_KP8:
                if(carte->MONDE_HAUTEUR>=2) //On limite la supprésion à 1ligne apparente 2 car ligne de \n
                {
                    carte->MONDE_HAUTEUR--;
                    for(i=carte->MONDE_HAUTEUR-1; i<carte->MONDE_HAUTEUR; i++)
                    {
                        for(j=0; j<carte->MONDE_LARGEUR; j++)
                        {
                            carte->tableauMap[i][j] = 0; //On initilise la colonne ajouter à 0
                        }
                        for(j=carte->MONDE_LARGEUR; j<carte->MONDE_LARGEUR; j++)
                        {
                            carte->tableauMap[i][j] = 8; //On initilise la colonne ajouter à 0
                        }

                    }
                }

                break;
                //!======================================
            case SDLK_s:
                //Affiche une message box pour sauvegarder
                char str[100];
                strcat(str, "Voulez vous sauvegarder ");
                strcat(str, carte->gestNiveau[carte->level].fileName);
                strcat(str, " ?");
                SDL_ShowCursor(1);
                if(SDLMessageBox((char*)str))
                {
                    sauvegardeMap(*carte);
                }
                SDL_ShowCursor(0);
                break;
                //Gestion des bloc
            case SDLK_1:
                *typeBloc = tileMUR;
                break;
            case SDLK_2:
                *typeBloc = tileBONUS;
                break;
            case SDLK_3:
                *typeBloc = tileSOL;
                break;
            case SDLK_4:
                *typeBloc = tileReserve;
                break;
            case SDLK_5:
                *typeBloc = tileSPAWN;
                break;
            case SDLK_6:
                *typeBloc = tileMONSTRE;
                break;
            case SDLK_7:
                *typeBloc = tileFIN;
                break;
                //Gestion des outils
                //Clean de la map
            case SDLK_c:
                int i,j;
                for(i=0; i<carte->MONDE_HAUTEUR; i++)
                    for(j=0; j<carte->MONDE_LARGEUR-1; j++)//-1 pour pas del les saute de ligne !
                        carte->tableauMap[i][j] = 0;
                break;
                //Nouveau niveau
            case SDLK_n:
                creationNiveau(carte);
                break;
            case SDLK_g:
                if(opt->afficheGrille == 1)
                    opt->afficheGrille = 0;
                else
                    opt->afficheGrille = 1;
                break;
            case SDLK_j:
                sauvegardeMap(*carte);
                jeu(ecran, *carte, *opt,joystick);
                break;
            case SDLK_h:
                if(opt->helpInterface == 0)
                    opt->helpInterface = 1;
                else
                    opt->helpInterface = 0;
                break;
            default:
                break;
            }
            break;
            //Mouvement souris
        case SDL_JOYAXISMOTION:
            if (event.jaxis.axis == 0 && event.jaxis.value < -3200)
            {
                carte->xscroll -= vitesse;
            }
            else if (event.jaxis.axis == 0 && event.jaxis.value > 3200)
            {
                carte->xscroll += vitesse;
            }
            else if (event.jaxis.axis == 1 && event.jaxis.value < -3200)
            {
                carte->yscroll -= vitesse;
            }
            else if (event.jaxis.axis == 1 && event.jaxis.value > 3200)
            {
                carte->yscroll += vitesse;
            }
            break;
        case SDL_JOYBUTTONDOWN:
            if (event.jbutton.button == 0)
            {
                *typeBloc = tileMUR;
            }
            else if (event.jbutton.button == 1)
            {
                *typeBloc = tileBONUS;
            }
            else if (event.jbutton.button == 2)
            {
                *typeBloc = tileSOL;
            }
            else if (event.jbutton.button == 3)
            {
                *typeBloc = tileReserve;
            }
            else if (event.jbutton.button == 4)
            {
                *typeBloc = tileSPAWN;
            }
            else if (event.jbutton.button == 5)
            {
                *typeBloc = tileMONSTRE;
            }
            else if (event.jbutton.button == 6)
            {
                *typeBloc = tileFIN;
            }
            else if (event.jbutton.button == 7)
            {
                int i,j;
                for(i=0; i<carte->MONDE_HAUTEUR; i++)
                    for(j=0; j<carte->MONDE_LARGEUR-1; j++)//-1 pour pas del les saute de ligne !
                        carte->tableauMap[i][j] = 0;
            }
            else if (event.jbutton.button == 8)
            {
                creationNiveau(carte);
            }
            else if (event.jbutton.button == 9)
            {
                if(opt->afficheGrille == 1)
                    opt->afficheGrille = 0;
                else
                    opt->afficheGrille = 1;
            }
            else if (event.jbutton.button == 10)
            {
                sauvegardeMap(*carte);
                jeu(ecran, *carte, *opt,joystick);
            }
            else if (event.jbutton.button == 11)
            {
                if(opt->helpInterface == 0)
                    opt->helpInterface = 1;
                else
                    opt->helpInterface = 0;
            }
            else if (event.jbutton.button == 12)
            {
                //Affiche une message box pour sauvegarder
                char str[100];
                strcat(str, "Voulez vous sauvegarder ");
                strcat(str, carte->gestNiveau[carte->level].fileName);
                strcat(str, " ?");
                SDL_ShowCursor(1);
                if(SDLMessageBox((char*)str))
                {
                    sauvegardeMap(*carte);
                }
            }
            break;
        case SDL_MOUSEMOTION:
            //Bloc qui montre ou l'on va poser un bloc
            //if(event.motion.x%32 || event.motion.y%32 )
            //{
            *mouseX = event.motion.x;
            *mouseY = event.motion.y;
            if(*clicGauche==1)
                carte->tableauMap[(event.button.y+carte->yscroll)/HTILE][(event.button.x+carte->xscroll)/LTILE] = *typeBloc;
            else if(*clicDroite==1)
                carte->tableauMap[(event.button.y+carte->yscroll)/HTILE][(event.button.x+carte->xscroll)/LTILE] = 0;
            //}
            break;

            //Touche souris appuie
        case SDL_MOUSEBUTTONDOWN:
            if(event.button.button == SDL_BUTTON_LEFT)
            {
                carte->tableauMap[(event.button.y+carte->yscroll)/HTILE][(event.button.x+carte->xscroll)/LTILE] = *typeBloc;
                *clicGauche = 1;
            }
            if(event.button.button == SDL_BUTTON_RIGHT)
            {
                carte->tableauMap[(event.button.y+carte->yscroll)/HTILE][(event.button.x+carte->xscroll)/LTILE] = 0;
                *clicDroite = 1;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            //
            if(event.button.button == SDL_BUTTON_LEFT)
                *clicGauche = 0;
            else if(event.button.button == SDL_BUTTON_RIGHT)
                *clicDroite = 0;

            break;
        default:
            break;
        }
    }
}
//Fonction qui permet d'afficher une boite de dialogue return le numero du bouton appuier
//Plus d'info : http://msdn.microsoft.com/en-us/library/ms645505%28VS.85%29.aspx
int SDLMessageBox(char *str)
{
    int continuer = 1,choix = 0,touche_enfonce = 0;
    SDL_Surface *screen = SDL_GetVideoSurface();
    SDL_Surface *rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, 7*strlen(str), 50, 32, 0, 0, 0, 0),*texteOui = NULL,*texteNon = NULL,*texteStr = NULL;
    SDL_FillRect(rectangle, NULL, SDL_MapRGB(screen->format, 190, 190, 190));
    SDL_Rect posRect,posTexteOui,posTexteNon,posTexteStr;
    posRect.x = screen->w/2 - rectangle->w/2;
    posRect.y = screen->h/2 - rectangle->h/2;
    SDL_BlitSurface(rectangle, NULL, screen, &posRect);
    TTF_Font *police = TTF_OpenFont("calibri.ttf", 15);
    SDL_Color couleurNoire = {0, 0, 0},couleurBlanche = {190,190,190},couleurRouge = {255,0,0},couleurTexteNon = {0,0,0},couleurTexteOui = {0,0,0};
    texteStr = TTF_RenderText_Shaded(police,str,couleurNoire,couleurBlanche);
    posTexteStr.x = posRect.x+5;
    posTexteStr.y = posRect.y+10;
    SDL_BlitSurface(texteStr, NULL, screen, &posTexteStr);
    texteOui = TTF_RenderText_Shaded(police,"Oui",couleurTexteOui,couleurBlanche);
    posTexteOui.x = posRect.x+rectangle->w/4;
    posTexteOui.y = posRect.y+rectangle->h-5-texteOui->h;
    SDL_BlitSurface(texteOui, NULL, screen, &posTexteOui);
    texteNon = TTF_RenderText_Shaded(police,"Non",couleurTexteNon,couleurBlanche);
    posTexteNon.x = posRect.x + (rectangle->w*3)/4-texteNon->w;
    posTexteNon.y = posTexteOui.y;
    SDL_BlitSurface(texteNon, NULL, screen, &posTexteNon);
    SDL_Flip(screen);
    SDL_Event event;
    while(continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            if(SDLMessageBox((char*)"Voulez vous quitter le jeu ?"))
            {
                TTF_Quit();
                SDL_Quit();
                exit(EXIT_SUCCESS);
            }
            SDL_BlitSurface(rectangle, NULL, screen, &posRect);
            SDL_BlitSurface(texteStr, NULL, screen, &posTexteStr);
            SDL_Delay(100);
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_RIGHT: // Flèche droite
                if(touche_enfonce == 0)
                {
                    if(choix)
                        choix = 0;
                    else
                        choix = 1;
                    touche_enfonce = 1;
                }
                break;
            case SDLK_LEFT: // Flèche gauche
                if(touche_enfonce == 0)
                {
                    if(choix)
                        choix = 0;
                    else
                        choix = 1;
                    touche_enfonce = 1;
                }
                break;
            case SDLK_RETURN:
                continuer = 0;
                break;
            default:
                break;
            }
            break;
        case SDL_KEYUP:
            if(event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_LEFT)
                touche_enfonce = 0;
            break;
        }
        if(choix)
        {
            couleurTexteOui = couleurRouge;
            couleurTexteNon = couleurNoire;
        }
        else
        {
            couleurTexteOui = couleurNoire;
            couleurTexteNon = couleurRouge;
        }
        SDL_FreeSurface(texteNon);
        texteNon = TTF_RenderText_Shaded(police,"Non",couleurTexteNon,couleurBlanche);
        SDL_BlitSurface(texteNon, NULL, screen, &posTexteNon);
        SDL_FreeSurface(texteOui);
        texteOui = TTF_RenderText_Shaded(police,"Oui",couleurTexteOui,couleurBlanche);
        SDL_BlitSurface(texteOui, NULL, screen, &posTexteOui);
        SDL_Flip(screen);
    }
    SDL_FreeSurface(rectangle);
    SDL_FreeSurface(texteStr);
    SDL_FreeSurface(texteOui);
    SDL_FreeSurface(texteNon);
    TTF_CloseFont(police);
    return choix;
}

void versionTexte()
{
    SDL_Surface *versionTexte;
    SDL_Rect posTexteVersion;
    TTF_Font *fontVersion=NULL;
    fontVersion = TTF_OpenFont("calibri.ttf", 15);
    SDL_Color couleur = {255, 255, 255};
    posTexteVersion.x = SDL_GetVideoSurface()->w - 180;
    posTexteVersion.y = 10;

    versionTexte=TTF_RenderText_Blended(fontVersion, "Version 0.5Beta - Free Only", couleur);
    SDL_BlitSurface(versionTexte, 0, SDL_GetVideoSurface(), &posTexteVersion);
    SDL_FreeSurface(versionTexte);
    TTF_CloseFont(fontVersion);
}
