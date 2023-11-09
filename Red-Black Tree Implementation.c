#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define RUBRO_NEGRA 1000

typedef enum
{
	RUBRO,
	NEGRO
} rubroNegra_cor;

typedef struct rubroNegra_no
{
	int chave;
	rubroNegra_cor cor;
	struct rubroNegra_no *pai, *esquerda, *direita;
} rubroNegra_no;

typedef struct
{
	rubroNegra_no *raiz, *nil;
} rubroNegra_arvore;

// Função para criar uma árvore rubro-negra
rubroNegra_arvore criar_RUBRO_NEGRO_arvore()
{
	rubroNegra_arvore arvore;

	arvore.nil = malloc(sizeof(rubroNegra_no));
	arvore.nil->cor = NEGRO;
	arvore.raiz = arvore.nil;

	return arvore;
}

// Rotação para a esquerda na árvore
void esquerda_rotacao(rubroNegra_arvore *arvore, rubroNegra_no *x)
{
	rubroNegra_no *y = x->direita;
	x->direita = y->esquerda;

	if (y->esquerda != arvore->nil)
		y->esquerda->pai = x;

	y->pai = x->pai;

	if (x->pai == arvore->nil)
		arvore->raiz = y;
	else if (x == x->pai->esquerda)
		x->pai->esquerda = y;
	else
		x->pai->direita = y;

	y->esquerda = x;
	x->pai = y;
}

// Rotação para a direita na árvore
void direita_rotacao(rubroNegra_arvore *arvore, rubroNegra_no *y)
{
	rubroNegra_no *x = y->esquerda;
	y->esquerda = x->direita;

	if (x->direita != arvore->nil)
		x->direita->pai = y;

	x->pai = y->pai;

	if (x->pai == arvore->nil)
		arvore->raiz = x;
	else if (y == y->pai->esquerda)
		y->pai->esquerda = x;
	else
		y->pai->direita = x;

	x->direita = y;
	y->pai = x;
}

// Função para inserir um novo nó na árvore
void inserir(rubroNegra_arvore *arvore, int chave)
{
	rubroNegra_no *x = arvore->raiz, *y = arvore->nil;

	while (x != arvore->nil)
	{
		y = x;

		if (chave < x->chave)
			x = x->esquerda;
		else
			x = x->direita;
	}

	rubroNegra_no *z = malloc(sizeof(rubroNegra_no));
	z->chave = chave;
	z->cor = RUBRO;
	z->pai = y;
	z->esquerda = arvore->nil;
	z->direita = arvore->nil;

	if (y == arvore->nil)
		arvore->raiz = z;
	else if (chave < y->chave)
		y->esquerda = z;
	else
		y->direita = z;

	inserir_ajuste(arvore, z);
}

// Função para ajustar a árvore após a inserção de um nó
void inserir_ajuste(rubroNegra_arvore *arvore, rubroNegra_no *z)
{
	while (z->pai->cor == RUBRO)
	{
		if (z->pai == z->pai->pai->esquerda)
		{
			rubroNegra_no *y = z->pai->pai->direita;

			if (y->cor == RUBRO)
			{
				z->pai->cor = NEGRO;
				y->cor = NEGRO;
				z->pai->pai->cor = RUBRO;
				z = z->pai->pai;
			}
			else
			{
				if (z == z->pai->direita)
				{
					z = z->pai;
					esquerda_rotacao(arvore, z);
				}

				z->pai->cor = NEGRO;
				z->pai->pai->cor = RUBRO;
				direita_rotacao(arvore, z->pai->pai);
			}
		}
		else
		{
			rubroNegra_no *y = z->pai->pai->esquerda;

			if (y->cor == RUBRO)
			{
				z->pai->cor = NEGRO;
				y->cor = NEGRO;
				z->pai->pai->cor = RUBRO;
				z = z->pai->pai;
			}
			else
			{
				if (z == z->pai->esquerda)
				{
					z = z->pai;
					direita_rotacao(arvore, z);
				}

				z->pai->cor = NEGRO;
				z->pai->pai->cor = RUBRO;
				esquerda_rotacao(arvore, z->pai->pai);
			}
		}
	}

	arvore->raiz->cor = NEGRO;
}

// Função para encontrar um nó com uma chave específica na árvore
rubroNegra_no *get_no(rubroNegra_no *aux, rubroNegra_no *nil, int chave)
{
	if (aux == nil)
		return nil;
	else if (aux->chave > chave)
		return get_no(aux->esquerda, nil, chave);
	else if (aux->chave < chave)
		return get_no(aux->direita, nil, chave);
	else
		return aux;
}

// Função para realizar uma troca de nós na árvore
void mudanca(rubroNegra_arvore *arvore, rubroNegra_no *u, rubroNegra_no *v)
{
	if (u->pai == arvore->nil)
		arvore->raiz = v;
	else if (u == u->pai->esquerda)
		u->pai->esquerda = v;
	else
		u->pai->direita = v;

	v->pai = u->pai;
}

// Função para encontrar o nó mínimo em uma subárvore
rubroNegra_no *arvore_minima(rubroNegra_arvore *arvore, rubroNegra_no *x)
{
	while (x->esquerda != arvore->nil)
		x = x->esquerda;

	return x;
}

// Função para ajustar a árvore após a exclusão de um nó
void deletar_ajuste(rubroNegra_arvore *arvore, rubroNegra_no *x)
{
	while (x != arvore->raiz && x->cor == NEGRO)
	{
		if (x == x->pai->esquerda)
		{
			rubroNegra_no *w = x->pai->direita;

			if (w->cor == RUBRO)
			{
				w->cor = NEGRO;
				x->pai->cor = RUBRO;
				esquerda_rotacao(arvore, x->pai);
				w = x->pai->direita;
			}

			if (w->esquerda->cor == NEGRO && w->direita->cor == NEGRO)
			{
				w->cor = RUBRO;
				x = x->pai;
			}
			else
			{
				if (w->direita->cor == NEGRO)
				{
					w->esquerda->cor = NEGRO;
					w->cor = RUBRO;
					direita_rotacao(arvore, w);
					w = x->pai->direita;
				}

				w->cor = x->pai->cor;
				x->pai->cor = NEGRO;
				w->direita->cor = NEGRO;
				esquerda_rotacao(arvore, x->pai);
				x = arvore->raiz;
			}
		}
		else
		{
			rubroNegra_no *w = x->pai->esquerda;

			if (w->cor == RUBRO)
			{
				w->cor = NEGRO;
				x->pai->cor = RUBRO;
				direita_rotacao(arvore, x->pai);
				w = x->pai->esquerda;
			}

			if (w->direita->cor == NEGRO && w->esquerda->cor == NEGRO)
			{
				w->cor = RUBRO;
				x = x->pai;
			}
			else
			{
				if (w->esquerda->cor == NEGRO)
				{
					w->direita->cor = NEGRO;
					w->cor = RUBRO;
					esquerda_rotacao(arvore, w);
					w = x->pai->esquerda;
				}

				w->cor = x->pai->cor;
				x->pai->cor = NEGRO;
				w->esquerda->cor = NEGRO;
				direita_rotacao(arvore, x->pai);
				x = arvore->raiz;
			}
		}
	}

	x->cor = NEGRO;
}

// Função para excluir um nó da árvore
void deletar(rubroNegra_arvore *arvore, int chave)
{
	rubroNegra_no *z = get_no(arvore->raiz, arvore->nil, chave);

	if (z == arvore->nil)
		return;

	rubroNegra_no *y = z, *x;
	rubroNegra_cor y_original_cor = y->cor;

	if (z->esquerda == arvore->nil)
	{
		x = z->direita;
		mudanca(arvore, z, z->direita);
	}
	else if (z->direita == arvore->nil)
	{
		x = z->esquerda;
		mudanca(arvore, z, z->esquerda);
	}
	else
	{
		y = arvore_minima(arvore, z->direita);
		y_original_cor = y->cor;
		x = y->direita;

		if (y->pai == z)
			x->pai = y;
		else
		{
			mudanca(arvore, y, y->direita);
			y->direita = z->direita;
			y->direita->pai = y;
		}

		mudanca(arvore, z, y);
		y->esquerda = z->esquerda;
		y->esquerda->pai = y;
		y->cor = z->cor;
	}

	if (y_original_cor == NEGRO)
		deletar_ajuste(arvore, x);

	free(z);
}

// Função para liberar todos os nós da árvore
void free_caminho(rubroNegra_no *aux, rubroNegra_no *nil)
{
	if (aux != nil)
	{
		free_caminho(aux->esquerda, nil);
		free_caminho(aux->direita, nil);
		free(aux);
	}
}

// Função para liberar a memória da árvore rubro-negra
void free_RUBRO_NEGRO_arvore(rubroNegra_arvore *arvore)
{
	free_caminho(arvore->raiz, arvore->nil);
	free(arvore->nil);
}

// Função para gerar um array de chaves aleatórias
int *get_chaves(int chaves_qty)
{
	int *chaves = malloc(sizeof(int) * chaves_qty);

	srand(time(NULL));

	for (int i = 0; i < chaves_qty;)
	{
		bool is_chave_repeated = true;
		int new_chave = rand();

		for (int j = i - 1; j >= 0 && is_chave_repeated; j--)
			if (new_chave == chaves[j])
				is_chave_repeated = false;

		if (is_chave_repeated)
		{
			chaves[i] = new_chave;
			i++;
		}
	}

	return chaves;
}

// Função para calcular a altura do caminho de um nó negro na árvore
int NEGRO_altura_caminho(rubroNegra_no *aux, rubroNegra_no *nil)
{
	if (aux == nil)
		return 1;

	int esquerda_altura = NEGRO_altura_caminho(aux->esquerda, nil),
		direita_altura = NEGRO_altura_caminho(aux->direita, nil);

	if (esquerda_altura != direita_altura || esquerda_altura == -1)
		return -1;
	else
		return esquerda_altura + (aux->cor == NEGRO ? 1 : 0);
}

// Função para calcular a altura da árvore com nós negros
int get_NEGRO_altura(rubroNegra_no *aux, rubroNegra_no *nil)
{
	if (aux == nil)
		return 1;

	int esquerda_altura = NEGRO_altura_caminho(aux->esquerda, nil),
		direita_altura = NEGRO_altura_caminho(aux->direita, nil);

	if (esquerda_altura != direita_altura || esquerda_altura == -1)
		return -1;
	else
		return esquerda_altura;
}

// Função para verificar se a árvore é uma árvore rubro-negra válida
bool RUBRO_NEGRO_arvore(rubroNegra_no *aux, rubroNegra_no *nil)
{
	if (aux == nil)
		return true;

	if (get_NEGRO_altura(aux, nil) != -1)
		if (RUBRO_NEGRO_arvore(aux->esquerda, nil))
			return RUBRO_NEGRO_arvore(aux->direita, nil);

	return false;
}

// Função para contar o número de nós na árvore
int conta_nos(rubroNegra_no *aux, rubroNegra_no *nil)
{
	if (aux == nil)
		return 0;

	return 1 + conta_nos(aux->esquerda, nil) + conta_nos(aux->direita, nil);
}

int main()
{
	for (int i = 0; i < RUBRO_NEGRA; i++)
	{
		rubroNegra_arvore arvore = criar_RUBRO_NEGRO_arvore();
		const int chaves_qty = 10000;
		int *chaves = get_chaves(chaves_qty);

		printf("\nThe red-black tree was created - %d.", i + 1);
		printf("\nInserting %d nodes.\n", chaves_qty);
		for (int j = 0; j < chaves_qty; j++)
			inserir(&arvore, chaves[j]);

		int nos_qty = conta_nos(arvore.raiz, arvore.nil);

		if (chaves_qty == nos_qty)
			puts("The nodes have been placed successfully.");
		else
		{
			printf("Allocation error. %d Keys placed.\n", nos_qty);
			exit(EXIT_FAILURE);
		}

		if (RUBRO_NEGRO_arvore(arvore.raiz, arvore.nil))
			puts("It's still a red-black tree.");
		else
		{
			puts("It is no longer a red-black tree.");
			exit(EXIT_FAILURE);
		}

		int deletar_qty = chaves_qty / 10;
		printf("Deleting %d nodes.\n", deletar_qty);
		for (int j = 0; j < deletar_qty; j++)
			deletar(&arvore, chaves[j]);

		nos_qty = conta_nos(arvore.raiz, arvore.nil);

		if (chaves_qty - deletar_qty == nos_qty)
			puts("All nodes were successfully deleted.");
		else
		{
			printf("Allocation error. %d The keys were placed in the tree.\n", nos_qty);
			exit(EXIT_FAILURE);
		}

		if (RUBRO_NEGRO_arvore(arvore.raiz, arvore.nil))
			puts("It's still a red-black tree.");
		else
		{
			puts("It is no longer a red-black tree.");
			exit(EXIT_FAILURE);
		}

		free(chaves);
		free_RUBRO_NEGRO_arvore(&arvore);
	}
	return 0;
}
