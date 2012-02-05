#ifdef _cplusplus
#include <ctime>
#include <cstdio>
#include <cstdlib>
#else
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#endif //_cplusplus

#include "jeu.h"
#include "highscore.h"
#include "menu.h"
#include "ennemie.h"
#include "editeur.h"

#include <fmodex/fmod.h>

void associer_surface_pos(SDL_Surface *surface,SDL_Rect *position)
{
    position->h = surface->h;
    position->w = surface->w;
}
void initPosPerso(Perso *perso1,Map *carteJeu,SDL_Surface *screen)
{
    perso1->imagePerso = (char*)"image/perso/persoPara.bmp";       //saut en parachute
    SDL_FreeSurface(perso1->persoSprite);
    perso1->persoSprite = IMG_Load(perso1->imagePerso);
    associer_surface_pos(perso1->persoSprite,&perso1->positionP);
    perso1->positionP.x = (screen->w-perso1->persoSprite->w)/2;
    perso1->positionP.y = (screen->h-perso1->persoSprite->h)/2;
    carteJeu->xscroll = carteJeu->posDepart.x - perso1->persoSprite->w - perso1->positionP.x;
    carteJeu->yscroll = carteJeu->posDepart.y - perso1->persoSprite->h - perso1->positionP.y;
    ScrollingDebordement(carteJeu,screen);
    /*perso1->point_tab[0].x = 0;
    perso1->point_tab[0].y = 36;
    perso1->point_tab[1].x = 17;
    perso1->point_tab[1].y = 0;
    perso1->point_tab[2].x = 60;
    perso1->point_tab[2].y = 0;
    perso1->point_tab[3].x = 73;
    perso1->point_tab[3].y = 38;
    perso1->point_tab[4].x = 48;
    perso1->point_tab[4].y = 147;
    perso1->point_tab[5].x = 27;
    perso1->point_tab[5].y = 147;
    perso1->point_tab[6].x = 0;
    perso1->point_tab[6].y = 36;*/
}

void jeu(SDL_Surface* screen,Map carteJeu, ParamMap opt,SDL_Joystick *joystick)
{
    //Informations relatives à l'affichage du Pseudo, nombre de pièces et niveau;
    TTF_Font *police=NULL;
    SDL_Rect positionPiece, positionVie, positionPseudo, positionScore,positionPiece2, positionVie2;

    //MatHack:Alignement par rapport à l'ecran en haut avec la l'espace de la bar
    int hauteurTexte = 25;
    int tmp = 0;
    //1
    positionPseudo.x=10;
    positionPseudo.y=hauteurTexte;
    //2
    positionPiece2.x=screen->w - (3*(screen->w*0.25));
    positionPiece2.y=hauteurTexte-32/2;
    positionPiece.x=positionPiece2.x+32+5;
    positionPiece.y=hauteurTexte;
    //3
    positionVie2.x=screen->w - (2*(screen->w*0.25));
    positionVie2.y=hauteurTexte-32/2;
    positionVie.x= positionVie2.x+32+5;
    positionVie.y=hauteurTexte;
    //4
    positionScore.x=screen->w - (screen->w*0.25);
    positionScore.y=hauteurTexte;

    Joueur j;
    Score s;
    char nbPiece[4],nbVie[4],score[12] = "Score: ";
    police = TTF_OpenFont("pix.ttf", 20);
    SDL_Color couleur = {255, 255, 255};
    SDL_Surface *textePiece = TTF_RenderText_Blended(police, nbPiece, couleur), *texteVie = TTF_RenderText_Blended(police, nbVie, couleur), *textePseudo = TTF_RenderText_Blended(police, j.pseudo, couleur),*texteScore = TTF_RenderText_Blended(police, score, couleur), *imagePiece=NULL, *imageVie=NULL;
    imagePiece=IMG_Load("image/map/pieces.gif");
    imageVie=IMG_Load("image/map/han_solo.gif");


    //MatHack:On enleve le curseur, comme sa utilisateur cherchera pas a cliquer sur le menu
    SDL_ShowCursor(0);
    ParamMap options;
    //Affichage du fond
    SDL_Surface *fond;
    carteJeu.posFond.x = 0;
    carteJeu.posFond.y = 0;
    fond = IMG_Load("image/map/fondV4.gif");

    options.afficheGrille = 1;
    nouvellePartie(screen, &j, &s);

    FMOD_SYSTEM *system;
    FMOD_System_Create(&system);
    FMOD_System_Init(system, 1, FMOD_INIT_NORMAL, NULL);
    FMOD_SOUND *jet = NULL;
    FMOD_SOUND *tir = NULL;
    FMOD_SOUND *piece = NULL;
    FMOD_System_CreateSound(system, "son/piece.wav", FMOD_CREATESAMPLE, 0, &piece);
    FMOD_System_CreateSound(system, "son/jetpack.wav", FMOD_CREATESAMPLE, 0, &jet);
    FMOD_System_CreateSound(system, "son/laser.wav", FMOD_CREATESAMPLE, 0, &tir);

    options.afficheGrille = 0;

    SDL_EnableKeyRepeat(1,1);
    int tempsPrecedent = 0, tempsActuel = 0,continuer = 1,decalage = 0,tempsMusique1 = 0,tempsMusique2 = 0,retour = 0;
    int a_atteri = 1,touche_enfonce = 0;
    int touche_a_ete_enfonce = 0;
    Perso perso1;
    perso1.imagePerso = (char*)"image/perso/persoPara.bmp";
    perso1.persoSprite = IMG_Load(perso1.imagePerso);
    perso1.sensxp = 0;
    perso1.sensyp = 1;
    perso1.vitesse = 4;
    perso1.dir = 0;
    //MatHack: Permet de positionner le joueur en fonction de la lettre dans le fichier map
    initPosPerso(&perso1,&carteJeu,screen);
    int nbr_balle = 0,cpt_balle = 0,new_balle = 0,realloc_balle = 0,realloc_ennemi = 0,reiinit_ennemi = 0;
    Balle *mario_tire = (Balle*)malloc(sizeof(Balle));
    int jetPack = 0,cptJetPack = DUREE_JET_PACK;
    //initialisation du tableau dynamique d'ennemi
    Ennemi *tabEnnemi = (Ennemi*)malloc(sizeof(Ennemi)*carteJeu.nb_ennemi);
    int i;
    for(i = 0; i < carteJeu.nb_ennemi; i++)
        initEnnemi(carteJeu,&tabEnnemi[i],i,screen);
    //MatHack:Test d'ennemie

    //MatHack: Decoupage image blaster
    //Perso
    decoupePerso(&perso1);

    perso1.posSpriteBlaster[0].x = 0;
    perso1.posSpriteBlaster[0].y = 0;
    perso1.posSpriteBlaster[0].h = 8;
    perso1.posSpriteBlaster[0].w = 22;
    perso1.posSpriteBlaster[1].x = 22;
    perso1.posSpriteBlaster[1].y = 0;
    perso1.posSpriteBlaster[1].h = 8;
    perso1.posSpriteBlaster[1].w = 22;
    perso1.posSpriteBlaster[2].x = 22*2;
    perso1.posSpriteBlaster[2].y = 0;
    perso1.posSpriteBlaster[2].h = 8;
    perso1.posSpriteBlaster[2].w = 22;
    perso1.frameBlaster = 0;
    perso1.frameActu = 1;
    perso1.ok = 0;
    perso1.vitesseAnim = 10;
    int fonduOk = 1;
    affichageFonduJeu(screen, 10, &fonduOk, carteJeu, opt, fond);

    //Chargement de la map
    int joystickTouche = 0;


    while(continuer)
    {
        tempsActuel = SDL_GetTicks();
        if (tempsActuel - tempsPrecedent > FRAMERATE)
        {
            cpt_balle++;
            SDL_BlitSurface(fond, NULL, screen, &carteJeu.posFond);
            afficheMap(carteJeu,screen,options);
            //Actualiser le blaster en frame

            //Mis à jour du nombre de pièces, de vie et du score
            if(j.nbvie < 10)
            {
                nbVie[0]='x';
                nbVie[1]='0';
                nbVie[2]=j.nbvie+'0';
                nbVie[3]='\0';
            }
            else
            {
                nbVie[0]='x';
                nbVie[1]=j.nbvie/10 + '0';
                nbVie[2]=j.nbvie%10 +'0';
                nbVie[3]='\0';
            }
            if(j.nbpiece < 10)
            {
                nbPiece[0]='x';
                nbPiece[1]='0';
                nbPiece[2]=j.nbpiece+'0';
                nbPiece[3]='\0';
            }
            else
            {
                nbPiece[0]='x';
                nbPiece[1]=j.nbpiece/10 + '0';
                nbPiece[2]=j.nbpiece%10 +'0';
                nbPiece[3]='\0';
            }
            if(j.score < 10)
            {
                score[7]='0';
                score[8]='0';
                score[9]='0';
                score[10]=j.score+'0';
                score[11]='\0';
            }
            else if(j.score < 100)
            {
                score[7]='0';
                score[8]='0';
                score[9]=j.score/10+'0';
                score[10]=j.score%10  +'0';
                score[11]='\0';
            }
            else if(j.score < 1000)
            {
                score[7]='0';
                score[8]=j.score/100+'0';
                score[9]=(j.score%100)/10+'0';
                score[10]=j.score%10  +'0';
                score[11]='\0';
            }
            else
            {
                score[7]=j.score/1000+'0';
                score[8]=(j.score%1000)/100+'0';
                score[9]=(j.score%100)/10+'0';
                score[10]=j.score%10  +'0';
                score[11]='\0';
            }

            //Affichage de tous les textes d'état (score, vie, pièce, pseudo)
            textePseudo = TTF_RenderText_Blended(police, j.pseudo, couleur);
            SDL_BlitSurface(textePseudo, NULL, screen, &positionPseudo);
            SDL_FreeSurface(textePseudo);
            textePiece = TTF_RenderText_Blended(police, nbPiece, couleur);
            SDL_BlitSurface(textePiece, NULL, screen, &positionPiece);
            SDL_FreeSurface(textePiece);
            texteVie = TTF_RenderText_Blended(police, nbVie, couleur);
            SDL_BlitSurface(texteVie, NULL, screen, &positionVie);
            SDL_FreeSurface(texteVie);
            texteScore = TTF_RenderText_Blended(police, score, couleur);
            SDL_BlitSurface(texteScore, NULL, screen, &positionScore);
            SDL_FreeSurface(texteScore);
            SDL_BlitSurface(imagePiece, NULL, screen, &positionPiece2);
            SDL_BlitSurface(imageVie, NULL, screen, &positionVie2);

            if(a_atteri==0 && strcmp(perso1.imagePerso,"image/perso/persoPara.bmp") == 0)   //si le perso en parachute a atteri
            {
                /*if(perso1.dir == 1)
                {
                    perso1.point_tab[0].x = 2;
                    perso1.point_tab[0].y = 11;
                    perso1.point_tab[1].x = 34;
                    perso1.point_tab[1].y = 5;
                    perso1.point_tab[2].x = 48;
                    perso1.point_tab[2].y = 24;
                    perso1.point_tab[3].x = 52;
                    perso1.point_tab[3].y = 56;
                    perso1.point_tab[4].x = 46;
                    perso1.point_tab[4].y = 55;
                    perso1.point_tab[5].x = 29;
                    perso1.point_tab[5].y = 48;
                    perso1.point_tab[6].x = 2;
                    perso1.point_tab[6].y = 11;
                }*/
                tmp = perso1.persoSprite->h;
                SDL_FreeSurface(perso1.persoSprite);
                perso1.imagePerso = (char*)"image/perso/persoV3.gif";   //perso mode normal sans parachute
                perso1.persoSprite = IMG_Load(perso1.imagePerso);
                associer_surface_pos(perso1.persoSprite,&perso1.positionP);
                perso1.positionP.y += (tmp-perso1.persoSprite->h-1);
                perso1.positionP.x += 27;
            }

            //MatHack:Permet de blit la surface si il est en laire ou si il est en animation au sol
            if(strcmp(perso1.imagePerso,"image/perso/persoPara.bmp") == 0)
                SDL_BlitSurface(perso1.persoSprite, NULL, screen, &perso1.positionP);
            else
            {
                SDL_BlitSurface(perso1.persoSprite, &perso1.posSprite[perso1.frameActu], screen, &perso1.positionP);
                perso1.positionP.w = perso1.posSprite[perso1.frameActu].w;
                perso1.positionP.h = perso1.posSprite[perso1.frameActu].h;
                retour = test_collision(perso1.positionP,&carteJeu,0,0,NULL,&perso1,0);
                while(retour == MUR_COLLISION)
                {
                    perso1.positionP.x -= 2;
                    retour = test_collision(perso1.positionP,&carteJeu,0,0,NULL,&perso1,0);
                }
            }
            affiche_laser(mario_tire,&nbr_balle,&carteJeu,screen,&realloc_balle, perso1,&j);    //affichage du blaster
            for(i = 0; i < carteJeu.nb_ennemi; i++)     //deplacement de tout les ennemis
                deplace_ennemi(&tabEnnemi[i],carteJeu,screen,perso1);

            //Affiche la version en cours
            versionTexte();

            SDL_Flip(screen);

            gestion_touche(&perso1, &carteJeu, &continuer,&a_atteri,&touche_enfonce,&touche_a_ete_enfonce,&new_balle,&cpt_balle,&jetPack,&cptJetPack,joystick,&joystickTouche);
            if(jetPack==1 && a_atteri == 1)
            {
                tempsMusique2 = SDL_GetTicks();
                if(tempsMusique2-tempsMusique1 > 1000)
                {
                    FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, jet, 0, NULL);
                    tempsMusique1 = tempsMusique2;
                }
            }
            if(new_balle==1)
            {
                FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, tir, 0, NULL);
            }
            deplace_mario(&perso1,&carteJeu,decalage,screen,&a_atteri,&j,&continuer,system,piece,&s,tabEnnemi,&reiinit_ennemi);
            if(reiinit_ennemi)
            {
                tabEnnemi = (Ennemi*)realloc(tabEnnemi,sizeof(Ennemi)*carteJeu.nb_ennemi);
                int i;
                for(i = 0; i < carteJeu.nb_ennemi; i++)
                    initEnnemi(carteJeu,&tabEnnemi[i],i,screen);
                reiinit_ennemi = 0;
            }
            test_collision_ennemi(tabEnnemi,&perso1,mario_tire,carteJeu.nb_ennemi,nbr_balle,&realloc_ennemi,&realloc_balle,&carteJeu,&a_atteri,&continuer,&j,screen);  //test de collision entre perso ennemis et blaster
            touche_a_ete_enfonce = 0;
            if(perso1.dir == 0)
                perso1.frameActu = 0;
            if(cptJetPack < DUREE_JET_PACK && jetPack == 0)     //incrementation cpt jet pack pour une duree de quelques sec
                cptJetPack++;
            if(jetPack == 1)
            {
                perso1.frameActu = 0;
                cptJetPack -= 2;
                if(cptJetPack < 0)
                {
                    cptJetPack = 0;
                }
            }
            if(realloc_balle != 0)  //suppression d'un blaster n°realloc_balle dans le tableau des blasters
            {
                nbr_balle--;
                for(i = realloc_balle; i < (nbr_balle); i++)
                {
                    mario_tire[i-1] = mario_tire[i];    //deplacement du tableau pour ecraser la balle a supprimer
                }
                mario_tire = (Balle*)realloc(mario_tire,(nbr_balle)*sizeof(Balle)); // reallocation du tableau de blaster
                realloc_balle = 0;
            }
            if(realloc_ennemi != 0) //suppression d'un ennemi n°realloc_ennemi dans le tab des ennemis
            {
                carteJeu.nb_ennemi--;
                j.score += 20;
                for(i = realloc_ennemi; i < (carteJeu.nb_ennemi+1); i++)
                {
                    tabEnnemi[i-1] = tabEnnemi[i];  //deplacement du tableau pour ecraser l'ennemi a supprimer
                }
                tabEnnemi = (Ennemi*)realloc(tabEnnemi,(carteJeu.nb_ennemi)*sizeof(Ennemi));    //reallocation du tab d'ennemi
                realloc_ennemi = 0;
            }
            if(new_balle == 1)      //ajout d'un blaster (tir)
            {
                nbr_balle++;
                mario_tire = (Balle*)realloc(mario_tire,(nbr_balle)*sizeof(Balle)); //realloc tab avec 1 case en plus
                mario_tire[nbr_balle-1].positionB.x = perso1.positionP.x;   //init du X blaster a la valeur du X du perso
                if(perso1.dir < 0)      //si le perso va vers la gauche
                {
                    mario_tire[nbr_balle-1].vitesseX = perso1.dir*10;       //init vitesse
                    //Tire en diagonale désactiver pour le moment
                    //if(a_atteri == 1)
                    // mario_tire[nbr_balle-yscroll1].vitesseY = -10;     //si le perso est en l'air on tire en diagonale
                    //else
                    //mario_tire[nbr_balle-1].vitesseY = 0;   //sinon tire horizontale
                    mario_tire[nbr_balle-1].positionB.y = perso1.positionP.y+8;    //init du Y du blaster
                    mario_tire[nbr_balle-1].positionB.x -= 20;      //-20 pour le design
                    mario_tire[nbr_balle-1].imageBalle = (char*)"image/perso/blaster_tire_g.gif";
                }
                else if(perso1.dir > 0) //si le perso va vers la droite
                {
                    mario_tire[nbr_balle-1].vitesseX = perso1.dir*10;
                    mario_tire[nbr_balle-1].positionB.x += 50;
                    //Tire en diagonale désactiver pour le moment
                    //if(a_atteri == 1)
                    // mario_tire[nbr_balle-1].vitesseY = -10;
                    //else
                    //mario_tire[nbr_balle-1].vitesseY = 0;
                    mario_tire[nbr_balle-1].positionB.y = perso1.positionP.y+8;
                    mario_tire[nbr_balle-1].imageBalle = (char*)"image/perso/blaster_tire_d.gif";
                }
                mario_tire[nbr_balle-1].balleSprite = IMG_Load(mario_tire[nbr_balle-1].imageBalle); //chargement de l'image
                associer_surface_pos(mario_tire[nbr_balle-1].balleSprite,&mario_tire[nbr_balle-1].positionB);
                cpt_balle = 0;
                new_balle = 0;

            }
            if((touche_enfonce != -1 && touche_enfonce != 1))   //si l'utilisateur n'appuie pas sur gauche ou droite
            {
                if(perso1.sensxp > 1)   //si il va a droite
                    perso1.sensxp -= 1; //alors on diminue son acceleration pour atteidre 0
                else if(perso1.sensxp < -1) //idem a gauche
                    perso1.sensxp += 1;
                else if(perso1.sensxp >= -1 && perso1.sensxp <= 1)  //si son acceleration est proche de 0, on la met a 0
                    perso1.sensxp = 0;
            }
            if((jetPack == 0 && cptJetPack > 0) && (perso1.sensyp > 0 || (a_atteri == 1 && perso1.sensyp > -1)))    //si le jet pack n'est pas actif et si le perso va vers le bas ou qu'il soit en l'air et va vers le haut
            {
                perso1.sensyp = 4;      //alors on le fait redescendre
            }
            else if(perso1.sensyp < 0)  //sinon on decremente l'acceleration en Y
            {
                perso1.sensyp += 0.2;
            }
            tempsPrecedent = tempsActuel;
        }
    }

    s.tabscore = j.score;
    strcpy(s.pseudo, j.pseudo);
    s.temps = time(NULL);
    sauvegardeScore(s);

    free(mario_tire);
    free(tabEnnemi);
    SDL_FreeSurface(imagePiece);
    SDL_FreeSurface(imageVie);
    SDL_FreeSurface(perso1.persoSprite);
    SDL_EnableKeyRepeat(0,0);

    FMOD_Sound_Release(jet);
    FMOD_Sound_Release(tir);
    FMOD_Sound_Release(piece);

    FMOD_System_Close(system);
    FMOD_System_Release(system);
}

//MatHack:Création d'un fondu
void affichageFonduJeu(SDL_Surface *ecran, int temps, int *fonduOk, Map carte, ParamMap opt, SDL_Surface *fond)
{
    SDL_Rect pos;
    pos.x = 0;
    pos.y = 0;
    if(*fonduOk == 1)
    {
        opt.afficheGrille = 0;
        int i;
        SDL_Surface *fondu;
        SDL_Rect posFondu;
        posFondu.x = 0;
        posFondu.y = 0;
        fondu = SDL_CreateRGBSurface(SDL_HWSURFACE, ecran->w, ecran->h, 32, 0,0,0,0);
        SDL_FillRect(fondu, NULL, SDL_MapRGB(fondu->format, 0,0,0));
        for(i=255; i>0; i--)
        {
            SDL_BlitSurface(fond, NULL, ecran, &pos);
            afficheMap(carte,ecran,opt);
            SDL_SetAlpha(fondu, SDL_SRCALPHA, i);
            SDL_BlitSurface(fondu, NULL, ecran, &pos);

            SDL_Flip(ecran);
            SDL_Delay(temps);
        }
        SDL_FreeSurface(fondu);
        *fonduOk = 0;
    }

}

void nouvellePartie(SDL_Surface* screen, Joueur *j, Score *sc)
{
    int doublon=0;
    do
    {
        doublon=0;
        enregistrerJoueur(j, screen);
    }
    while(doublon == 1);
    j->nbvie=3;
    j->niveau=1;
    j->savegardePos=0;
    j->nbpiece=0;
    j->score=0;
}

void enregistrerJoueur(Joueur *j, SDL_Surface* screen)
{
    TTF_Font *police=NULL,*police2=NULL;
    SDL_Surface *texte=NULL;
    SDL_Color couleur = {255, 255, 255};
    SDL_Color couleurRouge = {255, 0, 0};
    char lettre1='A', lettre2='A', lettre3='A';
    int numlettre=1;
    SDL_Event event;
    char pseudo[4]="AAA";
    int continuer=1;
    //police = TTF_OpenFont("pix.ttf", 60);
    police = TTF_OpenFont("pix.ttf", 60);
    police2 = TTF_OpenFont("calibri.ttf", 30);

    SDL_Rect position,positiontexte,positionFond,positionCaraActif;
    positionFond.x=0;
    positionFond.y=0;
    SDL_Surface *imageDeFond = NULL,*textePseudo=NULL, *texteActif=NULL;

    position.y = screen->h/2 - (60/2); //MatHack : Moitier de la fenetre - moitier du Texte = centrer
    position.x = screen->w/2 - ((3*60)/2.8); //MatHack :3 * taille 1 lettre / 2.8 car espace entre les lettre
    //MatHack: Sert pas si bien positionner avant
    if(screen->w == 1024)
    {
        positiontexte.x=screen->h/2 + ((5*30)/1.8);
        positiontexte.y =screen->w/2-(30/2)-220;
    }
    else
    {
        positiontexte.x=screen->h/4+100;
        positiontexte.y =screen->w/2-screen->w/6-100;
    }
    texteActif=TTF_RenderText_Blended(police2, "**", couleurRouge);
    textePseudo=TTF_RenderText_Blended(police2, "Pseudo", couleurRouge);
    SDL_BlitSurface(textePseudo,0,screen,&positiontexte);
    positionCaraActif.y = screen->h/2 - (60); //MatHack : Moitier de la fenetre - moitier du Texte = centrer
    positionCaraActif.x = screen->w/2 - ((3*60)/2.8);
    //MatHack:Ce flip sert à rien, surcharge
    SDL_EnableKeyRepeat(100,100);
    while(continuer)
    {

        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            TTF_Quit();
            SDL_Quit();
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_UP:
                switch(numlettre)
                {
                case 1:
                    switch(lettre1) //ALgo pour n'afficher que les majuscules, les minuscules et les chiffres.
                    {
                    case 'Z':
                        lettre1='a';
                        break;
                    case 'z':
                        lettre1='1';
                        break;
                    case '9':
                        lettre1='A';
                        break;
                    default:
                        lettre1++;
                        break;
                    }
                    break;
                case 2:
                    switch(lettre2)
                    {
                    case 'Z':
                        lettre2='a';
                        break;
                    case 'z':
                        lettre2='1';
                        break;
                    case '9':
                        lettre2='A';
                        break;
                    default:
                        lettre2++;
                        break;
                    }
                    break;
                case 3:
                    switch(lettre3)
                    {
                    case 'Z':
                        lettre3='a';
                        break;
                    case 'z':
                        lettre3='1';
                        break;
                    case '9':
                        lettre3='A';
                        break;
                    default:
                        lettre3++;
                        break;
                    }
                    break;
                default:
                    break;
                }
                break;
            case SDLK_DOWN:
                switch(numlettre)
                {
                case 1:
                    switch(lettre1)
                    {
                    case 'A':
                        lettre1='9';
                        break;
                    case '1':
                        lettre1='z';
                        break;
                    case 'a':
                        lettre1='Z';
                        break;
                    default:
                        lettre1--;
                        break;
                    }
                    break;
                case 2:
                    switch(lettre2)
                    {
                    case 'A':
                        lettre2='9';
                        break;
                    case '1':
                        lettre2='z';
                        break;
                    case 'a':
                        lettre2='Z';
                        break;
                    default:
                        lettre2--;
                        break;
                    }
                    break;
                case 3:
                    switch(lettre3)
                    {
                    case 'A':
                        lettre3='9';
                        break;
                    case '1':
                        lettre3='z';
                        break;
                    case 'a':
                        lettre3='Z';
                        break;
                    default:
                        lettre3--;
                        break;
                    }
                    break;
                default:
                    break;
                }
                break;
            case SDLK_RETURN: // Quand on appuie sur espace, on change de lettre, une fois toutes les lettres passées on quitte
                if(numlettre < 3)
                {
                    positionCaraActif.x += 50;
                    numlettre++;
                }
                else
                    continuer=0;
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
        //Affichage du pseudo en cours
        pseudo[0]=lettre1;
        pseudo[1]=lettre2;
        pseudo[2]=lettre3;
        textePseudo=TTF_RenderText_Blended(police2, "Pseudo", couleurRouge);
        SDL_BlitSurface(textePseudo,0,screen,&positiontexte);
        SDL_FreeSurface(textePseudo);

        SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 255,255, 255));

        if(screen->w == 1024)
        {
            imageDeFond = IMG_Load("image/menu1024.bmp");
            SDL_BlitSurface(imageDeFond, 0, screen, &positionFond);
            SDL_FreeSurface(imageDeFond);
        }
        else if (screen->w == 800)
        {
            imageDeFond = IMG_Load("image/menu800.bmp");
            SDL_BlitSurface(imageDeFond, 0, screen, &positionFond);
            SDL_FreeSurface(imageDeFond);
        }
        else
        {
            imageDeFond = IMG_Load("image/menu640.bmp");
            SDL_BlitSurface(imageDeFond, 0, screen, &positionFond);
            SDL_FreeSurface(imageDeFond);
        }
        //Texte du pseudo
        texteActif=TTF_RenderText_Blended(police2, "**", couleurRouge);
        SDL_BlitSurface(texteActif, 0, screen, &positionCaraActif);
        SDL_FreeSurface(texteActif);

        textePseudo=TTF_RenderText_Blended(police2, "Pseudo", couleurRouge);
        SDL_BlitSurface(textePseudo,0,screen,&positiontexte);
        SDL_FreeSurface(textePseudo);

        texte = TTF_RenderText_Blended(police, pseudo, couleur);
        SDL_BlitSurface(texte, NULL, screen, &position);
        SDL_FreeSurface(texte);
        SDL_Flip(screen);
    }
    TTF_CloseFont(police);
    TTF_CloseFont(police2);
    strcpy(j->pseudo, pseudo); //ON sauvegarde le pseudo

}
