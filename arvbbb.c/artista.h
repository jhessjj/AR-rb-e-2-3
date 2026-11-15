#ifndef ARTISTA_H_BB
#define ARTISTA_H_BB

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estrutura.c"



ArvArtista* criaArvArtista() {
    ArvArtista *raiz = (ArvArtista*) malloc(sizeof(ArvArtista));
    if (raiz != NULL)
        *raiz = NULL;
    return raiz;
}


Artista* insereArtista(Artista *H, char nome[], char tipo[], char estilo[], int *resp) {
    Artista *novo;
    int cmp;

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
        *resp = 0; //repetido                               
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
    if (H == NULL)
        return NULL;
    cmp = strcmp(nome, H->nome);
    if (cmp == 0)
        return H;
    else if (cmp < 0)
        return buscaArtista(H->esq, nome);
    return buscaArtista(H->dir, nome);
}

//mostra em ordem alfabetica
void mostraArtistas(Artista *H) {
    if (H != NULL) {
        mostraArtistas(H->esq);
        printf("%s | %s | %s | %d album(ns)\n", H->nome, H->tipo, H->estilo, H->numAlbuns);
        mostraArtistas(H->dir);
    }
}

#endif