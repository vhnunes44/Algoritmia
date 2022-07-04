#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DATA(node) ((node)->data)
#define NEXT(node) ((node)->next)

typedef enum _STATUS { ERROR, OK } STATUS;
typedef enum _BOOLEAN { FALSE, TRUE } BOOLEAN;

#define MAX_NOME 20

typedef struct _FUNC
{
	char nome[MAX_NOME], dept[MAX_NOME];
	float ordenado;
	BOOLEAN do_quadro;
}FUNC;

typedef struct _LIST_NODE
{
	void* data;
	struct _LIST_NODE* next;
} LIST_NODE;
typedef LIST_NODE* LIST;

LIST_NODE* NewNode(void* data);
STATUS InsertIni(LIST* list, void* data);
STATUS ReadFile(LIST*, const char*);
void FreeList(LIST* list);
void RemoveFirstNode(LIST* list);
void OrdenadoTotal(LIST list);
void ColocarQuadro(LIST* list);
void ShowFunc(LIST list);
void EliminarFuncFora(LIST* list);

int main()
{
	LIST list = NULL;
	FUNC func;

	if (ReadFile(&list, "funcionarios.txt"))
	{
		ShowFunc(list);
		printf("\n");
		OrdenadoTotal(list);
		printf("\n");
		ColocarQuadro(&list);
		printf("\n");
		ShowFunc(list);
		printf("\n");
		EliminarFuncFora(&list);
		printf("\n");
		ShowFunc(list);

	}
	else
		FreeList(&list);
	return 1;
}

STATUS ReadFile(LIST* list, const char* name_file)
{
	FILE* fp;
	FUNC func, * ptr;
	int aux;

	if ((fp = fopen(name_file, "r")) != NULL) {
		while (!feof(fp))
		{
			fscanf(fp, "%[^;];%[^;];%f;%d;\n", func.nome, func.dept, &(func.ordenado), &aux);
			if (((ptr = (FUNC*)malloc(sizeof(FUNC))) != NULL) && (InsertIni(list, ptr) != NULL)) {
				strcpy(ptr->nome, func.nome);
				strcpy(ptr->dept, func.dept);
				ptr->ordenado = func.ordenado;
				if (aux) ptr->do_quadro = TRUE;
				else ptr->do_quadro = FALSE;
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

void RemoveFirstNode(LIST* list)
{
	LIST_NODE* node;

	node = NEXT(*list);
	free(DATA(*list));
	free(*list);
	*list = node;
}

void OrdenadoTotal(LIST list)
{
	float total=0;

	while (list != NULL)
	{
		total = total + ((FUNC*)DATA(list))->ordenado;
		list = NEXT(list);
	}

	printf("Total de ordenados pagos >> %.1f", total);
}

void ColocarQuadro(LIST* list)
{
	char nome[MAX_NOME];
	LIST_NODE* temp = *list;

	printf("Insira o nome do funcionário que deseja colocar no quadro >> ");
	scanf("%[^\n]", &nome);

	while (temp != NULL)
	{
		if (!strcmp(((FUNC*)DATA(temp))->nome,nome))
		{
			if (((FUNC*)DATA(temp))->do_quadro == FALSE)
			{
				((FUNC*)DATA(temp))->do_quadro = TRUE;
				printf("\nO funcionario foi colocado no quadro\n");
			}
			else printf("\nO funcionario ja esta no quadro\n");
		}
		temp = NEXT(temp);
	}
}

void ShowFunc(LIST list)
{
	while (list != NULL)
	{
		printf("%s;%s;%.1f;%d\n", ((FUNC*)DATA(list))->nome, ((FUNC*)DATA(list))->dept, ((FUNC*)DATA(list))->ordenado, ((FUNC*)DATA(list))->do_quadro);
		list = NEXT(list);
	}
}

void EliminarFuncFora(LIST* list)
{
	LIST_NODE* temp = *list;
	LIST_NODE* ant = NULL;

	while (temp != NULL)
	{
		if (((FUNC*)DATA(temp))->do_quadro == FALSE)
		{
			if (ant == NULL)			// se for o primeiro o apontador da lista passa a ser o seguinte
				*list = NEXT(*list);
			else
				NEXT(ant) = NEXT(temp); //se nao for o primeiro  
		}
		else
			ant = temp; 
		temp = NEXT(temp);
	}
}
