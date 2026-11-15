#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "artista.h"


static int altura(ArtistaAVL *No) {
    if (No == NULL)
        return -1;
    return No->altura;
}

static int maior(int a, int b) {
    if (a > b)
        return a;
    return b;
}


static int fatorBalanceamento(ArtistaAVL *No) {
    return altura(No->esq) - altura(No->dir);
}

static ArtistaAVL* rotacionaDireita(ArtistaAVL *A) {
    ArtistaAVL *B = A->esq;
    A->esq = B->dir;
    B->dir = A;
    A->altura = 1 + maior(altura(A->esq), altura(A->dir));
    B->altura = 1 + maior(altura(B->esq), altura(B->dir));
    return B;
}

static ArtistaAVL* rotacionaEsquerda(ArtistaAVL *A) {
    ArtistaAVL *B = A->dir;
    A->dir = B->esq;
    B->esq = A;
    A->altura = 1 + maior(altura(A->esq), altura(A->dir));
    B->altura = 1 + maior(altura(B->esq), altura(B->dir));
    return B;
}

static ArtistaAVL* balancear(ArtistaAVL *H) {
    int fb;
    H->altura = 1 + maior(altura(H->esq), altura(H->dir));
    fb = fatorBalanceamento(H);

    if (fb > 1) {                                  
        if (fatorBalanceamento(H->esq) < 0)        
            H->esq = rotacionaEsquerda(H->esq);
        H = rotacionaDireita(H);
    }
    else if (fb < -1) {                            
        if (fatorBalanceamento(H->dir) > 0)        
            H->dir = rotacionaDireita(H->dir);
        H = rotacionaEsquerda(H);
    }
    return H;
}



ArvArtistaAVL* criaArvArtistaAVL() {
    ArvArtistaAVL *raiz = (ArvArtistaAVL*) malloc(sizeof(ArvArtistaAVL));
    if (raiz != NULL)
        *raiz = NULL;
    return raiz;
}

ArtistaAVL* insereArtistaAVL(ArtistaAVL *H, char nome[], char tipo[], char estilo[], int *resp) {
    ArtistaAVL *novo;
    int cmp;

    if (H == NULL) {
        novo = (ArtistaAVL*) malloc(sizeof(ArtistaAVL));
        if (novo == NULL) {
            *resp = 0;
            return NULL;
        }
        strcpy(novo->nome, nome);
        strcpy(novo->tipo, tipo);
        strcpy(novo->estilo, estilo);
        novo->numAlbuns = 0;
        novo->albuns = NULL;
        novo->altura = 0;                        
        novo->esq = NULL;
        novo->dir = NULL;
        *resp = 1;
        return novo;
    }

    cmp = strcmp(nome, H->nome);
    if (cmp == 0)
        *resp = 0;
    else if (cmp < 0)
        H->esq = insereArtistaAVL(H->esq, nome, tipo, estilo, resp);
    else
        H->dir = insereArtistaAVL(H->dir, nome, tipo, estilo, resp);

    return balancear(H);                          
}

int cadastraArtistaAVL(ArvArtistaAVL *raiz, char nome[], char tipo[], char estilo[]) {
    int resp;
    if (raiz == NULL)
        return 0;
    *raiz = insereArtistaAVL(*raiz, nome, tipo, estilo, &resp);
    return resp;
}