#define _CRT_SECURE_NO_WARNINGS
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct _PERSON
{
	char name[50];
	int	age;
	int alive;
}PERSON;

typedef struct _BTREE_NODE
{
	void* data;
	struct _BTREE_NODE* left;
	struct _BTREE_NODE* right;
}BTREE_NODE;

#define DATA(node)	((node)->data)
#define LEFT(node)	((node)->left)
#define RIGHT(node)	((node)->right)

typedef BTREE_NODE* BTREE;
typedef enum _BTREE_LOCATION { BTREE_LEFT, BTREE_RIGHT } BTREE_LOCATION;
typedef enum _BOOLEAN { FALSE = 0, TRUE = 1 } BOOLEAN;
typedef enum _STATUS { ERROR = 0, OK = 1 } STATUS;

BTREE_NODE* AddBtreeNode(BTREE_NODE* upnode, BTREE_NODE* node, BTREE_LOCATION where);
void InOrderPrint(BTREE btree);
BTREE_NODE* NewBtreeNode(void* data);
void BtreeFree(BTREE btree);
BTREE_NODE* InitNode(void* ptr_data, BTREE_NODE* node1, BTREE_NODE* node2);
STATUS ReadFile(void** persons, char* file_name);
BTREE_NODE* CreateBtree(void** v, int i, int size);
int CountVivos(BTREE btree);
void AlterarNome(BTREE, char*, char*);
BOOLEAN BtreeLeaf(BTREE_NODE* btree);
void AdicionarNo(BTREE btree, BTREE_NODE* temp);
BTREE_NODE* LerNo(BTREE_NODE*);

int main()
{
	BTREE btree;
	void* persons[7]; char file_name[20];
	char name[20], newName[20];
	printf("Nome do ficheiro: "); 
	scanf("%s", file_name); 
	if (ReadFile(persons, file_name))
	{
		btree = CreateBtree(persons, 0, 7);
		InOrderPrint(btree);
		printf("\nVivos = %d", CountVivos(btree));
		printf("\nInsere um nome: ");
		scanf("%s", &name);
		printf("\nInsere um novo nome: ");
		scanf("%s", &newName);
		AlterarNome(btree, name, newName);
		InOrderPrint(btree);
		AdicionarNo(btree, btree);
		InOrderPrint(btree);
		BtreeFree(btree);
	}
	else
		printf("ERRO na leitura do ficheiro\n"); 
	return 1;
}

BOOLEAN BtreeLeaf(BTREE_NODE* btree)
{
	if ((LEFT(btree) == NULL) && (RIGHT(btree) == NULL))
		return(TRUE);
	else
		return(FALSE);
}

void InOrderPrint(BTREE btree)
{
	if (btree != NULL)
	{
		InOrderPrint(btree->left);
		printf("%s\n", ((PERSON*)(btree->data))->name);
		InOrderPrint(btree->right);
	}
}

BTREE_NODE* NewBtreeNode(void* data)
{
	BTREE_NODE* tmp_pt;
	if ((tmp_pt = (BTREE_NODE*)malloc(sizeof(BTREE_NODE))) != NULL)
	{
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

STATUS ReadFile(void** persons, char* file_name)
{
	FILE* fp;
	int aux, j, i = 0;
	void* ptr_data;

	if ((fp = fopen(file_name, "r")) != NULL)
	{
		while (!feof(fp))
		{
			if ((ptr_data = malloc(sizeof(PERSON))) != NULL) {
				fscanf(fp, "%[^;];%d;%d\n", ((PERSON*)ptr_data)->name, &((PERSON*)ptr_data)->age, &aux);
				if (aux)
					((PERSON*)ptr_data)->alive = TRUE;
				else
					((PERSON*)ptr_data)->alive = FALSE;
				persons[i] = ptr_data;
				i++;
			}
			else {
				for (j = i; j >= 0; j--)
					free(persons[j]);
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

int CountVivos(BTREE btree)
{
	int count = 0;
	if (btree != NULL)
	{
		if (((PERSON*)btree->data)->alive)
			count++;
		count+=CountVivos(btree->left);
		count+=CountVivos(btree->right);
	}
	return count;
}

void AlterarNome(BTREE btree, char* name,char* newName)
{
	if (btree != NULL)
	{
		if (!strcmp((((PERSON*)btree->data)->name), name))
		{
			strcpy(((PERSON*)btree->data)->name,newName);
		}
		AlterarNome(btree->left, name, newName);
		AlterarNome(btree->right, name, newName);
	}
}

void AdicionarNo(BTREE btree, BTREE_NODE* temp)
{
	temp = btree;
	if (btree != NULL)
	{
		AdicionarNo(btree->right,temp);
		if (BtreeLeaf(btree))
		{
			temp = LerNo(btree->left);
			AddBtreeNode(btree, temp, BTREE_LEFT);
			temp = LerNo(btree->right);
			AddBtreeNode(btree, temp, BTREE_RIGHT);
	
		}
		
	}
}

BTREE_NODE* LerNo(BTREE_NODE* btreeNode)
{
	PERSON* newp = (PERSON*)malloc(sizeof(PERSON));
	printf("Insere nome: ");
	scanf("%s", newp->name);
	printf("Insere idade: ");
	scanf("%d", &newp->age);
	printf("Insere estado: ");
	scanf("%d", &newp->alive);
	return NewBtreeNode(newp);
}

BTREE_NODE* AddBtreeNode(BTREE_NODE* upnode, BTREE_NODE* node, BTREE_LOCATION where)
{
	BTREE_NODE* tmp_pt = upnode;
	if (where == BTREE_LEFT)
	{
		if (LEFT(upnode) == NULL)
			LEFT(upnode) = node;
		else
			tmp_pt = NULL;
	}
	else
	{
		if (RIGHT(upnode) == NULL)
			RIGHT(upnode) = node;
		else
			tmp_pt = NULL;
	}
	return tmp_pt;
}
