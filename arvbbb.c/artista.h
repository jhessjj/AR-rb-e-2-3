#ifndef ARTISTA_H_BB
#define ARTISTA_H_BB

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estrutura.c"
#include "../contador/contador.h"

ArvArtista* criaArvArtista() {
    ArvArtista *raiz = (ArvArtista*) malloc(sizeof(ArvArtista));
    if (raiz != NULL)
        *raiz = NULL;
    return raiz;
}

Artista* insereArtista(Artista *H, char nome[], char tipo[], char estilo[], int *resp) {
    Artista *novo;
    int cmp;

    cont.insere++;
    if (H == NULL) {
        novo = (Artista*) malloc(sizeof(Artista));
        if (novo == NULL) {
            *resp = 0;
            return NULL;
        }
        strcpy(novo->nome, nome);
        strcpy(novo->tipo, tipo);
        strcpy(novo->estilo, estilo);
        novo->numAlbuns = 0;
        novo->albuns = NULL;
        novo->esq = NULL;
        novo->dir = NULL;
        *resp = 1;
        return novo;
    }

    cmp = strcmp(nome, H->nome);
    if (cmp == 0)
        *resp = 0;
    else if (cmp < 0)
        H->esq = insereArtista(H->esq, nome, tipo, estilo, resp);
    else
        H->dir = insereArtista(H->dir, nome, tipo, estilo, resp);

    return H;
}


int cadastraArtista(ArvArtista *raiz, char nome[], char tipo[], char estilo[]) {
    int resp;
    if (raiz == NULL)
        return 0;
    *raiz = insereArtista(*raiz, nome, tipo, estilo, &resp);
    return resp;
}

Artista* buscaArtista(Artista *H, char nome[]) {
    int cmp;
    cont.busca++;
    if (H == NULL)
        return NULL;
    cmp = strcmp(nome, H->nome);
    if (cmp == 0)
        return H;
    else if (cmp < 0)
        return buscaArtista(H->esq, nome);
    return buscaArtista(H->dir, nome);
}

// mostra todos em ordem alfabetica
void mostraArtistas(Artista *H) {
    if (H != NULL) {
        mostraArtistas(H->esq);
        printf("%s | %s | %s | %d album(ns)\n", H->nome, H->tipo, H->estilo, H->numAlbuns);
        mostraArtistas(H->dir);
    }
}

#endif