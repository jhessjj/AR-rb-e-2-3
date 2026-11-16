#ifndef ARTISTA_H_RB
#define ARTISTA_H_RB

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estrutura.c"
#include "../contador/contador.h"

static int corArtistaRB(ArtistaRB *H) {
    if (H == NULL)
        return BLACK;
    return H->cor;
}

static void trocaCorArtistaRB(ArtistaRB *H) {
    cont.trocaCor++;
    H->cor = !H->cor;
    if (H->esq != NULL)
        H->esq->cor = !H->esq->cor;
    if (H->dir != NULL)
        H->dir->cor = !H->dir->cor;
}

static ArtistaRB* rotacionaDireitaArtistaRB(ArtistaRB *A) {
    ArtistaRB *B = A->esq;
    cont.rotacao++;
    A->esq = B->dir;
    B->dir = A;
    B->cor = A->cor;
    A->cor = RED;
    return B;
}

static ArtistaRB* rotacionaEsquerdaArtistaRB(ArtistaRB *A) {
    ArtistaRB *B = A->dir;
    cont.rotacao++;
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

    cont.insere++;
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

    if (corArtistaRB(H->dir) == RED && corArtistaRB(H->esq) == BLACK)
        H = rotacionaEsquerdaArtistaRB(H);

    if (corArtistaRB(H->esq) == RED && corArtistaRB(H->esq->esq) == RED)
        H = rotacionaDireitaArtistaRB(H);

    if (corArtistaRB(H->esq) == RED && corArtistaRB(H->dir) == RED)
        trocaCorArtistaRB(H);

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

// retorna o no do artista ou null
ArtistaRB* buscaArtistaRB(ArtistaRB *H, char nome[]) {
    int cmp;
    cont.busca++;
    if (H == NULL)
        return NULL;
    cmp = strcmp(nome, H->nome);
    if (cmp == 0)
        return H;
    else if (cmp < 0)
        return buscaArtistaRB(H->esq, nome);
    return buscaArtistaRB(H->dir, nome);
}

// mostra todos em ordem alfabetica
void mostraArtistasRB(ArtistaRB *H) {
    if (H != NULL) {
        mostraArtistasRB(H->esq);
        printf("%s | %s | %s | %d album(ns)\n", H->nome, H->tipo, H->estilo, H->numAlbuns);
        mostraArtistasRB(H->dir);
    }
}

#endif