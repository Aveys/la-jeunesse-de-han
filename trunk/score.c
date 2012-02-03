#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct
{
    char pseudo[4];
    int highscore;
    int scoreActuel;
    int tabscore[10];
} Score;

typedef struct
{
    char pseudo[4];
    int nbvie;
    int niveau;
    int savegardePos;
    int nbpiece;
} Joueur;

int main()
{
    FILE *fichier=fopen("joueur.txt", "rb");
    Score s[3], s2[3];
    int highscore[3];
    int k, a=0, inter;
    s2[0].highscore=0;
    s2[1].highscore=0;
    s2[2].highscore=0;
    int i,j;
    int nouveauHighScore=0;
    s[0].highscore=400;
    s[1].highscore=600;
    s[2].highscore=200;

    for(i=0;i<3;i++)
    {
        for(j=0;j<3;j++)
        {
            s[i].pseudo[j]='A';
            highscore[j]=0;
        }
        s[i].pseudo[3]='\0';
    }

    fwrite(s, sizeof(Score), 3, fichier);
    fread(&s2, sizeof(Score),3,fichier);

    do
    {
        for(k=a;k<=3;k++)
        {
            if(s[a].highscore>s[k].highscore)
            {
                inter=s[k].highscore;
                s[k].highscore=s[a].highscore;
                s[a].highscore=inter;
            }

        }
        a++;
    }while(a!=3);

    for(i=0;i<3;i++)
    {
        printf("highscore : %i :: Pseudo : %s\n", s[i].highscore, s2[i].pseudo);
    }
    fclose(fichier);
    return 0;
}
