#ifndef MUSICA_H_BB
#define MUSICA_H_BB

#include "album.h"



Musica* insereMusica(Musica *H, char titulo[], int minutos, int *resp) {
    Musica *novo;
    int cmp;

    if (H == NULL) {
        novo = (Musica*) malloc(sizeof(Musica));
        if (novo == NULL) {
            *resp = 0;
            return NULL;
        }
        strcpy(novo->titulo, titulo);
        novo->minutos = minutos;
        novo->esq = NULL;
        novo->dir = NULL;
        *resp = 1;
        return novo;
    }

    cmp = strcmp(titulo, H->titulo);
    if (cmp == 0)
        *resp = 0;                                
    else if (cmp < 0)
        H->esq = insereMusica(H->esq, titulo, minutos, resp);
    else
        H->dir = insereMusica(H->dir, titulo, minutos, resp);

    return H;
}

Musica* buscaMusica(Musica *H, char titulo[]) {
    int cmp;
    if (H == NULL)
        return NULL;
    cmp = strcmp(titulo, H->titulo);
    if (cmp == 0)
        return H;
    else if (cmp < 0)
        return buscaMusica(H->esq, titulo);
    return buscaMusica(H->dir, titulo);
}


int cadastraMusica(ArvArtista *raiz, char nomeArtista[], char tituloAlbum[], char titulo[], int minutos) {
    Artista *art;
    Album *alb;
    int resp;
    if (raiz == NULL)
        return -1;
    art = buscaArtista(*raiz, nomeArtista);
    if (art == NULL)
        return -1;
    alb = buscaAlbum(art->albuns, tituloAlbum);
    if (alb == NULL)
        return -2;
    alb->musicas = insereMusica(alb->musicas, titulo, minutos, &resp);
    if (resp == 1)
        alb->qtdMusicas++;
    return resp;
}


int mostraMusicaNosAlbuns(Album *A, Artista *art, char titulo[]) {
    Musica *m;
    int total = 0;
    if (A == NULL)
        return 0;
    total += mostraMusicaNosAlbuns(A->esq, art, titulo);
    m = buscaMusica(A->musicas, titulo);
    if (m != NULL) {
        printf("Musica : %s (%d min)\n", m->titulo, m->minutos);
        printf("Artista: %s (%s)\n", art->nome, art->tipo);
        printf("Album  : %s (%d)\n", A->titulo, A->ano);
        printf("Estilo : %s\n\n", art->estilo);
        total++;
    }
    total += mostraMusicaNosAlbuns(A->dir, art, titulo);
    return total;
}


int mostraDadosMusica(Artista *R, char titulo[]) {
    int total = 0;
    if (R == NULL)
        return 0;
    total += mostraDadosMusica(R->esq, titulo);
    total += mostraMusicaNosAlbuns(R->albuns, R, titulo);
    total += mostraDadosMusica(R->dir, titulo);
    return total;
}


Musica* procuraMenorMusica(Musica *atual) {
    Musica *no1 = atual;
    Musica *no2 = atual->esq;
    while (no2 != NULL) {
        no1 = no2;
        no2 = no2->esq;
    }
    return no1;
}


Musica* removeMenorMusica(Musica *H) {
    Musica *aux;
    if (H->esq == NULL) {
        aux = H->dir;
        free(H);
        return aux;
    }
    H->esq = removeMenorMusica(H->esq);
    return H;
}


Musica* removeMusica(Musica *H, char titulo[], int *resp) {
    Musica *aux;
    int cmp;

    if (H == NULL) {
        *resp = 0;
        return NULL;
    }

    cmp = strcmp(titulo, H->titulo);
    if (cmp < 0)
        H->esq = removeMusica(H->esq, titulo, resp);
    else if (cmp > 0)
        H->dir = removeMusica(H->dir, titulo, resp);
    else {                                        
        *resp = 1;
        if (H->esq == NULL) {                     
            aux = H->dir;
            free(H);
            return aux;
        }
        if (H->dir == NULL) {                     
            aux = H->esq;
            free(H);
            return aux;
        }
        aux = procuraMenorMusica(H->dir);    //usando o sucessor
        strcpy(H->titulo, aux->titulo);
        H->minutos = aux->minutos;
        H->dir = removeMenorMusica(H->dir);
    }
    return H;
}

#endif