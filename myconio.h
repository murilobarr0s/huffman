
// simula o textcolor 


//#include <stdafx.h
#include <conio.h>
#include <windows.h>
#include <stdio.h>
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHTGREY 7
#define DARKGREY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTCYAN 11
#define LIGHTRED 12
#define LIGHTMAGENTA 13
#define YELLOW 14
#define WHITE 15
#define BLINK 128

//simula o textcolor+textbackground
void TextColor(int fontcolor,int backgroundcolor)
{
   HANDLE screen;
  //int textcolor = LIGHTGREEN;
  //int backgroundcolor = BLACK;
  screen = GetStdHandle(STD_OUTPUT_HANDLE);
  int color_attribute;
  color_attribute = backgroundcolor;
  color_attribute = _rotl(color_attribute,4) | fontcolor;
  SetConsoleTextAttribute(screen,color_attribute);
}      
    
// simula o clrscr()
void clrscr()
{
   system("cls");
}
	  
// simula o gotoxy
void gotoxy(int xpos, int ypos)
{
  COORD scrn; 
  HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
  scrn.X = xpos; scrn.Y = ypos;
  SetConsoleCursorPosition(hOuput,scrn);
}

void pintaTela(int backgroundcolor)
{
	TextColor(BLACK,backgroundcolor);
	for (int i = 0; i < 80; i++)
	{
		for (int j = 0; j < 25; j++)
		{
			gotoxy(i,j);
			printf(" ");
		}
	}
	gotoxy(0,0);
}

void montaTela(char endereco[])
{
	int caracter,tam = strlen(endereco);
	pintaTela(LIGHTGREY);
	for (int i = 0; i < 80; i++)
	{
		for (int j = 0; j < 23; j++)
		{
			gotoxy(i,j);
			if(i == 0 || i == 79)
				caracter = 179;
			else if(j == 0 || j == 22)
					caracter = 196;
			else caracter = 0;
			printf("%c",caracter);

		}
	}
	gotoxy(0,2); printf("%c",195);gotoxy(79,2); printf("%c",180);
	for (int i = 1; i <79 ; i++)
	{
		gotoxy(i,2);
		printf("%c",196);
	}
	gotoxy(0,0); printf("%c",218);
	gotoxy(79,0); printf("%c",191);
	gotoxy(0,22); printf("%c",192);
	gotoxy(79,22); printf("%c",217);
	gotoxy(1,0); printf("%c",180);
	gotoxy(3,0); printf("%s",endereco);
	gotoxy(4+tam,0); printf("%c",195);
	gotoxy(2,0); printf(" ");
	gotoxy(3+tam,0); printf(" ");
	gotoxy(4,23); printf("F3 - Abrir           F4 - Executar ");
	gotoxy(1,1); printf("Interpretador  de Programas C");
}

void usaBarraN(int linha)
{
	linha++;
	gotoxy(2, linha);
}
