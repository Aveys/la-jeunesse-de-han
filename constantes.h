#ifndef DEF_CONSTANTES
#define DEF_CONSTANTES
#ifdef __cplusplus
#include <cstdlib>
#include <cstdio>
#include <ctime>
#else
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#endif
#ifdef __APPLE__
#include <SDL/SDL.h>
#else
#include <SDL.h>
#include <SDL_image.h>
#include <SDL/SDL_ttf.h>
#endif
//Define editeur/map
#define     TAILLE_FENETRE_X    5
#define     TAILLE_FENETRE_Y    5
#define     InterfaceHauteur    150
#define     bandeJeuHaut        70
#define     bandeJeuBas         0

#define     FRAMERATE   20 //50 fps
#define     FRAMERATE_BALLE 100
#define     LTILE   32
#define     HTILE   32
#define     DUREE_JET_PACK 125
#define     HPARA          148
#define     HENNEMIE       56
#define     LENNEMIE       23
#define     NB_PT           6
#define     JOY            3200

//DEBUG
#define     debugOutputMap      1

/* Perso
* structure du personnage
* SDL_Surface *persoSprite : surface du sprite du personnage
* char *imagePerso : chemin de l'image
* SDl_Rect positionP : structure de collision
* int vitesse : coefficient de vitesse du personnage
* double sensxp : direction en abscisse du personnage
* double sensyp : direction en ordonn�e du personnage
* SDL_Rect posSprite[16] : tableau des diff�rentes position du personnage
* int frameActu : image actuelle du sprite
* int dir : direction
* SDL_Rect posSpriteBlaster[3] : tableau des diff�rentes position du blaster
* int frameBlaster : image actuelle du sprite blaster
* int ok : temps n�cessaire � l'animation
* int vitesseAnim : coefficient de vitesse de l'animation
*/
struct Point
{
  float x,y;
};

struct Vecteur
{
  float x,y;
};

typedef struct
{
    SDL_Surface *persoSprite;
    char *imagePerso;
    SDL_Rect positionP;
    int vitesse;
    double sensxp;
    double sensyp;
    int nbSprite;
    SDL_Rect posSprite[16];
    int frameActu;
    int dir;
    SDL_Rect posSpriteBlaster[3];
    int frameBlaster;
    int ok;
    int vitesseAnim;
    Point point_tab[];
} Perso;

/* Balle
* propri�t�s des tirs du personnage
* SDL_Surface *balleSprite : surface du sprite de la balle
* char *imageBalle : chemin de l'image
* SDL_Rect positionB : structure de collision
* int vitesseX : vecteur vitesse d'abscisse de la balle
* int vitesseY : vecteur vitesse d'ordonn�e de la balle
*/
typedef struct
{
    SDL_Surface *balleSprite;
    char *imageBalle;
    SDL_Rect positionB;
    double vitesseX;
    double vitesseY;
    double x;
    double y;
} Balle;

/* BlocProp
* propri�t�s des blocs
* SDL_Rect posBloc : structure de collision
* int mur : bool�en d�finissant si c'est un mur ou non
* int cassable : bool�en d�finissant si il est cassable ou non
* int bonus : bool�en d�finissant si c'est un bonus
*/
typedef struct
{
    SDL_Rect posBloc;
    int mur;
    int cassable;
    int bonus;
} BlocProp;

/*MatHack:
Structure pour la gestion des niveau
id : C'est le numero de la carte
filename : C'est le fichier � charger pour avoir cette carte
minimap : C'est le fichier image de la carte
*/
typedef struct
{
    int id;
    char fileName[50];
    char minimap[50];
} GestionNiveau;

/*MatHack:
*Structure de Map
*filenameTileSet : Nom du fichier � charger pour les texture du d�cors
*filenameLevel : Nom du fichier � charger pour les diff�rent level (1 ligne = 1 level)
*level : Indique le level en cours ou � charger
*monde largeur/hauteur : hauteur et largeur de la map en tiles
*TableauMap : Tableau de la map actuellement en cour de jeu ou modification
*xscroll,yscroll : Permet de mettre les valeur
*posDepart : C'est la position en SDL_Rect du joueur
*nbTile : c'est le nombre different de tile
*posDepEnnemi : C'est la position en SDL_Rect du depart des ennemi
*nb_ennemi : C'est le nombre d'�nnemie actuel
*spriteSelect : c'est le sprite selectionner pour le perso
*/
typedef struct
{
    char filenameTileSet[30]; //charge le nom du fichier des sprites du d�cors (ex:tileset.jpg)
    char filenameLevel[30]; //Nom du fichier txt pour ouvrir les level
    int level;
    int LARGEUR_TILE, HAUTEUR_TILE;
    int MONDE_LARGEUR, MONDE_HAUTEUR;
    //Hack: Pour demo de lundi
    int tableauMap[100][1000]; //int **tableauMap;
    int xscroll,yscroll;
    SDL_Rect posDepart;
    int nbTile;
    SDL_Rect *posDepEnnemi;
    int nb_ennemi;
    char spriteSelect[30];
    int nbNiveau;
    GestionNiveau gestNiveau[10];
    SDL_Rect posFond;
} Map;



/* Fenetre
*structure n�cessaire � l'init/redimensionnement de la fenetre
* unsigned int width : largeur de la fenetre
* unsigned int height : hauteur de la fenetre
* unsigned int couleur : pixel range
* int fullscreen : bool�en indiquant si on joue en plein �cran ou non
*/
typedef struct
{
    unsigned int width;
    unsigned int height;
    unsigned int couleur;
    int fullscreen;
} Fenetre;


/* ParamMap
* informe sur les parametres sp�ciaux de la map
* int afficheGrille : boul�en indiquant si on affiche ou non la grille;
* int editeur : bool�en indiquant si on est en mode �diteur ou non
* char filenameGrille[45] : informe sur le nom du fichier .txt de niveau
*/
typedef struct
{
    int afficheGrille;
    int editeur;
    char filenameGrille[45];
    int helpInterface;
} ParamMap;

/* Ennemi
* SDL_Surface *EnnemiSprite : surface du sprite de l'ennemi
* SDL_Rect positionE : structure de collision
* int sensxe : vecteur de direction d'abscisse de l'ennemi
* int sensye : vecteur de direction d'ordonn�e de l'ennemi
*/
typedef struct
{
    SDL_Surface *EnnemiSprite;
    SDL_Rect positionE;
    int sensxe;
    int sensye;
    Balle balleEnnemi;
} Ennemi;

/* RETOUR_COLLISION
* �num�ration informant sur le type de collision
*/
typedef enum
{
    MUR_COLLISION = 1,
    ATTERI = 2,
    BONUS = 10,
    TROU = 100,
    FIN_MAP = 1000,
    FIN_LVL = 10000
} RETOUR_COLLISION;

/* BlocType
* �num�ration informant sur le type de bloc
*/
typedef enum
{
    tileVIDE = 0, //0
    tileMUR = 1, //1
    tileBONUS = 2, //2
    tileSOL = 3, //3
    tileReserve = 4, //4
    tileSPAWN = 5, //5 = S
    tileMONSTRE = 6,
    tileFIN = 7
} BlocType;

/* Score
* structure contenant les informations des highscores et du score du joueur
* char pseudo[4] : chaine du pseudo du joueur
* int tabscore : score du joueur
* time_t temps : date (format timestamp) du score enregistr�
*/
typedef struct
{
    char pseudo[4];
    int tabscore;
    time_t temps;
} Score;

/* Joueur
* structure contenant des informations sur le joueur courant
* char pseudo[4] : pseudo du joueur
* int nbVie : nombre de vies restante
* int niveau : niveau en cours du jeu
* int savegardePos : compteur de point de sauvegarde (*//** variable non utilis�e*//*)
* int nbPiece : nombre de pi�ces d�tenues par le joueur
* int score : score courant du jour
*/
typedef struct
{
    char pseudo[4];
    int nbvie;
    int niveau;
    int savegardePos;
    int nbpiece;
    int score;
} Joueur;
#endif
