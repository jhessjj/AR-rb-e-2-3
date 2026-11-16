#ifndef ARTISTA_H_AVL
#define ARTISTA_H_AVL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estrutura.c"
#include "../contador/contador.h"
//SEGUE O ESTILO DA BINARIS
static int maiorAVL(int a, int b) {
    if (a > b)
        return a;
    return b;
}

static int alturaArtistaAVL(ArtistaAVL *No) {
    if (No == NULL)
        return -1;
    return No->altura;
}

static void atualizaAlturaArtistaAVL(ArtistaAVL **R) {
    cont.atualizaAltura++;
    (*R)->altura = 1 + maiorAVL(alturaArtistaAVL((*R)->esq), alturaArtistaAVL((*R)->dir));
}

static void rotacaoDirArtistaAVL(ArtistaAVL **P) {
    ArtistaAVL *A = *P;
    ArtistaAVL *B = A->esq;
    cont.rotacao++;
    A->esq = B->dir;
    B->dir = A;
    A->altura = 1 + maiorAVL(alturaArtistaAVL(A->esq), alturaArtistaAVL(A->dir));
    B->altura = 1 + maiorAVL(alturaArtistaAVL(B->esq), alturaArtistaAVL(B->dir));
    *P = B;
}

static void rotacaoEsqArtistaAVL(ArtistaAVL **P) {
    ArtistaAVL *A = *P;
    ArtistaAVL *B = A->dir;
    cont.rotacao++;
    A->dir = B->esq;
    B->esq = A;
    A->altura = 1 + maiorAVL(alturaArtistaAVL(A->esq), alturaArtistaAVL(A->dir));
    B->altura = 1 + maiorAVL(alturaArtistaAVL(B->esq), alturaArtistaAVL(B->dir));
    *P = B;
}

static void balanceamentoArtistaAVL(ArtistaAVL **P) {
    int fb, fbE, fbD;
    ArtistaAVL *filhoEsq, *filhoDir;

    cont.balanceamento++;
    fb = alturaArtistaAVL((*P)->esq) - alturaArtistaAVL((*P)->dir);
    if (fb == 2) {
        filhoEsq = (*P)->esq;
        fbE = alturaArtistaAVL(filhoEsq->esq) - alturaArtistaAVL(filhoEsq->dir);
        if (fbE < 0)
            rotacaoEsqArtistaAVL(&((*P)->esq));
        rotacaoDirArtistaAVL(P);
    }
    else if (fb == -2) {
        filhoDir = (*P)->dir;
        fbD = alturaArtistaAVL(filhoDir->esq) - alturaArtistaAVL(filhoDir->dir);
        if (fbD > 0)
            rotacaoDirArtistaAVL(&((*P)->dir));
        rotacaoEsqArtistaAVL(P);
    }
}

ArvArtistaAVL* criaArvArtistaAVL() {
    ArvArtistaAVL *raiz = (ArvArtistaAVL*) malloc(sizeof(ArvArtistaAVL));
    if (raiz != NULL)
        *raiz = NULL;
    return raiz;
}


ArtistaAVL* criaNoArtistaAVL(char nome[], char tipo[], char estilo[]) {
    ArtistaAVL *No = (ArtistaAVL*) malloc(sizeof(ArtistaAVL));
    if (No != NULL) {
        strcpy(No->nome, nome);
        strcpy(No->tipo, tipo);
        strcpy(No->estilo, estilo);
        No->numAlbuns = 0;
        No->albuns = NULL;
        No->altura = 0;
        No->esq = NULL;
        No->dir = NULL;
    }
    return No;
}


int insereArtistaAVL(ArtistaAVL **R, ArtistaAVL *No) {
    int cmp, inseriu;

    cont.insere++;
    if (*R == NULL) {
        *R = No;
        return 1;
    }
    cmp = strcmp(No->nome, (*R)->nome);
    if (cmp == 0)
        return 0;
    if (cmp < 0)
        inseriu = insereArtistaAVL(&((*R)->esq), No);
    else
        inseriu = insereArtistaAVL(&((*R)->dir), No);

    balanceamentoArtistaAVL(R);
    atualizaAlturaArtistaAVL(R);
    return inseriu;
}


int cadastraArtistaAVL(ArvArtistaAVL *raiz, char nome[], char tipo[], char estilo[]) {
    ArtistaAVL *No;
    int resp;
    if (raiz == NULL)
        return 0;
    No = criaNoArtistaAVL(nome, tipo, estilo);
    if (No == NULL)
        return 0;
    resp = insereArtistaAVL(raiz, No);
    if (resp == 0)
        free(No);
    return resp;
}

ArtistaAVL* buscaArtistaAVL(ArtistaAVL *H, char nome[]) {
    int cmp;
    cont.busca++;
    if (H == NULL)
        return NULL;
    cmp = strcmp(nome, H->nome);
    if (cmp == 0)
        return H;
    else if (cmp < 0)
        return buscaArtistaAVL(H->esq, nome);
    return buscaArtistaAVL(H->dir, nome);
}

// mostra todos em ordem alfabetica
void mostraArtistasAVL(ArtistaAVL *H) {
    if (H != NULL) {
        mostraArtistasAVL(H->esq);
        printf("%s | %s | %s | %d album(ns)\n", H->nome, H->tipo, H->estilo, H->numAlbuns);
        mostraArtistasAVL(H->dir);
    }
}

#endif