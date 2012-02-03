//*******************************************
//Auteur : Mathieu MARTIN
//Desription : Fonction général pour charger et afficher une map
//Date : 23/01/2012
//*******************************************
#include "map.h"
#include <SDL_image.h>
#include <windows.h>
void initMap(Map *carteJeu)
{
    //sprintf(carteJeu->filenameLevel, "lvl%i.txt",lvl);
    strcpy(carteJeu->filenameTileSet, "image/map/charsetV1.bmp");
    //carteJeu->level = lvl;
    carteJeu->LARGEUR_TILE = 32;
    carteJeu->HAUTEUR_TILE = 32;
    carteJeu->xscroll = 0;
    carteJeu->yscroll = 0;
    carteJeu->nbTile = 8; //Nombre de tile different
    carteJeu->nb_ennemi = 0;
    carteJeu->posDepEnnemi = (SDL_Rect*) malloc(sizeof(SDL_Rect));
}
//Fonction qui permet de chager la map du fichier vers le tableau qui est dans la strcuture Map (Alloc Dynamique)
void chargeMap(Map *carte)
{
    listeDesNiveau(carte);
    //!Déclaration des variables
    FILE* fichierMap;
    int i, j;
    char asciiChar;
    int nbLigne=0, nbCaratere=0;
    carte->nb_ennemi = 0;

    //Version thomas
    //fichierMap=fopen(carte->filenameLevel,"r");
    //Vériciation si le fichier charger, si erreur alors on ecrit erreur dans un fichier
    //!Choix de la map (different fichier)
    char nomLevel[50];
    strcpy(nomLevel, "niveau/");
    strcat(nomLevel, carte->gestNiveau[carte->level].fileName);
    fichierMap=fopen(nomLevel,"r");

    //Vériciation si le fichier charger, si erreur alors on ecrit erreur dans un fichier
    if(fichierMap == NULL)
    {
        fprintf(stderr, "Erreur dans le chargement du fichier de la map");
        MessageBox(NULL, "Erreur dans le chargement du niveau", "Erreur", MB_ICONERROR | MB_OK | MB_DEFBUTTON1);
        if(carte->level > 0)
        {
            carte->level--;
            strcpy(nomLevel, "niveau/");
            strcat(nomLevel, carte->gestNiveau[carte->level].fileName);
            fichierMap=fopen(nomLevel,"r");
            if(fichierMap == NULL)
            {
                fprintf(stderr, "Erreur dans le chargement du fichier de la map");
                MessageBox(NULL, "Erreur dans le chargement du niveau", "Erreur", MB_ICONERROR | MB_OK | MB_DEFBUTTON1);
            }
        }
    }



    //!Analyse de la map
    //!=========================================
    do
    {
        //Permet de recupérer le caratère en cour
        asciiChar = fgetc(fichierMap);
        if(asciiChar != EOF)
        {
            if(asciiChar == '\n')
            {
                //Debug
                if(debugOutputMap==1)
                    printf("\n");
                //Si l'on detect un saut de ligne on ajoute 1
                nbLigne++;
            }
            else
            {
                //Debug
                if(debugOutputMap==1)
                {
                    if(asciiChar == 'd')
                        printf("d");
                    else if(asciiChar == 'm')
                        printf("m");
                    else if(asciiChar == 'f')
                        printf("f");
                    else
                        printf("%i",asciiChar-'0');
                }
                //Ajout un caratère
                nbCaratere++;
            }
        }

    }
    while(asciiChar != EOF); //on quitte quand on detect le caratère de fin
    //!=========================================

    //Debug
    if(debugOutputMap==1)
        printf("\nSortie while:NbLigne : %i NbCaratere : %i : NbCaratereParLigne = %i\n", nbLigne, nbCaratere, nbCaratere/nbLigne);

    nbCaratere+=nbLigne; //On rajoute le nombre de \n que l'on avait pas compter

    //Intilisation des variable de la taille de la map
    carte->MONDE_LARGEUR = abs(nbCaratere/nbLigne);
    carte->MONDE_HAUTEUR = nbLigne;


    //Hack: Pour demo de lundi sans realloc
    /*//!Allocation dynamique
    //!===============================================
    //MatHack: Ce tableau dynamique permet de charger n'importe quel taille de map !
    carte->tableauMap = (int**)malloc(sizeof(int*)*carte->MONDE_HAUTEUR);
    for(i=0; i<carte->MONDE_HAUTEUR+1; i++)
    {
        carte->tableauMap[i] = (int*)malloc(sizeof(int)*carte->MONDE_LARGEUR);
    }
    //MatHack:Debug vers fichier stdout.exe
    if(debugOutputMap==1)
        printf("\nAllocation dynamique : MondeLargeur:%i MondeHauteur:%i", carte->MONDE_LARGEUR, carte->MONDE_HAUTEUR);
    //Intilisation du tableau complet à 0 = Vide
    for(i=0; i<carte->MONDE_HAUTEUR; i++)
    {
        for(j=0; j<=carte->MONDE_LARGEUR; j++)
        {
            carte->tableauMap[i][j] = 0;
        }
    }
    //!===============================================*/

    //Debug
    if(debugOutputMap==1)
        printf("\nSortie FIN :\n");

    //!Fichier vers tableauMap
    //!===============================================
    //Remonte le curseur du fichier tout en haut
    rewind(fichierMap);
    //Insersion du fichier vers le tableau dynamique
    carte->nb_ennemi = 0;
    for(i=0; i<carte->MONDE_HAUTEUR; i++)
    {
        for(j=0; j<carte->MONDE_LARGEUR; j++)
        {
            asciiChar = fgetc(fichierMap);

            if(asciiChar == 'd')
            {
                carte->tableauMap[i][j] = 'd';
                carte->posDepart.x = j * LTILE - carte->xscroll;
                carte->posDepart.y = i * HTILE - carte->yscroll;
            }
            else if(asciiChar == 'm')
            {
                carte->nb_ennemi++;
                carte->tableauMap[i][j] = 'm';
                carte->posDepEnnemi = (SDL_Rect*)realloc(carte->posDepEnnemi,carte->nb_ennemi*sizeof(SDL_Rect));
                carte->posDepEnnemi[carte->nb_ennemi-1].x = j * LTILE - carte->xscroll;
                carte->posDepEnnemi[carte->nb_ennemi-1].y = i * HTILE - carte->yscroll;
            }
            else if(asciiChar == 'f')
                carte->tableauMap[i][j] = 'f';
            else
                carte->tableauMap[i][j] = asciiChar - '0';
        }
    }
    //!===============================================

    //On ferme le fichier pour utiliser si jamais
    fclose(fichierMap);

    //Debug
    if(debugOutputMap==1)
    {
        printf("\nSortie for:NbLigne : %i NbCaratere : %i = NbCaratereParLigne = %i\n", nbLigne, nbCaratere, nbCaratere/nbLigne);
        printf("Charge map fin : Monde Longeur : %i Monde Hauteur : %i", carte->MONDE_LARGEUR, carte->MONDE_HAUTEUR);
    }


}

//Fonction qui sauvegarde la map charger par le filename
void sauvegardeMap(Map carte)
{
    FILE* fichierSave;
    char str[100];
    strcpy(str, "niveau/");
    strcat(str, carte.gestNiveau[carte.level].fileName);
    fichierSave = fopen(str, "w+");

    int i,j;
    for(i=0; i<carte.MONDE_HAUTEUR; i++)
    {
        for(j=0; j<carte.MONDE_LARGEUR; j++)
        {
            if(carte.tableauMap[i][j] == 'd' || carte.tableauMap[i][j] == tileSPAWN)
                fprintf(fichierSave, "d");
            else if(carte.tableauMap[i][j] == 'f' || carte.tableauMap[i][j] == tileFIN)
                fprintf(fichierSave, "f");
            else if(carte.tableauMap[i][j] == 61 || carte.tableauMap[i][j] == tileMONSTRE)
                fprintf(fichierSave, "m");
            else if(carte.tableauMap[i][j] == -38 || carte.tableauMap[i][j] == 8 ||carte.tableauMap[i][j] == '\n')
                fprintf(fichierSave, "\n");
            else
                fprintf(fichierSave, "%i", carte.tableauMap[i][j]);
        }
    }
    fclose(fichierSave);
}

//Fonction qui liste les niveau du dossier niveau : gestionNiveau.txt (id/niveau/minimap)
void listeDesNiveau(Map *carte)
{
    FILE *fgestionNiveau = fopen("niveau/gestionNiveau", "r");
    if(fgestionNiveau == NULL)
    {
        MessageBox(NULL,"Impossible d'ouvrir la gestion des niveau", "Erreur",
                   MB_ICONERROR|MB_OK|MB_DEFBUTTON1);
        exit(EXIT_FAILURE);
    }
    //Met les niveau dans la structure
    int i =0, nbLigne=0;
    carte->nbNiveau = 0;
    char asciiChar;
    do
    {
        asciiChar = fgetc(fgestionNiveau);
        if(asciiChar == '\n')
            nbLigne++;

    }
    while(asciiChar!=EOF);
    //Nombre de niveau
    carte->nbNiveau = nbLigne;
    /*char debug[100];
    sprintf(debug, "Nb niveau : %i", carte->nbNiveau);
    MessageBox(NULL, debug, "Debug", MB_ICONQUESTION|MB_OK|MB_DEFBUTTON1);*/
    rewind(fgestionNiveau);
    for(i=0; i<nbLigne; i++)
    {
        fscanf(fgestionNiveau, "%i %s %s\n", &carte->gestNiveau[i].id, carte->gestNiveau[i].fileName, carte->gestNiveau[i].minimap);
        //printf("%i %s %s\n", carte->gestNiveau[i].id, carte->gestNiveau[i].fileName, carte->gestNiveau[i].minimap);
    }
    fclose(fgestionNiveau);


}

//Fonction qui permet d'afficher les tile en fonction du tableau
void afficheMap(Map carte, SDL_Surface *ecran, ParamMap opt)
{
    SDL_Surface *surfaceBloc; //Pour affichage des bloc
    SDL_Rect posBloc; //Pour la position des bloc
    SDL_Rect posTile[carte.nbTile]; //Alloue un tableau au nombre de tiles utiliser

    surfaceBloc = IMG_Load(carte.filenameTileSet);
    //Decoupage des sprite du decors
    int i, j;
    for(i=0; i<carte.nbTile; i++)
    {
        posTile[i].x = LTILE * i;
        posTile[i].y = 0;
        posTile[i].w = LTILE;
        posTile[i].h = HTILE;
    }


    //Declaration de la grille
    SDL_Surface *surfaceGrille;
    SDL_Rect posGrille;
    surfaceGrille = IMG_Load(opt.filenameGrille);

    //Vaiseau
    SDL_Surface* vaisseau;
    SDL_Rect posVaisseau;
    vaisseau = IMG_Load("image/map/vaiseau.bmp");

    //MatHack: Optimisation au max pour charger la map seulement ce que
    //l'on vois à ecran et non pas tout les sprite en dehors de la fenetre
    int minCarteX;
    int minCarteY;
    int maxCarteX;
    int maxCarteY;
    //si on est dans editeur
    int bandeEditeur=0, bandeJeu=0;
    //Ajoute la hauteur de interface si on est dans editeur
    if(opt.editeur == 1)
    {
        bandeEditeur = InterfaceHauteur;
        bandeJeu = 0;
    }
    else
    {
        bandeJeu = bandeJeuHaut;
        bandeEditeur = 0;
    }

    //!System optimsation pour affichage
    //Largeur/Hauteur minimal
    minCarteX = carte.xscroll / LTILE;
    minCarteY = (carte.yscroll+bandeJeu) / HTILE;
    //On n'optimisie pas laffichage si on est plus petit que ecran
    //Largeur maxi
    if(carte.MONDE_LARGEUR*LTILE >  ecran->w+LTILE)
        maxCarteX = (carte.xscroll + ecran->w+LTILE) / LTILE; //On rajoute une case de 32 pour pas avoir un effect de sacade au déplacement
    else
        maxCarteX = carte.MONDE_LARGEUR;//-1:Car on veut pas afficher la ligne des \n
    //Hauteur maxi
    if(carte.MONDE_HAUTEUR*HTILE >  ecran->h+HTILE)
        maxCarteY = (carte.yscroll + ecran->h+HTILE) / HTILE; //On rajoute une case de 32 pour pas avoir un effect de sacade au déplacement
    else
        maxCarteY = (carte.MONDE_HAUTEUR);


//Boucle pour faire affichage à ecran0
    for(i=minCarteY; i<maxCarteY; i++)
    {
        for(j=minCarteX; j<maxCarteX-1; j++)//-1:Car on veut pas afficher la ligne des \n
        {
            //Recupère les position du bloc dans le tableau
            posBloc.x = j * LTILE - carte.xscroll;
            posBloc.y = i * HTILE - carte.yscroll;
            //printf("\n%i;%i\n", i, j);
            //Affichage du bloc en coordonnée X,Y et le choisie en fonction du numéro
            switch(carte.tableauMap[i][j])
            {
            case tileVIDE: //Aucun sprite
                break;
            case tileMUR: //Mur
                SDL_BlitSurface(surfaceBloc, &posTile[tileMUR-1], ecran, &posBloc);
                break;
            case tileBONUS: //Bonus
                SDL_BlitSurface(surfaceBloc, &posTile[tileBONUS-1], ecran, &posBloc);
                break;
            case tileSOL: //Sol notmal
                SDL_BlitSurface(surfaceBloc, &posTile[tileSOL-1], ecran, &posBloc);
                break;
            case tileReserve:
                SDL_BlitSurface(surfaceBloc, &posTile[tileReserve-1], ecran, &posBloc);
                break;
            case 'd': //Spawn
                /*posVaisseau = posBloc;
                posVaisseau.y -= vaisseau->h-30;
                posVaisseau.x -= vaisseau->w/2+30;
                SDL_BlitSurface(vaisseau, NULL, ecran, &posVaisseau);*/
                if(opt.editeur == 1)
                    SDL_BlitSurface(surfaceBloc, &posTile[tileSPAWN-1], ecran, &posBloc);
                //SDL_FreeSurface(vaisseau);
                break;
            case 'm': //Monstre
                if(opt.editeur == 1)
                    SDL_BlitSurface(surfaceBloc, &posTile[tileMONSTRE-1], ecran, &posBloc);
                break;
            case 'f': //Fin
                posVaisseau = posBloc;
                posVaisseau.y -= vaisseau->h-30;
                posVaisseau.x -= vaisseau->w/2+30;
                SDL_BlitSurface(vaisseau, NULL, ecran, &posVaisseau);
                if(opt.editeur == 1)
                    SDL_BlitSurface(surfaceBloc, &posTile[tileFIN-1], ecran, &posBloc);
                SDL_FreeSurface(vaisseau);
                break;
            case tileFIN: //Fin
                posVaisseau = posBloc;
                posVaisseau.y -= vaisseau->h-30;
                posVaisseau.x -= vaisseau->w/2+30;
                SDL_BlitSurface(vaisseau, NULL, ecran, &posVaisseau);
                if(opt.editeur == 1)
                    SDL_BlitSurface(surfaceBloc, &posTile[tileFIN-1], ecran, &posBloc);
                SDL_FreeSurface(vaisseau);
                break;
            case tileMONSTRE: //Monstre
                if(opt.editeur == 1)
                    SDL_BlitSurface(surfaceBloc, &posTile[tileMONSTRE-1], ecran, &posBloc);
                break;
            case tileSPAWN:
                /*posVaisseau = posBloc;
                posVaisseau.y -= vaisseau->h-30;
                posVaisseau.x -= vaisseau->w/2+30;
                SDL_BlitSurface(vaisseau, NULL, ecran, &posVaisseau);*/
                if(opt.editeur == 1)
                    SDL_BlitSurface(surfaceBloc, &posTile[tileSPAWN-1], ecran, &posBloc);
                //SDL_FreeSurface(vaisseau);
                break;
            default:
                break;
            }
            //Permet d'afficher une grille pour bien visualiser les emplacement et la taille du tableau
            if(opt.afficheGrille == 1)
            {
                posGrille.x = j * LTILE - carte.xscroll;
                posGrille.y = i * HTILE - carte.yscroll;
                SDL_BlitSurface(surfaceGrille, 0, ecran, &posGrille);
            }
        }

    }
//Libère la mémoire
    SDL_FreeSurface(surfaceBloc);
    SDL_FreeSurface(surfaceGrille);
    SDL_FreeSurface(vaisseau);
}

//Fonction qui permet de faire défiler le scrolling en fonction du personnage en Y
int scrollingPersoY(Perso mario, Map *carte, SDL_Surface *screen)
{
    if(mario.positionP.y+ mario.persoSprite->h>= (screen->h*0.75))
    {
        if(carte->yscroll < (carte->MONDE_HAUTEUR - 1)*HTILE - screen->h)
        {
            carte->yscroll += mario.vitesse;
        }
        else
            return 1;
    }
    else if(mario.positionP.y <= (screen->h/4))
    {
        if(carte->yscroll > 0)
        {
            carte->yscroll -= mario.vitesse;
        }
        else
            return 1;
    }
    else
        return 1;
    return 0;
}

//Fonction qui permet de faire défiler le scrolling en fonction du personnage en X
int scrollingPersoX(Perso mario, Map *carte, SDL_Surface *screen)
{
    if(mario.positionP.x >= (screen->w*0.75))
    {
        if(carte->xscroll < (carte->MONDE_LARGEUR - 1)*LTILE - screen->w)
        {
            carte->xscroll+= mario.vitesse;
        }
        else
            return 1;
    }
    else if(mario.positionP.x <= (screen->w/4))
    {
        if(carte->xscroll > 0)
        {
            carte->xscroll-=mario.vitesse;
        }
        else
            return 1;
    }
    else
        return 1;
    return 0;
}

//Fonction qui bloc le scrolling manuel (editeur)
void ScrollingDebordement(Map *carte, SDL_Surface *screen)
{
    if(carte->xscroll<0)
        carte->xscroll = 0;
    if(carte->yscroll<0)
        carte->yscroll = 0;
    //Savoir si on est trop loin c'est : largeur du monde * largeur d'un tile - longeur de la fenetre
    //Meme chose pour la hauteur!
    if(carte->MONDE_LARGEUR*LTILE>screen->w-carte->xscroll) //Si la largeur du tableau et plus grand que la fenetre
        if(carte->xscroll>(carte->MONDE_LARGEUR*LTILE)-screen->w)
            carte->xscroll = (carte->MONDE_LARGEUR*LTILE)-screen->w;
    if(carte->MONDE_HAUTEUR*HTILE>screen->h-carte->yscroll)
        if(carte->yscroll>(carte->MONDE_HAUTEUR*HTILE)-(screen->h - InterfaceHauteur))
            carte->yscroll = (carte->MONDE_HAUTEUR*HTILE)-(screen->h-InterfaceHauteur);

}
