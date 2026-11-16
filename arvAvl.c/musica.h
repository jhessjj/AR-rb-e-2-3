#ifndef MUSICA_H_AVL
#define MUSICA_H_AVL

#include "album.h"

static int alturaMusicaAVL(MusicaAVL *No) {
    if (No == NULL)
        return -1;
    return No->altura;
}

static void atualizaAlturaMusicaAVL(MusicaAVL **R) {
    (*R)->altura = 1 + maiorAVL(alturaMusicaAVL((*R)->esq), alturaMusicaAVL((*R)->dir));
}

static void rotacaoDirMusicaAVL(MusicaAVL **P) {
    MusicaAVL *A = *P;
    MusicaAVL *B = A->esq;
    A->esq = B->dir;
    B->dir = A;
    A->altura = 1 + maiorAVL(alturaMusicaAVL(A->esq), alturaMusicaAVL(A->dir));
    B->altura = 1 + maiorAVL(alturaMusicaAVL(B->esq), alturaMusicaAVL(B->dir));
    *P = B;
}

static void rotacaoEsqMusicaAVL(MusicaAVL **P) {
    MusicaAVL *A = *P;
    MusicaAVL *B = A->dir;
    A->dir = B->esq;
    B->esq = A;
    A->altura = 1 + maiorAVL(alturaMusicaAVL(A->esq), alturaMusicaAVL(A->dir));
    B->altura = 1 + maiorAVL(alturaMusicaAVL(B->esq), alturaMusicaAVL(B->dir));
    *P = B;
}

static void balanceamentoMusicaAVL(MusicaAVL **P) {
    int fb, fbE, fbD;
    MusicaAVL *filhoEsq, *filhoDir;

    fb = alturaMusicaAVL((*P)->esq) - alturaMusicaAVL((*P)->dir);
    if (fb == 2) {
        filhoEsq = (*P)->esq;
        fbE = alturaMusicaAVL(filhoEsq->esq) - alturaMusicaAVL(filhoEsq->dir);
        if (fbE < 0)
            rotacaoEsqMusicaAVL(&((*P)->esq));
        rotacaoDirMusicaAVL(P);
    }
    else if (fb == -2) {
        filhoDir = (*P)->dir;
        fbD = alturaMusicaAVL(filhoDir->esq) - alturaMusicaAVL(filhoDir->dir);
        if (fbD > 0)
            rotacaoDirMusicaAVL(&((*P)->dir));
        rotacaoEsqMusicaAVL(P);
    }
}

MusicaAVL* criaNoMusicaAVL(char titulo[], int minutos) {
    MusicaAVL *No = (MusicaAVL*) malloc(sizeof(MusicaAVL));
    if (No != NULL) {
        strcpy(No->titulo, titulo);
        No->minutos = minutos;
        No->altura = 0;
        No->esq = NULL;
        No->dir = NULL;
    }
    return No;
}


int insereMusicaAVL(MusicaAVL **R, MusicaAVL *No) {
    int cmp, inseriu;

    if (*R == NULL) {
        *R = No;
        return 1;
    }
    cmp = strcmp(No->titulo, (*R)->titulo);
    if (cmp == 0)
        return 0;
    if (cmp < 0)
        inseriu = insereMusicaAVL(&((*R)->esq), No);
    else
        inseriu = insereMusicaAVL(&((*R)->dir), No);

    balanceamentoMusicaAVL(R);
    atualizaAlturaMusicaAVL(R);
    return inseriu;
}

MusicaAVL* buscaMusicaAVL(MusicaAVL *H, char titulo[]) {
    int cmp;
    if (H == NULL)
        return NULL;
    cmp = strcmp(titulo, H->titulo);
    if (cmp == 0)
        return H;
    else if (cmp < 0)
        return buscaMusicaAVL(H->esq, titulo);
    return buscaMusicaAVL(H->dir, titulo);
}


int cadastraMusicaAVL(ArvArtistaAVL *raiz, char nomeArtista[], char tituloAlbum[], char titulo[], int minutos) {
    ArtistaAVL *art;
    AlbumAVL *alb;
    MusicaAVL *No;
    int resp;
    if (raiz == NULL)
        return -1;
    art = buscaArtistaAVL(*raiz, nomeArtista);
    if (art == NULL)
        return -1;
    alb = buscaAlbumAVL(art->albuns, tituloAlbum);
    if (alb == NULL)
        return -2;
    No = criaNoMusicaAVL(titulo, minutos);
    if (No == NULL)
        return 0;
    resp = insereMusicaAVL(&(alb->musicas), No);
    if (resp == 0)
        free(No);
    else
        alb->qtdMusicas++;
    return resp;
}

int mostraMusicaNosAlbunsAVL(AlbumAVL *A, ArtistaAVL *art, char titulo[]) {
    MusicaAVL *m;
    int total = 0;
    if (A == NULL)
        return 0;
    total += mostraMusicaNosAlbunsAVL(A->esq, art, titulo);
    m = buscaMusicaAVL(A->musicas, titulo);
    if (m != NULL) {
        printf("Musica : %s (%d min)\n", m->titulo, m->minutos);
        printf("Artista: %s (%s)\n", art->nome, art->tipo);
        printf("Album  : %s (%d)\n", A->titulo, A->ano);
        printf("Estilo : %s\n\n", art->estilo);
        total++;
    }
    total += mostraMusicaNosAlbunsAVL(A->dir, art, titulo);
    return total;
}

int mostraDadosMusicaAVL(ArtistaAVL *R, char titulo[]) {
    int total = 0;
    if (R == NULL)
        return 0;
    total += mostraDadosMusicaAVL(R->esq, titulo);
    total += mostraMusicaNosAlbunsAVL(R->albuns, R, titulo);
    total += mostraDadosMusicaAVL(R->dir, titulo);
    return total;
}

MusicaAVL* procuraMenorMusicaAVL(MusicaAVL *atual) {
    MusicaAVL *no1 = atual;
    MusicaAVL *no2 = atual->esq;
    while (no2 != NULL) {
        no1 = no2;
        no2 = no2->esq;
    }
    return no1;
}

// 1  removeu, 0 nao achou
int removeMusicaAVL(MusicaAVL **R, char titulo[]) {
    MusicaAVL *aux;
    int cmp, removeu;

    if (*R == NULL)
        return 0;

    cmp = strcmp(titulo, (*R)->titulo);
    if (cmp < 0)
        removeu = removeMusicaAVL(&((*R)->esq), titulo);
    else if (cmp > 0)
        removeu = removeMusicaAVL(&((*R)->dir), titulo);
    else {
        if ((*R)->esq == NULL || (*R)->dir == NULL) {
            aux = *R;
            if ((*R)->esq != NULL)
                *R = (*R)->esq;
            else
                *R = (*R)->dir;
            free(aux);
            return 1;
        }
        aux = procuraMenorMusicaAVL((*R)->dir);
        strcpy((*R)->titulo, aux->titulo);
        (*R)->minutos = aux->minutos;
        removeu = removeMusicaAVL(&((*R)->dir), (*R)->titulo);
    }

    balanceamentoMusicaAVL(R);
    atualizaAlturaMusicaAVL(R);
    return removeu;
}

#endif