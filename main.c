#include <SDL.h>
#include <stdlib.h>
#include "ablak.h"
#include <SDL_mixer.h>
#include "karakterek.h"
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <SDL2_gfxPrimitives.h>
#include "debugmalloc.h"
#include <stdio.h>
#include <SDL_ttf.h> //A szöveg megjelenítéséhez kell
static Karakterek **Board; //A tábla alapját képző tömb
/* ablak letrehozasa */
void sdl_init(char const *felirat, int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer) {

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow(felirat, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szeles, magas, 0);
    if (window == NULL) {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(1);
    }
    SDL_RenderClear(renderer);
    *pwindow = window;
    *prenderer = renderer;
}
void MenuRajzol(SDL_Renderer *renderer,SDL_Surface *surface,SDL_Texture *texture,SDL_Rect *kirajzol,const char *Kov){
        char nev[40]="Assets\\"; //40-nél hosszabb sztring úgysem lesz elérési útvonal,ezért most elég. Mivel lokális változó így inkáb nem használtam ennél dinamikus memóriakezelést
        /*Nem volt kedvem mindenhol átirogatni az elérési útvonalat, helyette itt hozzáfűzöm a menü nevéhez, hogy
        hol is érhetőek el a kívánt képek*/
        strcat(nev,Kov);
        SDL_RenderClear(renderer);
        surface=IMG_Load(nev);
        texture=SDL_CreateTextureFromSurface(renderer,surface);
        SDL_FreeSurface(surface);
        SDL_RenderCopy(renderer,texture,NULL,kirajzol);
        SDL_RenderPresent(renderer);
        //Minden olyan dolog, ami az éppen aktuális menü kirajzolásához kell, cleareli a renderert, beolvas egy képet és azt kirajzolja

}
void Draw_Board(SDL_Renderer *render,const int meret,const float magas,const float szeles){ //Átveszi a méreteket, majd ezek alapján kirajzolja a táblát
    SDL_SetRenderDrawColor(render,0,0,0,255);
    SDL_RenderClear(render);

    SDL_SetRenderDrawColor(render,255,255,255,255);
    for(int i=1;i<meret;i++){ //1-től indul a ciklus, hiszen a 0*szeles az 0, oda meg nem kell vonalat rajzolni
        SDL_RenderDrawLine(render,i*szeles,0,i*szeles,Height); //fuggoleges vonalakat rajzol, majd csak a jatekban hivjuk meg
        SDL_RenderDrawLine(render,0,i*magas,Width,i*magas); //vízszintes vonalakat rajzol, együtt kiadják a táblát
    }
    SDL_RenderPresent(render);
}
void PointScreen(Points *point,bool destroy,Game *jatek){
    if(!destroy){
        SDL_Window *win=SDL_CreateWindow("Pontok",100,100,600,600,0);
        SDL_Renderer *render=SDL_CreateRenderer(win,-1,SDL_RENDERER_SOFTWARE);
        if(!win||!render){
            perror("Nem lehet megnyitni");
        }
        else{
        SDL_RenderClear(render);
        SDL_Surface *back=IMG_Load("Assets\\Pontablak.png");
        SDL_Rect rect;
        TTF_Font *font=TTF_OpenFont("Assets\\LiberationSerif-Regular.ttf",60); //Beöltöm a betűtípust, ezt követően kirajzolom a hátteret, majd Kiírom rá a neveket és a pontokat
        SDL_Texture *tex=SDL_CreateTextureFromSurface(render,back);
        SDL_FreeSurface(back);
        SDL_QueryTexture(tex,NULL,NULL,&rect.w,&rect.h);
        rect.x=(600-rect.w)/2;
        rect.y=(600-rect.h)/2;
        SDL_RenderCopy(render,tex,NULL,&rect);
        SDL_RenderPresent(render);
        point->scwindow=win;
        point->scrender=render;
        point->sctexture=tex;
        point->rect=rect;
        SDL_Color color={255,255,255,255}; //fehér szín
        char *nev; //A bot neve nehézségtől függően
        if(jatek->dif==0)
            nev="Csicska";
        else if(jatek->dif==1)
            nev="Kemény Karcsi";
        else
            nev="Brutál Botond";
        char *tomb=(char*) malloc(strlen(nev)+13); /*rra szolgál, hogy az egész sor szövegét le lehessen tárolni, majd később kiíratni,
        a +13-ban benne vannak a szóközök, a pontok és a játékos neve, + a kettőspontok*/
          if(jatek->jatekpont==100||jatek->geppont==100){ /*Úgy gondoltam, hogy járjon valami annak, aki eddig elmegy
              eddig, mert nem értem, hogy miért.*/
            if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048)<0){
                perror("Nem lehet audiot megnyitni");
            }
                MenuRajzol(render,back,tex,&rect,"BitesTheDust.png");
                jatek->jatekpont=0;
                jatek->geppont=0;
                Mix_Init(0);
                Mix_Music *bt=Mix_LoadMUS("Assets\\s_killaqueencatchphrase.mp3"); //Audio fájl betöltése
                Mix_PlayMusic(bt,0); //A zene lejátszódik, a loop 0-ra van beállítva, mindössze egyszer játszódna le, ha nem freeelném a szünet után is.
                SDL_Delay(4100); //Kis hatásszünet
                Mix_FreeMusic(bt);
                /*Kis easter egg, JónásJónás referencia, amennyiben valamelyik játékos eléri a 99 pontot
                Killer Queen 3. bombája érvénybe lép. Ekkor a háttér is más lesz, a pontok lenullázódnak és egy rövid
                hanganyag lejátszódik...also miért megy el valaki 99 pontig? */

        }
        sprintf(tomb,"P1: %d %s: %d",jatek->jatekpont,nev,jatek->geppont); //A tömbbe kíirom a dolgokat, amiket aztán megjelenítiek
        SDL_DestroyTexture(tex);
        back=TTF_RenderUTF8_Blended(font,tomb,color);
        tex=SDL_CreateTextureFromSurface(render,back);
        SDL_QueryTexture(tex,NULL,NULL,&rect.w,&rect.h);
        rect.x=(600-rect.w)/2;
        rect.y=(600-rect.h)/4; //A felső negyedbe helyezem kb a feliratot, vízszintesen közérpe igazítva
        SDL_FreeSurface(back);
        SDL_RenderCopy(render,tex,NULL,&rect);
        SDL_RenderPresent(render);
        TTF_CloseFont(font);
        free(tomb);
        }
    }
    else if(point->scwindow!=NULL){ //Amennyiben játék vége van az ablak bezáródik és a komponenseit fel kell szabadítani.
        SDL_DestroyTexture(point->sctexture);
        SDL_DestroyRenderer(point->scrender);
        SDL_DestroyWindow(point->scwindow);
    }
}
void MainMenu(SDL_Renderer *renderer,SDL_Surface *surface,SDL_Texture *texture,Menus *state,bool *quit,SDL_Rect *kirajzol,Game *jatek,Coordinates *st){//A főmenü kezelése, kilépés, játék töltése és új játék menübe lépés (almenü)
                    if(st->x<885&&st->x>370&&st->y>540&&st->y<675){ //betölti a nehézség választó képernyőt, ezek a koordináták a rajzon lévő new game boi ablak sarkainak közelítőleges koordinátái
                         MenuRajzol(renderer,surface,texture,kirajzol,"almenu.png");
                        *state=DIF;
                     }
                    else if(st->x<885&&st->x>370&&st->y>155&&st->y<290){ //Ezezk a load game rublika koordinátái
                        if(Beolv(jatek)){ //Meghívja a fajlkez.c-ben lévő beolvasás függvényt, majd a táblát megrajzolja ennek függvényében és a pontokat és nehézséget is beállítja
                            Board=lefoglal(jatek->meret);
                            Board_reset(jatek->meret,Board);
                            *state=INGAME;
                            st->cell_w=Width/jatek->meret;
                            st->cell_h=Height/jatek->meret;
                            Draw_Board(renderer,jatek->meret,st->cell_h,st->cell_w);

                        }
                    }
                    else if(st->x>370&&st->x<885&&st->y>760&&st->y<883){ //a quitet igazra állítva megszakítja a mainben használt event loopot és kilép a programból
                        *quit=true;
                    }
}
void DifficultyMenu(SDL_Renderer *renderer,SDL_Surface *surface,SDL_Texture *texture,Menus *state,SDL_Rect *kirajzol,Game *jatek,Coordinates *st){//Minden olyan hívás, ami az almenut/nehézség választó menut befolyásolja, visszalépés a főmenübe és belépés a méret válsaztó (DIF) menübe
    jatek->jatekpont=0; //Itt megtörténik a játékpontok lenullázása, elkerülve azt, hogy memóriaszemét kerüljön bele
    jatek->geppont=0;
    if(st->x>32&&st->x<240&&st->y>825&&st->y<950){
        MenuRajzol(renderer,surface,texture,kirajzol,"menu.png"); //A back rublika, visszalép a főmenübe
        *state=Main;
            }
    else if(st->x>408&&st->x<907&&st->y>120&&st->y<340){
            MenuRajzol(renderer,surface,texture,kirajzol,"DifEZ.png"); //A teljesen triviális rublika, könnyű mód, lépés a méret választó képernyőre
            *state=Meret;
            jatek->dif=0; //A nehézség 0, azaz easy
            jatek->kell=3; //3-at kell kirakni
    }

    else if(st->x>408&&st->x<907&&st->y>420&&st->y<630){
                MenuRajzol(renderer,surface,texture,kirajzol,"DifMed.png"); //Nehézség választó képernyőbe lép
                *state=Meret;
                jatek->dif=1;
                jatek->kell=5;
            }
    else if(st->x>408&&st->x<907&&st->y>670&&st->y<880){
                jatek->dif=2;
                jatek->kell=5;
                jatek->meret=6;
                *state=INGAME;
                Board=lefoglal(jatek->meret);
                Board_reset(jatek->meret,Board);
                st->cell_w=Width/jatek->meret;
                st->cell_h=Height/jatek->meret;
                Draw_Board(renderer,jatek->meret,st->cell_h,st->cell_w); //Nehéz fokozaton nincs választás, a pálya automatikusan 6x6-os lesz és a nem triviális rublikára kattintva egyből indul a játék
        }
}
void SizeMenu(SDL_Renderer *renderer,SDL_Surface *surface,SDL_Texture *texture,Menus *state,SDL_Rect *kirajzol,Game *jatek,Coordinates *st){
     if(st->x>130 && st->x<490&& st->y>290&&st->y<630){
                            if(jatek->dif==0) /*It ez az elágazás azt a célt szolgálja, hogy egy függvénybe kezelhessem a két
                            nehézségnek a méret kiválasztását, mivel a kordinátáik azonosak, pusztán a bot nehézségétől függ, hogy éppen 3 vagy 5 méretű
                                táblát generál*/
                                jatek->meret=3;
                            else
                                jatek->meret=5;
                            Board=lefoglal(jatek->meret);
                            Board_reset(jatek->meret,Board);
                           st->cell_w=Width/jatek->meret;
                           st->cell_h=Height/jatek->meret;
                            Draw_Board(renderer,jatek->meret,st->cell_h,st->cell_w);
                            *state=INGAME;

    }
     else if(st->x>810 && st->x<1175&& st->y>290&&st->y<630){
                            if(jatek->dif==0) //Ugyan az igaz, mint az előbb
                                jatek->meret=4;
                            else
                                jatek->meret=6;
                            Board=lefoglal(jatek->meret);
                            Board_reset(jatek->meret,Board);
                            st->cell_w=Width/jatek->meret;
                            st->cell_h=Height/jatek->meret;
                            Draw_Board(renderer,jatek->meret,st->cell_h,st->cell_w);
                            *state=INGAME;

    }
    else if(st->x>50&&st->x<280&&st->y>830&&st->y<940){
                        MenuRajzol(renderer,surface,texture,kirajzol,"almenu.png"); //Visszalépés az előtte lévő menübe
                        *state=DIF;
    }
}
bool PlayerTurn(SDL_Renderer *renderer,SDL_Surface *surface,SDL_Texture *texture,Menus *state,SDL_Rect *kirajzol,Game *jatek,Coordinates *st){//A játékos köre, rak, rajzol egy x-et majd a nehézségnek megfelelő botnak átadja a kört

    int o= (int) floor(st->y/st->cell_h); /*Így történik annak a kiszámítása, hogy a játékos kattintása a tömb melyik elemére correspondoljon
    A castolás azért fontos, mivel a floor double értéket ad vissza, az meg indexelésre nem túl jó. A kattintás kordinátáit leosztja egy
    cella méretével, majd azt lefele kerekítve megkapjuk, hogy melyik tömb elembe írjon a játék*/
    int s= (int) floor(st->x/st->cell_w);
    if(jatek->dif!=2){ /*Mivel a nehéz bot nem veszi annyira figyelembe a játékost, így ezt nem kell neki mindig megadni, mert csak elterelné a figyelmét (aka nem figyelne győzelmet, így
     a közepes és nehéz bot pont ugyan azt csinálná*/
    st->sor=s;
    st->oszlop=o;
    }
    if(Board[s][o]==EMPTY){
        thickLineRGBA(renderer,s*st->cell_w,o*st->cell_h+st->cell_h,s*st->cell_w+st->cell_w,o*st->cell_h,4,0,0,139,255);
        //Két, egymást metsző vonal megrajzolásával kapunk egy x-et.
        thickLineRGBA (renderer,s*st->cell_w+st->cell_w,o*st->cell_h+st->cell_h,s*st->cell_w,o*st->cell_h,4,0,0,139,255);
        SDL_RenderPresent(renderer);
        Board[s][o]=X;

    if(Board_State_Check(Board,jatek->meret,X,s,o,jatek->kell)){//Megnézi, nyert e valaki, ha false megnézi, hogy tele van e a board (vagyis döntetlen a helyzet)
        jatek->jatekpont+=1;
        MenuRajzol(renderer,surface,texture,kirajzol,"win screen.png");
        *state=ENDOFGAME;
        return true;

        }
         if(Tele(Board,jatek->meret)){ //Megnézi, tele van e a tábla
        *state=ENDOFGAME;
        MenuRajzol(renderer,surface,texture,kirajzol,"TieScreen.png");
        return true;
    }

    }
    else
        return true; //Erre azért van szükség, hogy amennyiben a játékos nem rakott, úgy ne adja át a kört a botnak, ugyanis a vagy kapcsolat miatt ha ez igazat ad vissza, akkor nem lép be a bot körébe
return false;
}
bool EzBotTurn(SDL_Renderer *renderer,SDL_Surface *surface,SDL_Texture *texture,Menus *state,SDL_Rect *kirajzol,Game *jatek,Coordinates *st){

        if(Ez_bot(Board,jatek->meret,&st->sor,&st->oszlop)){
        circleRGBA(renderer,st->sor*st->cell_w+st->cell_w/2,st->oszlop*st->cell_h+st->cell_h/2,st->cell_h/3,255,0,0,255);
        //A cella közepe a kör középpontja, a sugara pedig a cella 1/3-a. Minden bot így működik, csak más függvényt hívnak meg az elején
        SDL_RenderPresent(renderer);
        if(Board_State_Check(Board,jatek->meret,O,st->sor,st->oszlop,jatek->kell)){
            jatek->geppont+=1;
            *state=ENDOFGAME;
             MenuRajzol(renderer,surface,texture,kirajzol,"loose screen.png");
            return true;
        }
         if(Tele(Board,jatek->meret)){
            *state=ENDOFGAME;
             MenuRajzol(renderer,surface,texture,kirajzol,"TieScreen.png");
            return true;
        }
    }
return false;
}
bool MedBotTurn(SDL_Renderer *renderer,SDL_Surface *surface,SDL_Texture *texture,Menus *state,SDL_Rect *kirajzol,Game *jatek,Coordinates *st){

        if(Medium_bot(Board,jatek->meret,&st->sor,&st->oszlop,jatek->kell)){
             circleRGBA(renderer,st->sor*st->cell_w+st->cell_w/2,st->oszlop*st->cell_h+st->cell_h/2,st->cell_h/3,255,0,0,255);
            SDL_RenderPresent(renderer);

            if(Board_State_Check(Board,jatek->meret,O,st->sor,st->oszlop,jatek->kell)){
            jatek->geppont+=1;
            *state=ENDOFGAME;
             MenuRajzol(renderer,surface,texture,kirajzol,"loose screen.png");
                return true;
        }
         if(Tele(Board,jatek->meret)){
                *state=ENDOFGAME;
             MenuRajzol(renderer,surface,texture,kirajzol,"TieScreen.png");
                return true;
        }
    }
    return false;
}
bool HardBotTurn(SDL_Renderer *renderer,SDL_Surface *surface,SDL_Texture *texture,Menus *state,SDL_Rect *kirajzol,Game *jatek,Coordinates *st){
        int jsor=st->x/st->cell_w; //Egyetlen kivétel ez, ugyanis ez külön megnézi a saját legutóbbi helyzetét és a játékos helyzetét is.
        int joszlop=st->y/st->cell_h;
        if(Hard_bot(Board,jatek->meret,&st->sor,&st->oszlop,jsor,joszlop,jatek->kell)){
             circleRGBA(renderer,st->sor*st->cell_w+st->cell_w/2,st->oszlop*st->cell_h+st->cell_h/2,st->cell_h/3,255,0,0,255);
            SDL_RenderPresent(renderer);
            if(Board_State_Check(Board,jatek->meret,O,st->sor,st->oszlop,jatek->kell)){
                jatek->geppont+=1;
                *state=ENDOFGAME;
                MenuRajzol(renderer,surface,texture,kirajzol,"loose screen.png");
                return true;
        }
         if(Tele(Board,jatek->meret)){
            *state=ENDOFGAME;
             MenuRajzol(renderer,surface,texture,kirajzol,"TieScreen.png");
                return true;
        }
    }
    return false;

}

void Finish(SDL_Renderer *renderer,SDL_Surface *surface,SDL_Texture *texture,Menus *state,SDL_Rect *kirajzol,Game *jatek,Coordinates *st){
    if(st->x>45&&st->x<560&&st->y>660&&st->y<810){ //Ez a rematch gomb funkciója, a táblát "kitakarítja" és újra rajzolja.
        Board=lefoglal(jatek->meret);
        Board_reset(jatek->meret,Board);
        *state=INGAME;
        Draw_Board(renderer,jatek->meret,st->cell_h,st->cell_w);
    }
    else if(st->x>660&&st->x<1200&&st->y>660&&st->y<810){
        FreeBoard(Board,jatek->meret); //Quit opció, nullázza a pontokat és kilép a főmenübe, de előtte felszabadítja a táblát
        jatek->geppont=0;
        jatek->jatekpont=0;
        MenuRajzol(renderer,surface,texture,kirajzol,"menu.png");
        *state=Main;
    }
    else if(st->x>490&&st->x<780&&st->y>850&&st->y<930){
        Kiir(jatek); //Save gomb, fájlba kiírás, lásd "fajlkez.c"
    }
}
bool TurnHandle(SDL_Renderer *renderer,SDL_Surface *surface,SDL_Texture *texture,Menus *state,SDL_Rect *kirajzol,Game *jatek,bool *quit,Coordinates *st){
            if(jatek->dif==0){ /*Ez a függvény kezeli a köröket, ezért ad vissza minden turn function boolt, hogy lehessen
                nézni, hogy mikor kell átadni a kört. Ha a player turn truet ad vissza, azaz vagy nem rakott a játékos érvényes
                helyre, vagy nyert, akkor nem lép bele a bot körébe, ellenben igen*/
                return PlayerTurn(renderer,surface,texture,state,kirajzol,jatek,st)||
                EzBotTurn(renderer,surface,texture,state,kirajzol,jatek,st); //A megfelelő bot függvény meghívása
            }
            else if(jatek->dif==1){
                return PlayerTurn(renderer,surface,texture,state,kirajzol,jatek,st)||
                       MedBotTurn(renderer,surface,texture,state,kirajzol,jatek,st);
            }
            else{
                return PlayerTurn(renderer,surface,texture,state,kirajzol,jatek,st)||
                       HardBotTurn(renderer,surface,texture,state,kirajzol,jatek,st);
            }
}
void Menu_Kezeles(SDL_Renderer *renderer,SDL_Surface *surface,SDL_Texture *texture,Menus *state,SDL_Rect *kirajzol,Game *jatek,bool *quit,Coordinates *stuff){ /*
    Ebben a fügvényben van kezelve az összes többi, state függvényében meghívja a megfelelő függvényeket. Célja annyi, hogy egy picit átláthatóbb legyen a menük kezelése
    */
        switch(*state){
        case Main:
            MainMenu(renderer,surface,texture,state,quit,kirajzol,jatek,stuff);
            break;
        case DIF:
            DifficultyMenu(renderer,surface,texture,state,kirajzol,jatek,stuff);
            break;
        case Meret:
            SizeMenu(renderer,surface,texture,state,kirajzol,jatek,stuff);
            break;
        case INGAME:
            TurnHandle(renderer,surface,texture,state,kirajzol,jatek,quit,stuff);
            break;
        case ENDOFGAME:
            Finish(renderer,surface,texture,state,kirajzol,jatek,stuff);
            //Board=NULL;
        default:break;
        }
}

int main(int argc, char *argv[]) {
    TTF_Init();
    /* ablak letrehozasa */
    SDL_Window *window;
    SDL_Renderer *renderer;
    sdl_init("Amoba", Width, Height, &window, &renderer);
    Game jatek;
    SDL_Surface *surface=IMG_Load("Assets\\menu.png"); //Itt történik meg a főmenü első kirajzolása
    if(!surface){
        perror("Nem sikerult az ablakot letrehozni");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }
    SDL_Texture *texture=SDL_CreateTextureFromSurface(renderer,surface);
    SDL_FreeSurface(surface);
    SDL_Rect kirajzol;
    SDL_QueryTexture(texture,NULL,NULL,&kirajzol.w,&kirajzol.h); //Textúra méretének lekérdezése, annak eltárolása a rectangleben, inkább magamnak írom ezt
    kirajzol.x=(Width-kirajzol.w)/2;
    kirajzol.y=(Height-kirajzol.h)/2;
    SDL_RenderCopy(renderer,texture,NULL,&kirajzol);
    SDL_RenderPresent(renderer);
    Coordinates every;
    bool first=true;
    bool quit=false;
    SDL_Event ev;
    Menus state;
    state=Main;
    Points pontok;
    while(!quit){
        while(SDL_PollEvent(&ev)){ //Event kezelés itt történik
            switch(ev.type){
                case SDL_QUIT:
                    quit=true; //Amennyiben SDL_Quit event van a quit true lesz, ezzel megszakítva az event loopot, a loop után megtörténik a bezárás
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    SDL_GetMouseState(&every.x,&every.y); //Az egér kattintásának kordinátáit letárolom, ezután a függvényekben ezek helyzete alapján történnek dolgok, ezeket vizsgálom.
                    Menu_Kezeles(renderer,surface,texture,&state, &kirajzol,&jatek,&quit,&every);
                    if(jatek.dif==2&&state==INGAME&&first){ //A hard bot esetén a bot egyből rak, ezzel kikerülve, hogy minden lépéshez click szükséges
                        srand(time(0)); every.sor=rand()%6; every.oszlop=rand()%6; //Az első lépés random
                        HardBotTurn(renderer,surface,texture,&state,&kirajzol,&jatek,&every);
                        first=false; //A first arra szolgál, hogy amikor a bot kezd akkor éljen a véletlen helyzet, de ahogy már nem az első kör van rendesen kezelje a sorokat és ne csak évéletlen rakosgasson
                        PointScreen(&pontok,false,&jatek);
                    }
                    else if(first&&state==INGAME){ //A first másik funkciója, a pontozótáblát csak egyszer kell létrehozni, ez pedig akkor van, amikor a játék kezdődik
                        PointScreen(&pontok,false,&jatek);
                        first=false;
                    }
                    else if(state==ENDOFGAME){ //Ekkor csukjuk be a pontozó ablakot
                        PointScreen(&pontok,true,&jatek);
                        first=true;
                    }
                    break;
                case SDL_WINDOWEVENT:
                    if(ev.window.event==SDL_WINDOWEVENT_CLOSE&&ev.window.windowID==SDL_GetWindowID(pontok.scwindow)){
                        /*Arra szolgál, hogyha játék közben beakarjuk csukni az ablakokat
                        , akkor meglehessen tenni, ha ez nem lenne lekezelve játék közben nem lehetne kilépni*/
                           PointScreen(&pontok,true,&jatek);
                           quit=true;

                    }
                    break;
                default:
                    break;
            }
        }
    }
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();
    //A végén mindent becsukunk és felszabadítunk (a boardot azért nem, mert azt minden játék végével megteszem).
    return 0;
}
