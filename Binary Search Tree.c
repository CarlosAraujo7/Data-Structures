#include <stdio.h>
#include <stdlib.h>

typedef struct vertice
{
    int chave;
    struct vertice *dir;
    struct vertice *esq;
} VERTICE;

VERTICE *raiz = NULL; // vazia!!!!
int tam = 0;

/*procedimento busca-arvore(x, pt, f)
    se pt = vazio então f = 0
    senão se x = pt.chave então f = 1
    senão se x < pt.chave então
            se pt.esq = vazio então f = 2
            senão pt = pt.esq
            busca-arvore(x, pt, f)
    senão se pt.dir = vazio então f = 3
        senão pt = pt.dir
        busca-arvore(x, pt, f)*/

VERTICE *buscar(int x, VERTICE *aux)
{
    if (aux == NULL)
    {
        return NULL; // vazia
    }
    else if (x == aux->chave)
    {
        return aux;
    }
    else if (x < aux->chave)
    { // esq
        if (aux->esq == NULL)
        {
            return aux;
        }
        else
        {
            return buscar(x, aux->esq);
        }
    }
    else
    {
        if (aux->dir == NULL)
        {
            return aux;
        }
        else
        {
            return buscar(x, aux->dir);
        }
    }
}

void adicionar(int chave)
{
    VERTICE *aux = buscar(chave, raiz);
    if (aux != NULL && aux->chave == chave)
    {
        // chave já existe na árvore!
        printf("insercao invalida!\n");
    }
    else
    {
        VERTICE *novo = malloc(sizeof(VERTICE));
        novo->chave = chave;
        novo->esq = NULL;
        novo->dir = NULL;
        if (aux == NULL)
        { // vazia
            raiz = novo;
        }
        else if (chave < aux->chave)
        { // esq
            aux->esq = novo;
        }
        else
        { // dir
            aux->dir = novo;
        }
    }
}

void in_ordem(VERTICE *aux)
{
    if (aux->esq != NULL)
    {
        in_ordem(aux->esq);
    }

    printf("%d ", aux->chave);

    if (aux->dir != NULL)
    {
        in_ordem(aux->dir);
    }
}

VERTICE *remover(VERTICE *aux, int v)
{
    if (aux == NULL)
        return NULL;
    else if (aux->chave > v)
        aux->esq = remover(aux->esq, v);
    else if (aux->chave < v)
        aux->dir = remover(aux->dir, v);
    else
    { /* aqui achou o nó a remover */
        /* nó sem filhos */
        if (aux->esq == NULL && aux->dir == NULL)
        {
            free(aux);
            aux = NULL;
        }
        /* o nó só tem filho à direita */
        else if (aux->esq == NULL)
        {
            VERTICE *t = aux;
            aux = aux->dir;
            free(t);
        }
        /* só tem filho à esquerda */
        else if (aux->dir == NULL)
        {
            VERTICE *t = aux;
            aux = aux->esq;
            free(t);
        }
        /* o nó tem os dois filhos */
        else
        {
            VERTICE *f = aux->esq;
            while (f->dir != NULL)
            {
                f = f->dir;
            }
            aux->chave = f->chave;
            f->chave = v;
            aux->esq = remover(aux->esq, v);
        }
    }
    return aux;
}

int main()
{

    adicionar(2);
    adicionar(7);
    adicionar(5);
    adicionar(1);
    adicionar(15);
    adicionar(8);
    adicionar(4);
    adicionar(3);
    adicionar(6);
    adicionar(19);
    adicionar(9);
    adicionar(12);
    adicionar(20);
    adicionar(18);
    adicionar(11);
    adicionar(16);
    adicionar(13);
    adicionar(10);
    adicionar(17);
    adicionar(14);
    printf("\nImprimindo arvore...\n");
    in_ordem(raiz);
    remover(raiz, 1);
    remover(raiz, 19);
    remover(raiz, 8);
    remover(raiz, 6);
    remover(raiz, 11);
    remover(raiz, 3);
    remover(raiz, 17);
    remover(raiz, 7);
    printf("\nImprimindo arvore...\n");
    in_ordem(raiz);
    return 0;
}
