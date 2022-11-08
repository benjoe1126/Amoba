#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "karakterek.h"
#include "ablak.h"

void FreeBoard(Karakterek **Board,int meret){
    for(int i=0;i<meret;i++)
        free(Board[i]);
    free(Board);
    //Mivel a tömböm 2 dimenziós így felszabadításkor először a tömb elemeit szabadítom fel, akik maguk is tömbök, majd ha ez megvan a tömbök tömbjét is fel lehet szabadítani
}
void Board_reset(int hossz,Karakterek (**tomb)){
    for(int i=0;hossz>i;i++){
        for(int j=0;hossz>j;j++){
            tomb[i][j]=EMPTY;
        }
    }
    /*Mindössze annyi történik itt, hogy a tömb elemeit csupa EMPTY karakterre írom át,
    ugyanis ha ezt nem tenném akkor csak memóriaszemét kerülne bele és a játék játszhatatlan lenne*/
}
bool Tele(Karakterek **board,int meret){
    for(int i=0;meret>i;i++){
        for(int j=0;meret>j;j++)
            if(board[i][j]==EMPTY)
            return false;
    }
    return true;
    /*Ez a függvény a döntetlen helyzet kezelésére van. Onnan lehet tudni egy játékról, hogy döntetlen, hogy tele van a Board és senki sem nyert.
    EZ a függvény megnézi, hogy van e EMPTY karakter még a boardban, ha nem, igazzal tér vissza.*/
}
bool vizszintes_check(Karakterek**tabla,int meret,Karakterek kicsoda,int sor,int kell){
    int vizcount=0;
     for(int i=0;meret>i;i++){

        if(tabla[sor][i]==kicsoda){ //a sor van "lefixálva és az oszlop változik, sort ellenőriz
            vizcount++;
             if(vizcount==kell)
             return true;
        }

        else
            vizcount=0;
     }

     return false;
}
bool oszlop_check(Karakterek **tabla,int meret,Karakterek kicsoda,int oszlop, int kell){
   int ocount=0;
   for(int i=0;meret>i;i++){
   if(tabla[i][oszlop]==kicsoda){ //az oszlop van "lefixálva" és a sor változik, oszlopot ellenőriz
            ocount++;
            if(ocount==kell)
        return true;

   }
        else
            ocount=0;
   }

    return false;
}
bool Atlo_1(Karakterek **tabla,const int meret,const Karakterek kicsoda,int sor,int oszlop,int kell){//elmegy az átló végpontjára és onnan megnézi egyesével léptetve, hogy kijön e a kombináció, ha szakad, akkor az nem teljes így a megszámot nullázza

   int megszam=0; //Működési elve hasonlít a botéhoz, elmegy egy átló végpontjára és onnan nézi, hogy kijött e a győztes kombináció
   while(sor!=0&&oszlop!=0){
    sor--;oszlop--;
   }
   while(sor!=meret&&oszlop!=meret){
    if(tabla[sor][oszlop]==kicsoda)
        megszam++;
    else
        megszam=0;
    if(megszam==kell)
        return true;
    sor++;oszlop++;
   }
   return false;


}
bool Atlo_2(Karakterek **tabla,const int meret,const Karakterek kicsoda,int sor,int oszlop,int kell){ //megnézi a másik átlót

    int megszam=0;  //Ugyan az, mint az első, csak itt másik átlót jár be
    while(sor!=meret-1&&oszlop!=0){
        sor++;oszlop--;
    }
    while(sor!=-1&&oszlop!=meret){
    if(tabla[sor][oszlop]==kicsoda)
        megszam++;

    else
        megszam=0;
    sor--;oszlop++;
    if(megszam==kell)
        return true;
   }
   return false;
}


bool Board_State_Check(Karakterek **tabla,const int meret,const Karakterek kicsoda,int sor,int oszlop, int kell){
 return(Atlo_1(tabla,meret,kicsoda,sor,oszlop,kell)||Atlo_2(tabla,meret,kicsoda,sor,oszlop,kell)||
        vizszintes_check(tabla,meret,kicsoda,sor,kell)||oszlop_check(tabla,meret,kicsoda,oszlop,kell));
 /*Ez a függvény összegzi a fent leírt függvényeket, ha bármilyen olyan állapotot talál, ahol vége a játéknak
  (bármelyik függvény igazat ad vissza) akkor ő maga is igazat ad vissza,
   ebben az esetben a főprogramban lekezelem, hogy ekkor mi történjen.*/
}
Karakterek **lefoglal(int meret){
      Karakterek **tomb=(Karakterek**) malloc(meret*sizeof(Karakterek*));
    for(int i=0;meret>i;++i)
    {
        tomb[i]=(Karakterek*) malloc(meret*sizeof(Karakterek)); //Itt történik meg a dinamikus memóriakezelés, a játéks inputjának megfelelő tömböt hoz létre, ez a játéktérnek az alapja.
    }
    return tomb; //Visszatérés az így lefoglalt memóriacímmel
}



