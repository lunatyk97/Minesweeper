#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define bts(x) ((x)?"true":"false") // pozwala na zwracanie wartosci bool do str

typedef struct okienko{
    int wartosc;
    bool ukryte;
    int x;
    int y;
    SDL_Rect rect;
}okienko;

okienko** create_table(int N, int M)
{
    okienko **t;
    t = malloc( N * sizeof( okienko * ) );
    if ( t == NULL )
    {
        printf("Blad przydzialu pamieci!\n");
    }
    /*  Teraz przydzielamy pamiêæ dla ka¿dego wiersza */
    for (int i = 0; i < N; i++ )
    {
        t[i] = malloc( M * sizeof( okienko ) );
        if ( t[i] == NULL )
        {
            printf("Blad przydzialu pamieci #2!\n");
        }
    }
    return t;
}

void fill_table(okienko **tab, int h, int n)
{
	int i, j;
	for(i=0; i < h; i++)
	{
		for(j=0; j < n; j++)
		{
			tab[i][j].wartosc = 31;
            tab[i][j].ukryte = true;
            tab[i][j].x = j;
            tab[i][j].y = i;
            tab[i][j].rect.x = j * 21 + 10;
            tab[i][j].rect.y = i * 21 + 50;
            tab[i][j].rect.h = 20;
            tab[i][j].rect.w = 20;
		}
	}
}

void fill_bombs(okienko **tab, int h, int k, int *bomby)
{
    int bomba = 9;
    int counter = 0;
    int min = 0;
    int x;
    int y;
    while(counter < *bomby){
        //bierzemy losowo x
         x = min+ (rand() % ((k - 1) + 1 - min));
         y = min+ (rand() % ((h - 1)+ 1 - min));
        if(tab[y][x].wartosc == 31){
            tab[y][x].wartosc = bomba;
            //printf("x: %d, y: %d\n",x,y);
            counter++;
        }
    }
}

void fill_numbers(okienko **tab, int h, int n)
{
    //printf("liczby\n");
	int i, j;
	int bomba = 9;
	for(i=0; i < h; i++)
	{
		for(j=0; j < n; j++)
		{
            if(tab[i][j].wartosc != bomba){
                //printf("in ");
                int licznik = 0;
                if (i > 0 && j > 0)
                    {if(tab[i - 1][j - 1].wartosc == bomba) licznik++;}
                //printf("a");
                if (i > 0)
                    {if(tab[i - 1][j].wartosc == bomba) licznik++;}
                //printf("b");
                if (i > 0 && j < n - 1)
                    {if(tab[i - 1][j + 1].wartosc == bomba) licznik++;}
                //printf("c");
                if (j > 0)
                    {if(tab[i][j - 1].wartosc == bomba) licznik++;}
                //printf("d");
                if (j < n - 1)
                    {if(tab[i][j + 1].wartosc == bomba) licznik++;}
                //printf("e");
                if (i < h - 1 && j > 0)
                    {if(tab[i + 1][j - 1].wartosc == bomba) licznik++;}
                //printf("f");
                if (i < h - 1)
                    {if(tab[i + 1][j].wartosc == bomba) licznik++;}
                //printf("g");
                if (i < h - 1 && j < n - 1)
                    {if(tab[i + 1][j + 1].wartosc == bomba) licznik++;}
                //printf("2\n");
                if(licznik != 0) tab[i][j].wartosc = licznik;
            }
		}
	}
}

void fill(okienko **table, int wysokosc, int szerokosc, int *liczba_bomb)
{
    fill_table(table, wysokosc, szerokosc);

    fill_bombs(table, wysokosc, szerokosc, liczba_bomb);

    fill_numbers(table, wysokosc, szerokosc);
}


void destroy_table(okienko **tab, int h, int n)
{
	int i;
	for(i=0; i < n; i++)
	{
		free(tab[i]);
	}
	free(tab);
}

void odkryj_pozostale(okienko **tab, int h, int k, int y, int x, int *nieodkryte){
    /* funkcja majaca za zadanie odkrywac puste pola gdy jedno z nich zostanie wybrane*/

    if (tab[y][x].ukryte == true){
        tab[y][x].ukryte = false;
        if(tab[y][x].wartosc == 31){
            if(x + 1 < k)
                odkryj_pozostale(tab,h,k, y, x + 1,nieodkryte);
            if(x - 1 >= 0)
                odkryj_pozostale(tab,h,k, y, x - 1,nieodkryte);
            if(y + 1 < h)
                odkryj_pozostale(tab,h,k, y + 1, x,nieodkryte);
            if(y - 1 >= 0)
                odkryj_pozostale(tab,h,k, y - 1, x,nieodkryte);
        }
        (*nieodkryte)--;
    }
}

int check_value(okienko **tab, int h, int k, int y, int x, int *end, int *nieodkryte){
    int wroc = 0;

    if(tab[y][x].wartosc == 9){
        *end = 1;
        tab[y][x].wartosc = 33;
    }
    if(tab[y][x].wartosc == 31){
        //printf("funkcja odkrywajaca puste pola\n");
        odkryj_pozostale(tab,h,k,y,x,nieodkryte);
        wroc = 1;
    }
    else
        (*nieodkryte)--;
    tab[y][x].ukryte = false;

    return wroc;
}

void check(okienko **tab, int h, int k, int mx,int my, int *koniec, int *ile_nieodkrytych_pol){
    //h -wysokosc - liczba wierszy w macierzy
    //k - liczba kolumn
    int x = mx;
    int y = my;

    int a = check_value(tab,h,k, y, x, koniec, ile_nieodkrytych_pol);

}


int main(int argc, char** argv){

    //PARAMETRYGRY
    int szerokosc = 20;
	int wysokosc = 10;
	int ile_nieodkrytych_z = szerokosc * wysokosc;
	int liczba_bomb_z = 20;
	okienko **table;

    int zamknij_z = 0;
    bool show_bomb_z = false;
    bool wygrana_z = false;
    bool zakoncz_gre;

    int *z_szerokosc = &szerokosc;
    int *z_wysokosc = &wysokosc;
    int *zamknij = &zamknij_z; // jesli 0 to leci, 1 zamyka
    int *ile_nieodkrytych = &ile_nieodkrytych_z;
    int *liczba_bomb = &liczba_bomb_z;

    bool *show_bomb = &show_bomb_z;
    bool *wygrana = &wygrana_z;

    srand(time(NULL));
	table = create_table(wysokosc, szerokosc);
	fill(table, wysokosc, szerokosc, liczba_bomb);

    //PARAMETRY OKNA
    int wysokosc_okna = 400;
    int szerokosc_okna = 440;


    //SDL INIT
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_WM_SetCaption( " blue SAPER ", NULL );

    //TTF INIT
    TTF_Init();

    //EKRAN
    SDL_Surface *screen;
    screen = SDL_SetVideoMode(szerokosc_okna,wysokosc_okna,32,SDL_SWSURFACE);
    bool running = true;
    const int FPS = 30;
    Uint32 start;

    // Na te zmienne beda wskazniki polozenia myszy
    int mx, my;

    //inicjacja prostokatow
    SDL_Rect ramka;
    ramka.x = 0;
    ramka.y = 0;
    ramka.w = szerokosc_okna;
    ramka.h = wysokosc_okna;
    Uint32 ramka_kol = SDL_MapRGB(screen->format,60,150,210);

    SDL_Rect ramka_bomby;
    ramka_bomby.x = 300;
    ramka_bomby.y = 10;
    ramka_bomby.w = 100;
    ramka_bomby.h = 20;

    SDL_Rect ramka_punkty;
    ramka_punkty.x = 10;
    ramka_punkty.y = 10;
    ramka_punkty.w = 230;
    ramka_punkty.h = 20;
    Uint32 bialy = SDL_MapRGB(screen->format,0xff,0xff,0xff);

    SDL_Rect rect;
    rect.x = 10;
    rect.y = 10;
    rect.w = 20;
    rect.h = 20;

    SDL_Rect ramka_wygrana;
    ramka_wygrana.x = 175;
    ramka_wygrana.y = 300;
    ramka_wygrana.w = 100;
    ramka_wygrana.h = 100;

    SDL_Rect ramka_przegrana;
    ramka_przegrana.x = 100;
    ramka_przegrana.y = 300;
    ramka_przegrana.w = 100;
    ramka_przegrana.h = 100;

    Uint32 color = SDL_MapRGB(screen->format,200,200,100);
    Uint32 color2 = SDL_MapRGB(screen->format,50,150,200);
    Uint32 color2_2 = SDL_MapRGB(screen->format,80,190,220);

    while(running){
        start = SDL_GetTicks();
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
            case SDL_QUIT:
                running = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if(event.button.button == SDL_BUTTON_LEFT){
                    //color2 = SDL_MapRGB(screen->format,50,150,200);
                    SDL_GetMouseState(&mx, &my);
                    }
                break;
            }
        }
        //USTAWIENIA CZCIONKI
        TTF_Font *times;
        times = TTF_OpenFont("Dashboard.ttf",15);
        SDL_Color white = {0,0,0};
        SDL_Color red ={255,0,0};
        SDL_Color blue ={0,0,255};

        //ELEMENTY RYSOWANE:
        SDL_FillRect(screen,&screen->clip_rect,SDL_MapRGB(screen->format,0xff,0xff,0xff));
        //SDL_FillRect(screen,&ramka,ramka_kol);
        SDL_FillRect(screen,&ramka_punkty,bialy);
        SDL_FillRect(screen,&ramka_bomby,bialy);

        for(int i = 0; i < wysokosc; i++){
            for(int j = 0; j < szerokosc; j++){
                rect.x = j * 21 + 10;
                rect.y = i * 21 + 50;
                rect.h = 20;
                rect.w = 20;
                SDL_FillRect(screen,&rect,color2_2);
            }
        }

        for(int i = 0; i < wysokosc; i++){
            for(int j = 0; j < szerokosc; j++){



                if(table[i][j].ukryte == false){
                    SDL_FillRect(screen,&table[i][j].rect,color2_2);
                    if(table[i][j].wartosc == 31){
                        SDL_Surface *ko = TTF_RenderText_Solid(times,"   ",white);
                        SDL_BlitSurface(ko,NULL,screen,&table[i][j].rect);

                    }else if(table[i][j].wartosc == 33){
                        SDL_Surface *ko = TTF_RenderText_Solid(times," X ",red);
                        SDL_BlitSurface(ko,NULL,screen,&table[i][j].rect);
                    }else if(table[i][j].wartosc == 9){
                        SDL_Surface *ko = TTF_RenderText_Solid(times," O ",red);
                        SDL_BlitSurface(ko,NULL,screen,&table[i][j].rect);
                    }else{
                        char wt[10];
                        itoa(table[i][j].wartosc,wt,10);
                        SDL_Surface *ko = TTF_RenderText_Solid(times,wt,white);
                        SDL_BlitSurface(ko,NULL,screen,&table[i][j].rect);
                    }
                }
                else
                    SDL_FillRect(screen,&table[i][j].rect,color2);
                //KLIKANIE ELEMENTOW
                if (mx >= table[i][j].rect.x && mx < (table[i][j].rect.x + table[i][j].rect.w)){
                    if (my >= table[i][j].rect.y && my < (table[i][j].rect.y + table[i][j].rect.h))
                    {
                        if(*wygrana == true)
                            running = false;
                        if(table[i][j].ukryte == true && *zamknij == false)
                            check(table, wysokosc, szerokosc, j, i, zamknij, ile_nieodkrytych);
                    }
                }
            }
        }

        char tekst[]={"bomby: "};
        char tekst2[10];
        itoa(*liczba_bomb,tekst2,10);
        strcat(&tekst,&tekst2);

        char tekst3[]={"Pozostalo do odkrycia: "};
        char tekst4[10];
        itoa(*ile_nieodkrytych - *liczba_bomb,tekst4,10);
        strcat(&tekst3,&tekst4);

        SDL_Surface *message = TTF_RenderText_Solid(times, tekst3, white);
        SDL_BlitSurface(message,NULL,screen,&ramka_punkty);

        SDL_Surface *message2 = TTF_RenderText_Solid(times, tekst, white);
        SDL_BlitSurface(message2,NULL,screen,&ramka_bomby);

        //KONCZENIE GRY
        if(*ile_nieodkrytych == *liczba_bomb){
            //SDL_FillRect(screen,&screen->clip_rect,SDL_MapRGB(screen->format,0xff,0xff,0xff));
            SDL_Surface *przegrales = TTF_RenderText_Solid(times, "WYGRALES!", blue);
            //SDL_BlitSurface(przegrales,NULL,screen,&ramka_wygrana);
            SDL_BlitSurface(przegrales,NULL,screen,&ramka_wygrana);
            *wygrana = true;

        }
        if(*zamknij == true){
            SDL_Surface *przegrales = TTF_RenderText_Solid(times, "PRZEGRALES, KONIEC GRY!", red);
            SDL_BlitSurface(przegrales,NULL,screen,&ramka_przegrana);
        }

        //RYSOWANIE
        SDL_Flip(screen);
        if(1000/FPS > SDL_GetTicks() - start)
            SDL_Delay(1000/FPS - (SDL_GetTicks() - start));

        SDL_FreeSurface(message);
    }
    //ZAKONCZENIE
    destroy_table(table,wysokosc,szerokosc);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
