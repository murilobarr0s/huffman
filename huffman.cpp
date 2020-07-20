#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "myconio.h"

#define N 128
#define MAXBITS 50

// Estruturas
struct tpNo
{
	char letra;
	int frequencia;
	struct tpNo *esq, *dir;
};
typedef struct tpNo No;

struct tpTabela
{
	No *no;
	int codigo[10], TL;
};
typedef struct tpTabela Tabela;


// Funções
char isFolha(No *no)
{
	return no->esq == NULL && no->dir == NULL;
}

void bubble_sort(No *vet[], int TL)
{
	int TL2 = TL;
	No *aux;
	
	while(TL2 > 1)
	{
		for(int i = 0; i < TL2 - 1; i++)
		{
			if(vet[i]->frequencia < vet[i+1]->frequencia)
			{
				aux = vet[i];
				vet[i] = vet[i+1];
				vet[i+1] = aux;
			}
		}
		TL2--;
	}
}

void ordena_tabela(Tabela vet[], int TL)
{
	int TL2 = TL;
	Tabela aux;
	
	while(TL2 > 1)
	{
		for(int i = 0; i < TL2 - 1; i++)
		{
			if(vet[i].no->letra > vet[i+1].no->letra)
			{
				aux = vet[i];
				vet[i] = vet[i+1];
				vet[i+1] = aux;
			}
		}
		TL2--;
	}
}

void initFrequencia(No *vet[])
{
	for(int i = 0; i < N; i++)
	{
		vet[i] = (No*)malloc(sizeof(No));
		vet[i]->frequencia = 0;
		vet[i]->esq = vet[i]->dir = NULL;
	}
}

void conta_frequencias(char ent[], No *freq[])
{
	// percorre a entrada procurando ocorrencias
	int length = strlen(ent);
	for(int i = 0; i < length; i++)
	{
		freq[ent[i]]->letra = ent[i];
		freq[ent[i]]->frequencia++;
	}
}

void exibeASCII(No *frequencia[])
{
	int aux;
	for(int i = 0, aux = 7; i < N; i++)
	{
		if(i == 2 * aux)
		{
			aux = i;
			printf("\t\t");
		}
		printf("%c  %d\n", i, frequencia[i]->frequencia);
	}
}

void letras_entrada(No *frequencia[], No *letras[], int &TL)
{
	for(int i = 0; i < N; i++)
	{
		if(frequencia[i]->frequencia != 0)
		{
			letras[TL] = frequencia[i];
			TL++;
		}
	}
}

void exibe(No *raiz, int x, int &y, int par)
{
	int x1, y1, x2, c;
	
	if(raiz != NULL)
	{
		gotoxy(x, y); 
		if(raiz->esq == NULL && raiz->dir == NULL)
			printf("%c", raiz->letra);
		else
			printf("%d", raiz->frequencia);
		x1 = x - ((par - x + 1) / 2);
		y1 = y + 2;
		x2 = x + ((par - x + 1) / 2);
		
		gotoxy(x1, y1-1); 
		printf("%c", 218);
		gotoxy(x2, y1-1); 
		printf("%c", 191);
		for(c = x1 + 1; c <= x2 - 1; c++)
		{
			gotoxy(c, y1-1); 
			printf("%c", 196);
		}	
		gotoxy(x, y+1); 
		printf("%c", 193);
		
		exibe(raiz->esq, x1, y1, x-1);
		exibe(raiz->dir, x2, y1, par);	
	}	
}

void in_ordem(No *raiz, int vet[], int i, Tabela tab[], int &pos)
{
	if(raiz != NULL)
	{
		if(!isFolha(raiz))
		{
			tab[pos].codigo[i] = 0;
			in_ordem(raiz->esq, tab[pos].codigo, i + 1, tab, pos);
			
			// preenche vetor da posicao
			for(int k = 0; k < i; k++)
				tab[pos].codigo[k] = tab[pos-1].codigo[k];
			
			tab[pos].codigo[i] = 1;
			in_ordem(raiz->dir, tab[pos].codigo, i + 1, tab, pos);
		}
		else
		{
			char a = raiz->letra;
			tab[pos].no = raiz;
			tab[pos].TL = i;
			pos++;
		}
	}
}

void gera_tabela_codigo(No *raiz, Tabela tab[], int &pos)
{
	int vetCod[10], i = 0;
	
	in_ordem(raiz, vetCod, i, tab, pos);
	
	ordena_tabela(tab, pos);
}

void codifica_entrada(Tabela tab[], char entrada[], int saida[], int &tlSaida)
{
	int length = strlen(entrada), pos;
	bool achou = false;
	
	for(int i = 0; i < length; i++)
	{
		// procura na tabela a letra, e pega o codigo
		pos = 0;
		while(!achou)
		{
			if(tab[pos].no->letra == entrada[i])
			{
				// pega o codigo
				for(int j = 0; j < tab[pos].TL; j++, tlSaida++)
					saida[tlSaida] = tab[pos].codigo[j];
				achou = true;
			}
			pos++;
		}
		achou = false;
	}
}


void decodifica(No *letras[], No *raiz, int vet[], int &i, int TL)
{
	if(raiz != NULL)
	{
		if(isFolha(raiz))
		{
			printf("%c", raiz->letra);
			if(i < TL)
				decodifica(letras, letras[0], vet, i, TL);	
		}
		else
		{
			if(vet[i] == 1)
			{
				i++;
				decodifica(letras, raiz->dir, vet, i, TL);
			}
			else
			{
				i++;
				decodifica(letras, raiz->esq, vet, i, TL);
			}	
		}
	}
}

void constroi_arvore(No *letras[], int &tlLetras, Tabela tab[], int &tlTab, char entrada[])
{
	No *frequencia[N];
	
	initFrequencia(frequencia);
	initFrequencia(letras);
	
	// 1º conta frequencias
	conta_frequencias(entrada, frequencia);
	
	// antes de ordenar cria novo vetor com as ocorrencias
	letras_entrada(frequencia, letras, tlLetras);
	
	if(tlLetras == 1)
	{
		tab[0].no = letras[0];
		tab[0].codigo[0] = 0;
		tab[0].TL = 1;
		tlTab = 1;
	}
	else
	{
		// constroi arvore
		while(tlLetras > 1)
		{
			// Ordena o vetor
			bubble_sort(letras, tlLetras);
	
			// pega as 2 menores orcorrencias, tranforma em uma, devolve ao vetor e reduz o tl
			No *novo = (No*)malloc(sizeof(No));
			novo->frequencia = letras[tlLetras - 1]->frequencia +  letras[tlLetras - 2]->frequencia;
			
			if(letras[tlLetras - 1]->frequencia == letras[tlLetras - 2]->frequencia)
			{
				novo->esq = letras[tlLetras - 2];
				novo->dir = letras[tlLetras - 1];
			}
			else
			{
				novo->esq = letras[tlLetras - 1];
				novo->dir = letras[tlLetras - 2];
			}
			
			letras[tlLetras - 2] = novo;
			tlLetras--;
		}
			
		// gera tabela de codigos
		gera_tabela_codigo(letras[0], tab, tlTab);
	}
}

void exibe_dados(No *raiz, Tabela tab[], int tlTab, char entrada[])
{
	int length, tlLetras = 0, y = 5, y2, saida[2 * N], tlSaida = 0;
	
	clrscr();
	printf("\t\t\t Arvore de Huffman ");
	exibe(raiz, 35, y,  70);
	
	y += 10;
	gotoxy(5, y);
	printf("Entrada: %s", entrada);
	y += 2;
	gotoxy(5, y);
	printf("Entrada Codificada: ");
	codifica_entrada(tab, entrada, saida, tlSaida);
	int x = 25;
	for(int i = 0; i < tlSaida; i++, x++)
	{
		gotoxy(x, y);
		printf("%d", saida[i]);
	}
	
	y += 2;
	y2 = y;
	gotoxy(5, y);
	printf("Tabela de Ocorrencias: \n");
	
	// exibe ocorrencias
	for(int i = 0; i < tlTab; i++)
	{
		y++;
		gotoxy(15, y);
		printf("%c %d", tab[i].no->letra, tab[i].no->frequencia);
	}
	
	// exibe tabela de codigos
	gotoxy(50, y2);
	printf("Tabela de Codigos: \n");
	for(int k = 0; k < tlTab; k++)
	{
		y2++;
		gotoxy(55, y2);
		printf("%c ", tab[k].no->letra);
		for(int j = 0; j < tab[k].TL; j++)
		{
			gotoxy(60 + j, y2);
			printf("%d", tab[k].codigo[j]);
		}
	}
}

void exibe_tabela(Tabela tab[], int tlTab, int x, int &y)
{
	printf("Tabela de Codigos: \n");
	for(int k = 0; k < tlTab; k++)
	{
		y++;
		gotoxy(x, y);
		printf("X ");
		x += 2;
		for(int j = 0; j < tab[k].TL; j++)
		{
			gotoxy(x + j, y);
			printf("%d", tab[k].codigo[j]);
		}
		x -= 2;
	}
}

void executa(No *letras[], int &tlLetras, Tabela tab[], int &tlTab, int op)
{
	char entrada[N];
	int codigo[2*N];
	
	switch(op)
	{
		case 1:
			clrscr();
			printf("Entrada: ");
			fflush(stdin);
			gets(entrada);
			constroi_arvore(letras, tlLetras, tab, tlTab, entrada);
			exibe_dados(letras[0], tab, tlTab, entrada);
			getch();
			break;
		case 2:
			clrscr();
			int x = 5, y = 5, TL = -1, limit = 2 * N;
			
			// exibe tabela de codigos
			exibe_tabela(tab, tlTab, x, y); y += 5;
			gotoxy(x, y); printf("Codigo: "); x += 10;
			gotoxy(x, y);
			
			// le codigo
			do
			{
				TL++;
				gotoxy(x, y);
				fflush(stdin);
				scanf("%d", &codigo[TL]);
				x++;
			}while(codigo[TL] == 1 || codigo[TL] == 0);
			
			clrscr();
			printf("Codigo: ");
			for(int k = 0; k < TL; k++)
				printf("%d", codigo[k]);
			printf("\n");
			printf("Decodificado: ");
			
			// decodifica a entrada
			int i = 0;
			decodifica(letras, letras[0], codigo, i, TL);
			getch();
			break;
	}
}

void menu()
{
	int x, y, op, tlLetras, tlTab;
	No *letras[N];
	Tabela tab[N];
	
	do
	{
		clrscr();
		x = 30; y = 10;
		gotoxy(x, y); printf("[1] Construir Arvore"); y++;
		gotoxy(x, y); printf("[2] Decodificar"); y++;
		gotoxy(x, y); printf("[0] Sair"); y++;
		gotoxy(x, y);
		fflush(stdin);
		scanf("%d", &op);
		if(op == 1)
		{
			tlLetras = 0;
			tlTab = 0;
			executa(letras, tlLetras, tab, tlTab, op);
		}
		else if(op == 2)
			executa(letras, tlLetras, tab, tlTab, op);
			
	}while(op != 0);
}

int main()
{	
	menu();
	
	return 0;
}
