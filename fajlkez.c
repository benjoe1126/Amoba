#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ablak.h"
bool Kiir (Game *ki){ //A fájlba író függvény
FILE *toltes;
toltes=fopen("Assets/jatekallas.txt","wt");
if(toltes==NULL){
    perror("Nem sikerült a fájl létrehozása");
    return false;
    }
fprintf(toltes,("%d %d %d %d %d"),ki->jatekpont,ki->geppont,ki->meret,ki->dif,ki->kell);
fclose(toltes);
return true;
}
bool Beolv(Game *betoltom){
    FILE *olvasas=fopen("Assets/jatekallas.txt","r");
    if(olvasas==NULL){
        perror("Nem lehet olvasni a szovegfajlt, mert nem letezik...duh");//duh
        return false;
        }
    fscanf(olvasas,"%d %d %d %d %d",&betoltom->jatekpont,&betoltom->geppont,&betoltom->meret,&betoltom->dif,&betoltom->kell);
    fclose(olvasas);
    return true;
}

