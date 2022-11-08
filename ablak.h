#include <SDL.h>
#define Width (1280) //Főablak szélessége, illetve magassága
#define Height (960)
typedef enum{ //Mivel az, hogy egyes helyekre kattintva mit csináljon a játék függ attól, hogy éppen melyik menüben vagyunk így érdemes állapotokat kezelni és aszerint kezelni magát a játékot, erre szolgál ez az enum
    Main,DIF,Meret,INGAME, ENDOFGAME
}Menus;
typedef struct{ //A játékhoz szükséges elemek, tartalmazza a tábla méretét, a bot nehézségét, a két játszó fél pontját és azt, hogy mennyi karaktert kell kirakni (egybéként ez utóbbi nem szükséges)
    int meret;
    int dif;
    int jatekpont;
    int geppont;
    int kell;
}Game;
typedef struct{
int x; //Ezekben tárolom le az egér kattintásának pozícióját
int y;
float cell_w; //Ezekben tárolom el az adott táblán egy darab cella méretét. Ez a kirajzolásnál, valamint annál játszik szerepet, amikor is meg kell adni, hogy adott kattintás a Board tömb mely elemét változtassa meg
float cell_h;
int sor; //Mivel a bot lépéseinek szükséges feltétele, hogy tudja, hogy hol volt, ezt is le kell tárolnom, ehhez van a sor és oszlop változó.
int oszlop;
}Coordinates;
typedef struct{ //Ez a pontozó ablak létrehozásához, illetve majd megrajzolásához szükséges struktúra
SDL_Renderer *scrender;
SDL_Window *scwindow;
SDL_Texture *sctexture;
SDL_Rect rect;
}Points;

