#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "artista.h"


static int cor(ArtistaRB *H) {
    if (H == NULL)
        return BLACK;
    return H->cor;
}


static void trocaCor(ArtistaRB *H) {
    H->cor = !H->cor;
    if (H->esq != NULL)
        H->esq->cor = !H->esq->cor;
    if (H->dir != NULL)
        H->dir->cor = !H->dir->cor;
}

static ArtistaRB* rotacionaDireita(ArtistaRB *A) {
    ArtistaRB *B = A->esq;
    A->esq = B->dir;
    B->dir = A;
    B->cor = A->cor;
    A->cor = RED;
    return B;
}

static ArtistaRB* rotacionaEsquerda(ArtistaRB *A) {
    ArtistaRB *B = A->dir;
    A->dir = B->esq;
    B->esq = A;
    B->cor = A->cor;
    A->cor = RED;
    return B;
}



ArvArtistaRB* criaArvArtistaRB() {
    ArvArtistaRB *raiz = (ArvArtistaRB*) malloc(sizeof(ArvArtistaRB));
    if (raiz != NULL)
        *raiz = NULL;
    return raiz;
}

ArtistaRB* insereArtistaRB(ArtistaRB *H, char nome[], char tipo[], char estilo[], int *resp) {
    ArtistaRB *novo;
    int cmp;

    if (H == NULL) {
        novo = (ArtistaRB*) malloc(sizeof(ArtistaRB));
        if (novo == NULL) {
            *resp = 0;
            return NULL;
        }
        strcpy(novo->nome, nome);
        strcpy(novo->tipo, tipo);
        strcpy(novo->estilo, estilo);
        novo->numAlbuns = 0;
        novo->albuns = NULL;
        novo->cor = RED;                         
        novo->esq = NULL;
        novo->dir = NULL;
        *resp = 1;
        return novo;
    }

    cmp = strcmp(nome, H->nome);
    if (cmp == 0)
        *resp = 0;                               
    else if (cmp < 0)
        H->esq = insereArtistaRB(H->esq, nome, tipo, estilo, resp);
    else
        H->dir = insereArtistaRB(H->dir, nome, tipo, estilo, resp);

    
    if (cor(H->dir) == RED && cor(H->esq) == BLACK)
        H = rotacionaEsquerda(H);

    if (cor(H->esq) == RED && cor(H->esq->esq) == RED)
        H = rotacionaDireita(H);

    if (cor(H->esq) == RED && cor(H->dir) == RED)
        trocaCor(H);

    return H;
}

int cadastraArtistaRB(ArvArtistaRB *raiz, char nome[], char tipo[], char estilo[]) {
    int resp;
    if (raiz == NULL)
        return 0;
    *raiz = insereArtistaRB(*raiz, nome, tipo, estilo, &resp);
    if (*raiz != NULL)
        (*raiz)->cor = BLACK;                     
    return resp;
}