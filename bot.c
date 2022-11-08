#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "karakterek.h"
#include "ablak.h"

bool kb_viszonyitva_rak(Karakterek **tomb,int meret, int *soro,int *oszlopo){
    int sor=*soro;
    int oszlop=*oszlopo;
    if(sor-1<0)
        sor=1;
    if(sor+1==meret)
        sor=meret-2;
    if(oszlop-1<0)
        oszlop=1;
    if(oszlop+1==meret)
        oszlop=meret-2;
    //A fentiek kezelése azért történik meg, hogy túlindexelés ne történhessen meg, mivel ugyanis a függvény "körbenézi" az utolsó cella körüli cellákat így megeshet, hogy invalid cellákat is megnéz, ahol viszont nem biztos, hogy nincs megfelelő érték, ezért ezt el kell kerülni

    for(int i=sor-1;sor+2>i;i++){
        for(int j=oszlop-1;oszlop+2>j;j++){
            if(tomb[i][j]==EMPTY){
                tomb[i][j]=O;
                *soro=i;
                *oszlopo=j;
                return true; //végigjárja a cella körüli cellákat és az első üres helyre rak, ha ez megtörtént akkor igazat ad vissza, ha nem, hamisat, ez kell azért, hogy több rajzoló függvény tudjak kezelni
            }
        }
    }
    return false;
}
bool rak(Karakterek **tomb,int meret,int *sor,int *oszlop){
    for(int i=0;meret>i;i++){
        for(int j=0;meret>j;j++){
            if(tomb[i][j]==EMPTY){
                tomb[i][j]=O;
                *sor=i; *oszlop=j;
                return true; //Itt nem történik más, minthogy végigjárja a tömböt és az első EMPTY helyre rak egy O-t. A visszatérés mögötti logika ugyan az.
            }
        }
    }
    return false;
}
bool Wincon_sor_oszlop(Karakterek **tomb,int meret,int *sor,int *oszlop,int kell,Karakterek kit_keres){
    int mennyis=0; //Ez a függvény csak a nehéz és közepes bot esetén hívódik meg, lényege, hogy elindul egy adott cellától függőlegesen is és vízszintesen is
    //Minden talált elemnél (Kit_keres) növeli vagy a mennyis, vagy a mennyio értékét, ha az közelíti a kirakáshoz szükséges kombinációt akkor az első üres helyre, amit ez után talál berakja a saját alakzatát (előbb ezt ellenőrzi, majd csak utána számol)
    int mennyio=0;
    for(int i=0;i<meret;i++){ //egyik irányba bejárás
        if(tomb[*sor][i]==EMPTY&&mennyis>=kell-2){
            tomb[*sor][i]=O;
            *oszlop=i;
            return true;
        }
        if(tomb[i][*oszlop]==EMPTY&&mennyio>=kell-2){
            tomb[i][*oszlop]=O;
            *sor=i;
            return true;
        }
        if(tomb[*sor][i]==kit_keres)
            mennyis++;
        else
            mennyis=0;
        if(tomb[i][*oszlop]==kit_keres)
            mennyio++;
        else
            mennyio=0;
    }
    mennyis=0;
    mennyio=0;
    //Ez azért kell, mert ha nincs akkor csak enyik irányba nézi végig az átlót és ha fordítva kezdi rakni a játékos akkor nem tudja.
       for(int i=meret-1;i>0;i--){ //egyik irányba bejárás
        if(tomb[*sor][i]==EMPTY&&mennyis>=kell-2){ //A logika az, mint az előzőnél, egy ciklussal végig megy minden lehetséges, valid indexen, miközben az egyiket lefixálja, így "keres" sorban és oszlopban
            tomb[*sor][i]=O;
            *oszlop=i;
            return true;
        }
        if(tomb[i][*oszlop]==EMPTY&&mennyio>=kell-2){
            tomb[i][*oszlop]=O;
            *sor=i;
            return true;
        }
        if(tomb[*sor][i]==kit_keres)
            mennyis++;
        else
            mennyis=0;
        if(tomb[i][*oszlop]==kit_keres)
            mennyio++;
        else
            mennyio=0;
    }
    return false;
}

bool Wincon_egyik_atlo_relativ(Karakterek **tomb,int meret,int *sor,int *oszlop,int jsor,int joszlop,int kell,Karakterek kit_keres){
int kellke=0;
int s=*sor;
int o=*oszlop;
while(s>0&&o>0){ //Elmegy az átló egyik végére, ez azért kell, hogy aztán attól a ponttól bejárja az egész átlót.
    s--;o--;
}
while(s<meret&&o<meret){

    if(kellke>=kell-2&&tomb[s][o]==EMPTY){ //Ez a feltétel nézi meg, hogy kijöhet e a győztes kombináció és ha igen, akkor egyből rak.
        tomb[s][o]=O;
        *sor=s;
        *oszlop=o;
        return true;
    }
    if(tomb[s][o]==kit_keres) //Számolja az egymástól átlóban, egymás mellett lévő elemeket
        kellke++;
    if(tomb[s][o]!=kit_keres)
        kellke=0;
    s++;o++;
}
kellke=0; //A jsor és a j oszlop a játékosnak a legutóbb lerakott helyét mutatja, célja, hogyha a gép esetleg nem tud nyerni, de a gép igen, vagy legalábbis tud közelíteni, akkor úgy rak.
s=jsor;
o=joszlop;
while(s>0&&o>0){
    s--;o--;
}
while(s<meret&&o<meret){
    if(kellke>=kell-2&&tomb[s][o]==EMPTY){
        tomb[s][o]=O;
        *sor=s;
        *oszlop=o;
        return true;
    }
    if(tomb[s][o]==kit_keres)
        kellke++;
    if(tomb[s][o]!=kit_keres)
        kellke=0;
    s++;o++;
}
return false;
}

bool Wincon_masik_atlo_relativ(Karakterek **tomb,int meret,int *sor,int *oszlop,int jsor,int joszlop,int kell,Karakterek kit_keres){//probléma, hogy egy sarkot nem tud nézni, ezért újra megírom, csak máshonnan indul
int kellke=0; //Ugyan az mint az előző, annyi különbséggel, hogy egy másik átlót jár be. Ugyanis az egyik átlót úgy kapjuk meg, ha az éppen aktuális elem mindkét indexét csökkentjük, ezzel egy \ átlót kapunk, ennél az egyik index nő, a másik csökken, az átló / ilyen.
int s=*sor;
int o=*oszlop;
while(s>0&&meret>o){
    s--;o++;
}
while(s<meret&&o>=0){
    if(kellke>=kell-2&&tomb[s][o]==EMPTY){
        tomb[s][o]=O;
        *sor=s;
        *oszlop=o;
        return true;
    }
     if(tomb[s][o]==kit_keres)
        kellke++;
    if(tomb[s][o]!=kit_keres)
        kellke=0;
        s++;o--;
}
kellke=0;
s=jsor;
o=joszlop;
while(s>0&&meret>o){
    s--;o++;
}
while(s<meret&&o>=0){
    if(kellke>=kell-2&&tomb[s][o]==EMPTY){
        tomb[s][o]=O;
        *sor=s;
        *oszlop=o;
        return true;
    }
    if(tomb[s][o]==kit_keres)
        kellke++;
    if(tomb[s][o]!=kit_keres)
        kellke=0;
        s++;o--;
}
return false;
}

bool Ez_bot(Karakterek **tomb,int meret,int *sor,int *oszlop){
    return(kb_viszonyitva_rak(tomb,meret,sor,oszlop)||rak(tomb,meret,sor,oszlop)); //Összegző függvény, a legkönnyebb bot mindössze 2-t hív meg, ezek közül a vagy miatt előbb megpróbál viszonyítva rakni, ha nem megy akkor meg csak az első valid helyre rak
}
bool Medium_bot(Karakterek **tomb,int meret, int *sor,int *oszlop,int kell){
    return(Wincon_sor_oszlop(tomb,meret,sor,oszlop,kell,X)||    //Az Ez_bothoz hasonlóan működik, annyi van, hogy a jsor és j oszlop mint olyan nincs. Bár ez nem túl optimális, ekkora tömbnél még nem zavaró. Először a sort és oszlopot nézi és mindenképpen a játékos győzelmét keresi és próbálja meggátolni
           Wincon_egyik_atlo_relativ(tomb,meret,sor,oszlop,*sor,*oszlop,kell,X)||
       Wincon_masik_atlo_relativ(tomb,meret,sor,oszlop,*sor,*oszlop,kell,X)||
       kb_viszonyitva_rak(tomb,meret,sor,oszlop)||rak(tomb,meret,sor,oszlop));

}
bool Hard_bot(Karakterek **tomb,int meret, int *sor,int *oszlop,int jsor,int joszlop,int kell){
     return(Wincon_sor_oszlop(tomb,meret,sor,oszlop,kell,O)|| //Mindössze annyi különbség van az előzőhöz képest, hogy előbb a saját alakzatát keresi és ha talál egymás mellett eleget (kell-2), akkor rak. Mindkét bot legvéső esetben csak az első üres helyre rak.
        Wincon_egyik_atlo_relativ(tomb,meret,sor,oszlop,jsor,joszlop,kell,O)||
        Wincon_masik_atlo_relativ(tomb,meret,sor,oszlop,jsor,joszlop,kell,O)||
        Wincon_sor_oszlop(tomb,meret,sor,oszlop,kell,X)||
        Wincon_egyik_atlo_relativ(tomb,meret,sor,oszlop,jsor,joszlop,kell,X)||
        Wincon_masik_atlo_relativ(tomb,meret,sor,oszlop,jsor,joszlop,kell,X)||
        kb_viszonyitva_rak(tomb,meret,sor,oszlop)||rak(tomb,meret,sor,oszlop));

}

