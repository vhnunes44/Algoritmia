//#include "stdafx.h"
#define _CRT_SECURE_NO_WARNINGS
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_NAME 20
#define STAGES 15

typedef struct _DEPT {
	char	desig[20];
	float	ganhos, despesas; 
	int	n_pessoas;
}DEPT;

typedef struct _BTREE_NODE {
	void* data;
	struct _BTREE_NODE* left;
	struct _BTREE_NODE* right;
} BTREE_NODE;

#define DATA(node)	((node)->data) 
#define LEFT(node)	((node)->left) 
#define RIGHT(node)	((node)->right) 
typedef BTREE_NODE * BTREE;
typedef enum _BOOLEAN { FALSE = 0, TRUE = 1 } BOOLEAN;
typedef enum _STATUS { ERROR = 0, OK = 1 } STATUS;

BTREE_NODE* NewBtreeNode(void* data);
void BtreeFree(BTREE btree);
BTREE_NODE* InitNode(void* ptr_data, BTREE_NODE* node1, BTREE_NODE* node2);
void InOrderPrint(BTREE btree);
STATUS ReadFile(void** depts, char* file_name);
BTREE_NODE* CreateBtree(void** v, int i, int size);
BOOLEAN BtreeLeaf(BTREE_NODE* btree);
float Lucros(BTREE btree);
BTREE_NODE* MaiorDept(BTREE btree, BTREE_NODE* temp);
void Print(BTREE);
void EliminDept(BTREE btree, const char* nome);
int CountFunc(BTREE_NODE* btree);

int main()
{
	BTREE btree;
	BTREE_NODE* temp;
	DEPT* dept=NULL;
	void* depts[7]; char file_name[20];
	printf("Nome do ficheiro: "); 
	scanf("%s", file_name); 
	if (ReadFile(depts, file_name)) {
		btree = CreateBtree(depts, 0, 7);
		InOrderPrint(btree);
		printf("\nLucros da empresa = %.1f", Lucros(btree));
		temp=MaiorDept(btree,btree);
		Print(temp);
		EliminDept(btree,"lojistica");
		InOrderPrint(btree);
		BtreeFree(btree);
	}
	else 
		printf("ERRO na leitura do ficheiro\n"); 
	return 1;
}

BTREE_NODE* NewBtreeNode(void* data)
{
	BTREE_NODE* tmp_pt;
	if ((tmp_pt = (BTREE_NODE*)malloc(sizeof(BTREE_NODE))) != NULL) {
		DATA(tmp_pt) = data;
		LEFT(tmp_pt) = RIGHT(tmp_pt) = NULL;
	} 
	return tmp_pt;
}

void BtreeFree(BTREE btree)
{
	if (btree != NULL) 
	{
		BtreeFree(LEFT(btree)); 
		BtreeFree(RIGHT(btree)); 
		free(DATA(btree)); 
		free(btree); 
	}
}

BTREE_NODE* InitNode(void* ptr_data, BTREE_NODE* node1, BTREE_NODE* node2)
{
	BTREE_NODE* tmp_pt = NULL; 
	tmp_pt = NewBtreeNode(ptr_data); 
	LEFT(tmp_pt) = node1; 
	RIGHT(tmp_pt) = node2; 
	return(tmp_pt);
}

void InOrderPrint(BTREE btree)
{
	if (btree != NULL)
	{
		InOrderPrint(btree->left);
		printf("\n%s", ((DEPT*)(btree->data))->desig);
		InOrderPrint(btree->right);
	}
}

STATUS ReadFile(void** depts, char* file_name)
{
	FILE* fp; int j, i = 0;
	void* ptr_data;
	if ((fp = fopen(file_name, "r")) != NULL) {
		while (!feof(fp)) {
			if ((ptr_data = malloc(sizeof(DEPT))) != NULL) {
				fscanf(fp,"%[^;];%d;%f;%f\n", ((DEPT*)ptr_data)->desig, &((DEPT*)ptr_data)->n_pessoas, &((DEPT*)ptr_data)->ganhos, &((DEPT*)ptr_data)->despesas);
				depts[i] = ptr_data; 
				i++;
			}
			else {
				for (j = i; j >= 0; j--)
					free(depts[j]); 
				return(ERROR);
			}
		}
		fclose(fp);
		return(OK);
	}
	else
		return(ERROR);
}

BTREE_NODE* CreateBtree(void** v, int i, int size)
{
	if (i >= size)	
		return(NULL);
	else
		return(InitNode(*(v + i), CreateBtree(v, 2 * i + 1, size), CreateBtree(v, 2 * i + 2, size)));	
}

BOOLEAN BtreeLeaf(BTREE_NODE* btree)
{
	if ((LEFT(btree) == NULL) && (RIGHT(btree) == NULL))
		return(TRUE);
	else
		return(FALSE);
}

float Lucros(BTREE btree)
{
	float lucros = 0;
	if (btree != NULL) 
	{
		lucros += ((DEPT*)btree->data)->ganhos - ((DEPT*)btree->data)->despesas + Lucros(btree->left) + Lucros(btree->right);
	}
	return lucros;
}

BTREE_NODE* MaiorDept(BTREE btree, BTREE_NODE* temp)
{
	if (btree != NULL)
	{
		if ((((DEPT*)btree->data)->n_pessoas) > (((DEPT*)temp->data)->n_pessoas))
		{
			temp = btree;
			
		} 
		temp=MaiorDept(btree->left,temp);
		temp=MaiorDept(btree->right,temp);		
	}
	return temp;
}
void Print(BTREE btree)
{
	printf("\n\n%s\n%d\n%.1f\n%.1f", ((DEPT*)btree->data)->desig, ((DEPT*)btree->data)->n_pessoas, ((DEPT*)btree->data)->ganhos, ((DEPT*)btree->data)->despesas);
}

void EliminDept(BTREE btree, const char* nome)
{

	if (btree != NULL)
	{
		if (!strcmp(((DEPT*)btree->data)->desig, nome))
		{
			((DEPT*)btree->data)->n_pessoas = CountFunc(btree);
			Print(btree);
			BtreeFree(btree->left);
			btree->left = NULL;
			BtreeFree(btree->right);
			btree->right = NULL;
		}
		else
		{
			EliminDept(btree->left, nome);
			EliminDept(btree->right, nome);
		}
	}
}

int CountFunc(BTREE_NODE* btree)
{
	int count = 0;
	if (btree != NULL)
	{
		count+= (((DEPT*)btree->data)->n_pessoas) + CountFunc(btree->left) + CountFunc(btree->right);
		
	}
	return count;
}