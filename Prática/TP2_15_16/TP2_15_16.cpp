//#include "stdafx.h"
#define _CRT_SECURE_NO_WARNINGS
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DATA(node) ((node)->pData)
#define LEFT(node) ((node)->pLeft)
#define RIGHT(node) ((node)->pRight) 
#define EMPTY NULL
#define NO_LINK NULL

typedef enum _STATUS {OK,ERROR} STATUS;
typedef enum _BOOLEAN { FALSE = 0, TRUE = 1 } BOOLEAN;
// Definição da estrutura PERSON 
typedef struct _PERSON {
	char name[30]; 
	int age;
	BOOLEAN deceased;
} PERSON;

// Definição da estrutura BT_NODE 
typedef struct _BT_NODE {
	void* pData;
	struct _BT_NODE* pLeft; 
	struct _BT_NODE* pRight;
} BT_NODE;

typedef BT_NODE* BT;
//Declaração das funções
BT_NODE* initNode(void*, BT_NODE*, BT_NODE*); 
BT_NODE* createNewBTNode(void*);
BT createBT(PERSON*, int, int); 
BOOLEAN emptyBTree(BT);
BOOLEAN isLeaf(BT_NODE*);
void printPersonBTree(BT);
void changeStaff(BT);
BT transformBTree(BT);
void PrintParents(BT, char*);
int maxAgeGreatGreatParents(BT, int);
int maxAge(BT, int);
int maxAgeSide(BT);
BOOLEAN Side(BT, int);
#define NMAX 15

int main(int argc, char* argv[])
{
	BT T = NULL;
	char nome[NMAX];
	int max = 0;
	int side = 0;
	PERSON v[] = { "Xavier",45,FALSE,
					"Jorge",82,FALSE,
					"Luisa",99,FALSE,
					"Raul",102,TRUE,
					"Joaquina",72,TRUE,
					"Marco",77,TRUE,
					"Teresa",91,FALSE,
					"Antero",70,TRUE,
					"Maria",86,TRUE,
					"Carlos",81,TRUE,	
					"Sandra",36,TRUE,
					"Duarte",72,TRUE,
					"Ilda",84,TRUE,
					"Alfredo",55,TRUE,
					"Francisca",77,TRUE };

	T = createBT(v, 0, NMAX);
	printPersonBTree(T);
	printf("\n\nNome: ");
	scanf("%s", nome);
	PrintParents(T, nome);
	max = maxAgeGreatGreatParents(T, ((PERSON*)T->pData)->age);
	printf("\n\nIdade max dos bisavos = %d", max);
	side = maxAgeSide(T);
	if (side == 0)
		printf("\n\nPessoa mais velha esta do lado materno");
	else 
		printf("\n\nPessoa mais velha esta do lado paterno");
		return EXIT_SUCCESS;
}

// Função que verifica se uma árvore binária está vazia 
BOOLEAN emptyBTree(BT T)
{
	return (T == NULL) ? TRUE : FALSE;
}

// Função que verifica se um nó é uma folha
BOOLEAN isLeaf(BT_NODE* pT)
{
	return ((LEFT(pT) == NULL) && (RIGHT(pT) == NULL)) ? TRUE : FALSE;
}

// Função que cria um nó
BT_NODE* createNewBTNode(void* pData)
{
	BT_NODE* pTemp;

	if ((pTemp = (BT_NODE*)malloc(sizeof(BT_NODE))) != NULL) 
	{
		DATA(pTemp) = pData; 
		LEFT(pTemp) = RIGHT(pTemp) = NULL;
	}

	return pTemp;
}


// Funções que criam a BST
BT createBT(PERSON v[], int i, int size)
{
	if (i >= size) 
		return(NULL);
	else 
		return(initNode(&v[i], createBT(v, 2 * i + 1, size), createBT(v, 2 * i + 2, size)));
}

BT_NODE* initNode(void* pData, BT_NODE* n1, BT_NODE* n2)
{
	BT_NODE* pTemp = NULL;

	pTemp = createNewBTNode(pData);
	LEFT(pTemp) = n1;
	RIGHT(pTemp) = n2;

	return(pTemp);
}

// Função que apresenta a árvore binária (de elementos de tipo PERSON)
void printPersonBTree(BT T)
{
	if (emptyBTree(T) == TRUE) return;

	printPersonBTree(LEFT(T));
	printf("\nnome : %s - idade : %d (%s)", ((PERSON*)DATA(T))->name,
		((PERSON*)DATA(T))->age, ((PERSON*)DATA(T))->deceased ? "falecido" : "vivo");
	printPersonBTree(RIGHT(T));

	return;
}


void PrintParents(BT B, char* nome)
{
	if (B != NULL)
	{
		if (!(strcmp(((PERSON*)B->pData)->name, nome)))
		{
			if (isLeaf(B))
				printf("Nao tem pais");
			else
				printf("%s - %s", ((PERSON*)B->pLeft->pData)->name, ((PERSON*)B->pRight->pData)->name);
		}
		PrintParents(B->pLeft, nome);
		PrintParents(B->pRight, nome);
	}
}

int maxAgeGreatGreatParents(BT T, int max)
{
	if (T != NULL)
	{
		if (isLeaf(T))
		{
			if (((PERSON*)T->pData)->age > max)
				max = ((PERSON*)T->pData)->age;
		}
		max = maxAgeGreatGreatParents(T->pLeft, max);
		max = maxAgeGreatGreatParents(T->pRight, max);
	}
	return max;
}

int maxAge(BT T, int max)
{
	if (T != NULL)
	{
		if (((PERSON*)T->pData)->age > max)
			max = ((PERSON*)T->pData)->age;
		max = maxAge(T->pLeft,max);
		max = maxAge(T->pRight,max);
	}
	return max;
}


int maxAgeSide(BT T)
{
	int max = maxAge(T, 0);
	if (T != NULL)
	{
		if (!Side(T->pLeft,max))
			return 1;
		if (!Side(T->pRight,max))
			return 0;
	}
	
}

BOOLEAN Side(BT T, int max)
{
	
	if (T != NULL)
	{
		if (((PERSON*)T->pData)->age == max)
			return FALSE;
		Side(T->pLeft,max);
		Side(T->pRight,max);
	}
}