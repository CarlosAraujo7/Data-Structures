#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define AVL 100
#define SIZE_AVL 1000
#define limite_elementos 100000

struct no {
    int Chave;
    int bal;
    struct no *esquerda;
    struct no *direita;
};

typedef struct no No;

// Função para calcular a altura de um nó na AVL
int altura_AVL(No *pt) {
    if (pt == NULL) {
        return 0;
    }

    int altura_esquerda = altura_AVL(pt->esquerda);
    int altura_direita = altura_AVL(pt->direita);

    // A altura do nó é o máximo entre a altura da subárvore esquerda e direita, mais 1
    return 1 + (altura_esquerda > altura_direita ? altura_esquerda : altura_direita);
}

void comprimento_altura(No *pt, int *size){
    if (pt == NULL){
        *size = 0;
        return;
    }
    (*size)++;
    if (pt->esquerda != NULL)
        comprimento_altura(pt->esquerda, size);
    if (pt->direita != NULL)
        comprimento_altura(pt->direita, size);
}

void regular_AVL(No *pt, bool *altura){
    if (pt == NULL){
        return;
    }
    int bal = (altura_AVL(pt->direita) - altura_AVL(pt->esquerda));
    if (bal > 1 || bal < -1){
        *altura = false;
    }
    regular_AVL(pt->esquerda, altura);
    regular_AVL(pt->direita, altura);
}


// Função para verificar se a árvore é AVL e calcular a altura
int verificar_AVL(No *pt, bool *ehAVL) {
    if (pt == NULL) {
        return 0;
    }

    int altura_esquerda = verificar_AVL(pt->esquerda, ehAVL);
    int altura_direita = verificar_AVL(pt->direita, ehAVL);

    // Verificar o balanceamento do nó
    int balanceamento = altura_direita - altura_esquerda;

    if (balanceamento > 1 || balanceamento < -1) {
        *ehAVL = false;
    }

    // A altura do nó é o máximo entre a altura da subárvore esquerda e direita, mais 1
    return 1 + (altura_esquerda > altura_direita ? altura_esquerda : altura_direita);
}

// Função para calcular a quantidade de nós em uma subárvore
void contar_nos(No *pt, int *size) {
    if (pt == NULL) {
        return;
    }
    (*size)++;
    contar_nos(pt->esquerda, size);
    contar_nos(pt->direita, size);
}

// Função para percorrer a árvore em pré-ordem e imprimir informações dos nós
void pre_ordem(No *pt) {
    if (pt == NULL) {
        return;
    }
    printf("\n%d [%d] |", pt->Chave, pt->bal);
    pre_ordem(pt->esquerda);
    pre_ordem(pt->direita);
}

// Função para realizar uma rotação para a esquerda durante a inserção
void rotacao_inserir_esquerda(No **pt, bool *altura) {
    No *ptu = (*pt)->esquerda;
    if (ptu->bal == -1) {
        (*pt)->esquerda = ptu->direita;
        ptu->direita = (*pt);
        (*pt) = ptu;
        (*pt)->bal = 0;
        (*pt)->direita->bal = 0;
    } else {
        No *ptv = ptu->direita;
        ptu->direita = ptv->esquerda;
        ptv->esquerda = ptu;
        (*pt)->esquerda = ptv->direita;
        ptv->direita = (*pt);

        if (ptv->bal == 1) {
            (*pt)->bal = 0;
            ptu->bal = -1;
        } else if (ptv->bal == 0) {
            (*pt)->bal = 0;
            ptu->bal = 0;
        } else {
            (*pt)->bal = 1;
            ptu->bal = 0;
        }
        (*pt) = ptv;
    }
    (*pt)->bal = 0;
    *altura = false;
}

// Função para realizar uma rotação para a direita durante a inserção
void rotacao_inserir_direita(No **pt, bool *altura) {
    No *ptu = (*pt)->direita;

    if (ptu->bal == 1) {
        (*pt)->direita = ptu->esquerda;
        ptu->esquerda = (*pt);
        (*pt) = ptu;
        (*pt)->bal = 0;
        (*pt)->esquerda->bal = 0;
    } else {
        No *ptv = ptu->esquerda;
        ptu->esquerda = ptv->direita;
        ptv->direita = ptu;
        (*pt)->direita = ptv->esquerda;
        ptv->esquerda = (*pt);

        if (ptv->bal == 1) {
            (*pt)->bal = -1;
            ptu->bal = 0;
        } else if (ptv->bal == 0) {
            (*pt)->bal = 0;
            ptu->bal = 0;
        } else {
            ptu->bal = 1;
            (*pt)->bal = 0;
        }
        (*pt) = ptv;
    }
    (*pt)->bal = 0;
    *altura = false;
}

// Função para inserir um elemento na AVL
void inserir_AVL(int x, No **pt, bool *altura) {
    if (*pt == NULL) {
        No *new = (No *)malloc(sizeof(No));
        new->Chave = x;
        new->bal = 0;
        new->esquerda = NULL;
        new->direita = NULL;
        (*pt) = new;
        *altura = true;
    } else {
        if (x == (*pt)->Chave) {
            *altura = false;
            printf("\nElement found.");
        } else if (x < (*pt)->Chave) {
            inserir_AVL(x, &(*pt)->esquerda, altura);
            if (*altura) {
                switch ((*pt)->bal) {
                    case 1:
                        (*pt)->bal = 0;
                        *altura = false;
                        break;
                    case 0:
                        (*pt)->bal = -1;
                        break;
                    case -1:
                        rotacao_inserir_esquerda(pt, altura);
                        break;
                }
            }
        } else {
            inserir_AVL(x, &(*pt)->direita, altura);
            if (*altura) {
                switch ((*pt)->bal) {
                    case 1:
                        rotacao_inserir_direita(pt, altura);
                        break;
                    case 0:
                        (*pt)->bal = 1;
                        break;
                    case -1:
                        (*pt)->bal = 0;
                        *altura = false;
                        break;
                }
            }
        }
    }
}

// Função para realizar uma rotação para a esquerda durante a remoção
void rotacao_remover_esquerda(No **pt, bool *altura) {
    No *ptu = (*pt)->esquerda;

    if (ptu->bal <= 0) {
        (*pt)->esquerda = ptu->direita;
        ptu->direita = (*pt);
        (*pt) = ptu;
        
        if ((*pt)->bal == -1) {
            (*pt)->bal = 0;
            (*pt)->direita->bal = 0;
            *altura = true;
        } else if ((*pt)->bal == 0) {
            (*pt)->bal = 1;
            (*pt)->direita->bal = -1;
            *altura = false;
        }
    } else {
        No *ptv = ptu->direita;
        ptu->direita = ptv->esquerda;
        ptv->esquerda = ptu;
        (*pt)->esquerda = ptv->direita;
        ptv->direita = (*pt);

        switch (ptv->bal) {
            case 1:
                (*pt)->bal = 0;
                ptu->bal = -1;
                break;
            case 0:
                (*pt)->bal = 0;
                ptu->bal = 0;
                break;
            case -1:
                (*pt)->bal = 1;
                ptu->bal = 0;
                break;
        }
        (*pt) = ptv;
        (*pt)->bal = 0;
        *altura = true;
    }
}

// Função para realizar uma rotação para a direita durante a remoção
void rotacao_remover_direita(No **pt, bool *altura) {
    No *ptu = (*pt)->direita;

    if (ptu->bal >= 0) {
        (*pt)->direita = ptu->esquerda;
        ptu->esquerda = (*pt);
        (*pt) = ptu;

        if ((*pt)->bal == 1) {
            (*pt)->bal = 0;
            (*pt)->esquerda->bal = 0;
            *altura = true;
        } else {
            (*pt)->bal = -1;
            (*pt)->esquerda->bal = 1;
            *altura = false;
        }
    } else {
        No *ptv = ptu->esquerda;
        ptu->esquerda = ptv->direita;
        ptv->direita = ptu;
        (*pt)->direita = ptv->esquerda;
        ptv->esquerda = (*pt);

        switch (ptv->bal) {
            case 1:
                ptu->bal = 0;
                (*pt)->bal = -1;
                break;
            case 0:
                ptu->bal = 0;
                (*pt)->bal = 0;
                break;
            case -1:
                ptu->bal = 1;
                (*pt)->bal = 0;
                break;
        }
        (*pt) = ptv;
        (*pt)->bal = 0;
        *altura = true;
    }
}

// Função para balancear uma árvore AVL após a remoção
void balancear_AVL(No **pt, int R, bool *altura) {
    if (*altura) {
        if (R) { 
            switch ((*pt)->bal) {
                case 1:
                    (*pt)->bal = 0;
                    break;
                case 0:
                    (*pt)->bal = -1;
                    *altura = false;
                    break;
                case -1:
                    rotacao_remover_esquerda(pt, altura);
                    break;
            }
        } else {
            switch ((*pt)->bal) {
                case 1:
                    rotacao_remover_direita(pt, altura);
                    break;
                case 0:
                    (*pt)->bal = 1;
                    *altura = false;
                    break;
                case -1:
                    (*pt)->bal = 0;
                    break;
            }
        }
    }
}

// Função para trocar dois nós na árvore AVL
void trocar_AVL(No **pt, No **pai_S) {
    int value = (*pt)->Chave;
    (*pt)->Chave = (*pai_S)->Chave;
    (*pai_S)->Chave = value;
}

// Função para remover um elemento da árvore AVL
void remover_AVL(int x, No **pt, bool *altura, No **pt_fonte_aux) {
    if ((*pt) == NULL) {
        *altura = false;
        printf("\nElement does not exist.");
    } else {
        if (x < (*pt)->Chave) {
            remover_AVL(x, &(*pt)->esquerda, altura, pt_fonte_aux);
            balancear_AVL(pt, 0, altura);
        } else if (x > (*pt)->Chave) {
            remover_AVL(x, &(*pt)->direita, altura, pt_fonte_aux);
            balancear_AVL(pt, 1, altura);
        } else {
            No *aux = (*pt);
            if ((*pt)->direita == NULL) {
                if ((*pt) == (*pt_fonte_aux)) {
                    (*pt) = (*pt)->esquerda;
                    (*pt_fonte_aux) = (*pt);
                } else {
                    (*pt) = (*pt)->esquerda;
                }
                *altura = true;
                free(aux);
            } else if ((*pt)->esquerda == NULL) {
                if ((*pt) == (*pt_fonte_aux)) {
                    (*pt) = (*pt)->direita;
                    (*pt_fonte_aux) = (*pt);
                } else {
                    (*pt) = (*pt)->direita;
                }
                *altura = true;
                free(aux);
            } else if ((*pt)->esquerda != NULL && (*pt)->direita != NULL) {
                No *s = (*pt)->direita;
                if (s->esquerda == NULL) {
                    s->esquerda = (*pt)->esquerda;
                    s->bal = (*pt)->bal;
                    (*pt) = s;
                    *altura = true;
                } else {
                    No *pai_S;
                    while (s->esquerda != NULL) {
                        pai_S = s;
                        s = s->esquerda;
                    }
                    trocar_AVL(pt, &pai_S->esquerda);
                    remover_AVL(x, &(*pt)->direita, altura, pt_fonte_aux);
                }
                balancear_AVL(pt, 1, altura);
            } else {
                if ((*pt) == (*pt_fonte_aux)) {
                    (*pt_fonte_aux) = NULL;
                }
                free((*pt));
                *altura = true;
            }
        }
    }
}

int main() {
    for (int i = 0; i < AVL; i++) {
        No *pt_fonte_aux = NULL;
        bool altura = false;
        srand(time(NULL));

        int array[SIZE_AVL];

        for (int j = 0; j < SIZE_AVL; j++) {
            bool checkValue = true;
            do {
                checkValue = true;
                int value = (rand() % (limite_elementos - 1)) + 1;
                for (int k = 0; k < j; k++) {
                    if (array[k] == value) {
                        checkValue = false;
                    }
                }
                if (checkValue) {
                    array[j] = value;
                    inserir_AVL(value, &pt_fonte_aux, &altura);
                }
            } while (!checkValue);
        }
        printf("\nThe tree was created - %d.", i + 1);

        int size = 0;
        No *size_AVL = pt_fonte_aux;
        comprimento_altura(size_AVL, &size);
        printf("\nNodes: %d", size);

        bool bal = true;
        regular_AVL(pt_fonte_aux, &bal);
        if (!bal) {
            printf("\nThe tree is not avl");
            printf("Balancing tree...");
        } else {
            printf("\nThe tree is avl");
        }

        printf("\nRemoving: ");
        for (int i = 0; i < 100; i++) {
            remover_AVL(array[i], &pt_fonte_aux, &altura, &pt_fonte_aux);
        }

        size = 0;
        size_AVL = pt_fonte_aux;
        comprimento_altura(size_AVL, &size);
        printf("100 nodes");

        bal = true;
        regular_AVL(pt_fonte_aux, &bal);
        if (bal) {
            printf("\nThe tree is avl");
        }

        printf("\n");
    }

    return 0;
}
