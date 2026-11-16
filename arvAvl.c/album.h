#ifndef ALBUM_H_AVL
#define ALBUM_H_AVL

#include "artista.h"

static int alturaAlbumAVL(AlbumAVL *No) {
    if (No == NULL)
        return -1;
    return No->altura;
}

static void atualizaAlturaAlbumAVL(AlbumAVL **R) {
    (*R)->altura = 1 + maiorAVL(alturaAlbumAVL((*R)->esq), alturaAlbumAVL((*R)->dir));
}

static void rotacaoDirAlbumAVL(AlbumAVL **P) {
    AlbumAVL *A = *P;
    AlbumAVL *B = A->esq;
    A->esq = B->dir;
    B->dir = A;
    A->altura = 1 + maiorAVL(alturaAlbumAVL(A->esq), alturaAlbumAVL(A->dir));
    B->altura = 1 + maiorAVL(alturaAlbumAVL(B->esq), alturaAlbumAVL(B->dir));
    *P = B;
}

static void rotacaoEsqAlbumAVL(AlbumAVL **P) {
    AlbumAVL *A = *P;
    AlbumAVL *B = A->dir;
    A->dir = B->esq;
    B->esq = A;
    A->altura = 1 + maiorAVL(alturaAlbumAVL(A->esq), alturaAlbumAVL(A->dir));
    B->altura = 1 + maiorAVL(alturaAlbumAVL(B->esq), alturaAlbumAVL(B->dir));
    *P = B;
}

static void balanceamentoAlbumAVL(AlbumAVL **P) {
    int fb, fbE, fbD;
    AlbumAVL *filhoEsq, *filhoDir;

    fb = alturaAlbumAVL((*P)->esq) - alturaAlbumAVL((*P)->dir);
    if (fb == 2) {
        filhoEsq = (*P)->esq;
        fbE = alturaAlbumAVL(filhoEsq->esq) - alturaAlbumAVL(filhoEsq->dir);
        if (fbE < 0)
            rotacaoEsqAlbumAVL(&((*P)->esq));
        rotacaoDirAlbumAVL(P);
    }
    else if (fb == -2) {
        filhoDir = (*P)->dir;
        fbD = alturaAlbumAVL(filhoDir->esq) - alturaAlbumAVL(filhoDir->dir);
        if (fbD > 0)
            rotacaoDirAlbumAVL(&((*P)->dir));
        rotacaoEsqAlbumAVL(P);
    }
}

AlbumAVL* criaNoAlbumAVL(char titulo[], int ano) {
    AlbumAVL *No = (AlbumAVL*) malloc(sizeof(AlbumAVL));
    if (No != NULL) {
        strcpy(No->titulo, titulo);
        No->ano = ano;
        No->qtdMusicas = 0;
        No->musicas = NULL;
        No->altura = 0;
        No->esq = NULL;
        No->dir = NULL;
    }
    return No;
}


int insereAlbumAVL(AlbumAVL **R, AlbumAVL *No) {
    int cmp, inseriu;

    if (*R == NULL) {
        *R = No;
        return 1;
    }
    cmp = strcmp(No->titulo, (*R)->titulo);
    if (cmp == 0)
        return 0;
    if (cmp < 0)
        inseriu = insereAlbumAVL(&((*R)->esq), No);
    else
        inseriu = insereAlbumAVL(&((*R)->dir), No);

    balanceamentoAlbumAVL(R);
    atualizaAlturaAlbumAVL(R);
    return inseriu;
}

AlbumAVL* buscaAlbumAVL(AlbumAVL *H, char titulo[]) {
    int cmp;
    if (H == NULL)
        return NULL;
    cmp = strcmp(titulo, H->titulo);
    if (cmp == 0)
        return H;
    else if (cmp < 0)
        return buscaAlbumAVL(H->esq, titulo);
    return buscaAlbumAVL(H->dir, titulo);
}


int cadastraAlbumAVL(ArvArtistaAVL *raiz, char nomeArtista[], char titulo[], int ano) {
    ArtistaAVL *art;
    AlbumAVL *No;
    int resp;
    if (raiz == NULL)
        return -1;
    art = buscaArtistaAVL(*raiz, nomeArtista);
    if (art == NULL)
        return -1;
    No = criaNoAlbumAVL(titulo, ano);
    if (No == NULL)
        return 0;
    resp = insereAlbumAVL(&(art->albuns), No);
    if (resp == 0)
        free(No);
    else
        art->numAlbuns++;
    return resp;
}

int mostraAlbunsAnoAVL(AlbumAVL *H, int ano) {
    int total = 0;
    if (H == NULL)
        return 0;
    total += mostraAlbunsAnoAVL(H->esq, ano);
    if (H->ano == ano) {
        printf("%s | %d | %d musica(s)\n", H->titulo, H->ano, H->qtdMusicas);
        total++;
    }
    total += mostraAlbunsAnoAVL(H->dir, ano);
    return total;
}


int mostraAlbunsDoArtistaAVL(ArvArtistaAVL *raiz, char nomeArtista[], int ano) {
    ArtistaAVL *art;
    if (raiz == NULL)
        return -1;
    art = buscaArtistaAVL(*raiz, nomeArtista);
    if (art == NULL)
        return -1;
    return mostraAlbunsAnoAVL(art->albuns, ano);
}

#endif