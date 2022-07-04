#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DATA(node) ((node)->data)
#define NEXT(node) ((node)->next)

typedef enum _STATUS {ERROR,OK} STATUS;
typedef enum _BOOLEAN {FALSE,TRUE} BOOLEAN;

#define MAX_NOME 20

typedef struct _PLAYER
{
	char nome[MAX_NOME];
	char club[MAX_NOME];
	int posicao;
	BOOLEAN titular;
}PLAYER;

typedef struct _LIST_NODE
{
	void* data;
	struct _LIST_NODE* next;
} LIST_NODE;
typedef LIST_NODE* LIST;



LIST_NODE* NewNode(void*);
STATUS InsertIni(LIST* list, void* data);
STATUS ReadFile(LIST*,const char*);
void FreeList(LIST*);
void AddPlayer(LIST* );
void ShowPlayer(LIST);
void ShowTitular(LIST);
STATUS TrocarTit(LIST* list);

int main()
{
	LIST list=NULL;
	PLAYER player;
	
	if (ReadFile(&list,"selecao.txt"))
	{
		AddPlayer(&list);
		printf("\n");
		ShowPlayer(list);
		printf("\nTITULARES\n");
		ShowTitular(list);
		printf("\n");
		TrocarTit(&list);
		printf("\n");
		ShowPlayer(list);
	}
	else 
		FreeList(&list);
	return 1;
}

STATUS ReadFile(LIST* list, const char* name_file)
{
	FILE* fp;
	PLAYER player, * ptr;
	int aux;

	if ((fp = fopen(name_file, "r")) != NULL) {
		while (!feof(fp))
		{
			fscanf(fp, "%[^;]; %[^;];%d;%d\n", player.nome, player.club, &player.posicao, &aux);
			if (((ptr = (PLAYER*)malloc(sizeof(PLAYER))) != NULL) && (InsertIni(list, ptr) != NULL)) {
				strcpy(ptr->nome, player.nome);
				strcpy(ptr->club, player.club);
				ptr->posicao = player.posicao;
				if (aux) ptr->titular = TRUE;
				else ptr->titular = FALSE;
			}
			else {
				return ERROR;
			}

		}

		fclose(fp);
		return OK;
	}
	else {
		return ERROR;
	}

}

LIST_NODE* NewNode(void* data)
{
	LIST_NODE* new_node;
	if ((new_node = (LIST_NODE*)malloc(sizeof(LIST_NODE))) != NULL)
	{
		DATA(new_node) = data;
		NEXT(new_node) = NULL;
	}
	return(new_node);
}

STATUS InsertIni(LIST* list, void* data)
{
	LIST_NODE* new_node;
	if ((new_node = NewNode(data)) != NULL)
	{
		NEXT(new_node) = *list;
		*list = new_node;
		return OK;
	}
	return ERROR;
}

void FreeList(LIST* list)
{
	LIST_NODE* node;
	while (*list != NULL)
	{
		node = (*list)->next;
		free((*list)->data);
		free(*list);
		*list = node;
	}
}

void AddPlayer(LIST* list)
{
	PLAYER* pt=NULL;
	int titular = 0;

	if (((pt = (PLAYER*)malloc(sizeof(PLAYER))) != NULL) && (InsertIni(list,pt)==OK))
	{
		printf("Nome >> ");
		scanf("%s", pt->nome);
		printf("Club >> ");
		scanf("%s", pt->club);
		printf("Posicao >> ");
		scanf("%d", &(pt->posicao));
		printf("Titular >> ");
		scanf("%d", &titular);
		if (titular == 1)
			pt->titular = TRUE;
		else
			pt->titular = FALSE;
	}
}

void ShowPlayer(LIST list)
{
	while (list != NULL)
	{
		printf("%s;%s;%d;%d\n", ((PLAYER*)DATA(list))->nome, ((PLAYER*)DATA(list))->club, ((PLAYER*)DATA(list))->posicao, ((PLAYER*)DATA(list))->titular);
		list = NEXT(list);
	}
}

void ShowTitular(LIST list)
{
	int count = 0;
	while (list != NULL)
	{
		if (((PLAYER*)DATA(list))->titular == TRUE)
		{
			printf("%s;%s;%d\n", ((PLAYER*)DATA(list))->nome, ((PLAYER*)DATA(list))->club, ((PLAYER*)DATA(list))->posicao);
			count++;
		}
		list = NEXT(list);
	}
}

STATUS TrocarTit(LIST* list)
{
	LIST_NODE* temp = *list;
	char AntTit[MAX_NOME], NewTit[MAX_NOME];

	printf("Nome do Titular que deseja alterar >> ");
	scanf("%s", AntTit);
	printf("Nome do novo Titular >> ");
	scanf("%s", NewTit);

	while (temp != NULL)
	{
		if ((!strcmp(((PLAYER*)DATA(temp))->nome,AntTit)) && ((((PLAYER*)DATA(temp))->titular) == TRUE))
		{
			LIST_NODE* temp_new = *list;
			while (temp_new != NULL)
			{
				if ((!strcmp(((PLAYER*)DATA(temp_new))->nome, NewTit)) && ((((PLAYER*)DATA(temp_new))->titular) == FALSE))
				{
					((PLAYER*)DATA(temp))->titular = FALSE;
					((PLAYER*)DATA(temp_new))->titular = TRUE;
					return OK;
				}
				temp_new = NEXT(temp_new);
			}
		}
		temp = NEXT(temp);
	}
	printf("Troca efetuada\n");
	return ERROR;
}
