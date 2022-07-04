#define _CRT_SECURE_NO_WARNINGS
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct _DIR {
	char nome[20];
	int n_fich;
	int n_Kb;
}DIR;

typedef struct _BTREE_NODE
{
	void* data;
	struct _BTREE_NODE* left;
	struct _BTREE_NODE* right;
} BTREE_NODE;

#define DATA(node) ((node)->data)
#define LEFT(node) ((node)->left)
#define RIGHT(node) ((node)->right)

typedef BTREE_NODE* BTREE;
typedef enum _BOOLEAN { FALSE = 0, TRUE = 1 } BOOLEAN;
typedef enum _STATUS { ERROR = 0, OK = 1 } STATUS;

BTREE_NODE* NewBtreeNode(void* data);
void BtreeFree(BTREE btree);
BTREE_NODE* InitNode(void* ptr_data, BTREE_NODE* node1, BTREE_NODE* node2);
BTREE_NODE* CreateBtree(void** v, int i, int size);
void InOrderPrint(BTREE btree);
STATUS ReadFile(void** dirs, char* file_name);
void MaisMilKb(BTREE btree);
int NumFich(BTREE, char*);
int NumDirVazias(BTREE btree);

int main()
{
	BTREE btree;
	void* dirs[7];
	char file_name[20], nome[20];
	printf("Nome do ficheiro: ");
	scanf("%s", file_name);
	if (ReadFile(dirs, file_name))
	{
		btree = CreateBtree(dirs, 0, 7);
		InOrderPrint(btree);
		printf("\n\nDiretorias com mais de 1000Kb: ");
		MaisMilKb(btree);
		printf("\nNome: ");
		scanf("%s",&nome);
		printf("\nN de ficheiros de %s = %d", nome, NumFich(btree,nome));
		printf("\nN de diretorias vazias = %d", NumDirVazias(btree));
		BtreeFree(btree);
	}
	else
		printf("ERRO na leitura do ficheiro\n");
	return 1;
}

void InOrderPrint(BTREE btree)
{
	if (btree != NULL)
	{
		InOrderPrint(btree->left);
		printf( "%s\n", ((DIR*)(btree->data))->nome);
		InOrderPrint(btree->right);
	}
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
	if (btree != NULL) {
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
BTREE_NODE* CreateBtree(void** v, int i, int size)
{
	if (i >= size)
		return(NULL);
	else
		return(InitNode(*(v + i), CreateBtree(v, 2 * i + 1, size), CreateBtree(v, 2 * i + 2, size)));
}

STATUS ReadFile(void** dirs, char* file_name)
{
	FILE* fp;
	int j, i = 0;
	void* ptr_data;
	if ((fp = fopen(file_name, "r")) != NULL)
	{
		while (!feof(fp))
		{
			if ((ptr_data = malloc(sizeof(DIR))) != NULL)
			{
				fscanf(fp, "%[^;];",((DIR*)ptr_data)->nome);
				fscanf(fp, "%d;", &(((DIR*)ptr_data)->n_fich));
				fscanf(fp, "%d\n", &(((DIR*)ptr_data)->n_Kb));
				//((DIR*)ptr_data)->nome[strcspn(((DIR*)ptr_data)->nome, "\n")] = 0;
				dirs[i] = ptr_data;
				i++;
			}
			else
			{
				for (j = i; j >= 0; j--)
					free(dirs[j]);
				return(ERROR);
			}
		}
		fclose(fp);
		return(OK);
	}
	else
		return(ERROR);
}

void MaisMilKb(BTREE btree)
{
	
	if (btree != NULL)
	{
		if (((DIR*)btree->data)->n_Kb > 1000)
			printf("\n%s", ((DIR*)btree->data)->nome);
		MaisMilKb(btree->left);
		MaisMilKb(btree->right);
	}
}

int NumFich(BTREE btree, char* nome)
{
	int count = 0;
	if (btree != NULL)
	{
		if (!(strcmp(nome, (((DIR*)btree->data)->nome))))
		{
			count += (((DIR*)btree->data)->n_fich);
			if (btree->left != NULL)
				count += NumFich(btree->left, ((DIR*)btree->left->data)->nome);
			if (btree->right != NULL)
				count += NumFich(btree->right, ((DIR*)btree->right->data)->nome);
		}
		else
			count+= NumFich(btree->left, nome) + NumFich(btree->right, nome);

	}
	return count;
}

int NumDirVazias(BTREE btree)
{
	int count = 0;
	if (btree != NULL)
	{
		if (((DIR*)btree->data)->n_fich == 0)
			count++;
		else count += NumDirVazias(btree->left) + NumDirVazias(btree->right);
	}
	return count;
}