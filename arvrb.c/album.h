#ifndef ALBUM_H_RB
#define ALBUM_H_RB

#include "artista.h"


static int corAlbumRB(AlbumRB *H) {
    if (H == NULL)
        return BLACK;
    return H->cor;
}

// troca cor de h e os filhos dele
static void trocaCorAlbumRB(AlbumRB *H) {
    H->cor = !H->cor;
    if (H->esq != NULL)
        H->esq->cor = !H->esq->cor;
    if (H->dir != NULL)
        H->dir->cor = !H->dir->cor;
}

static AlbumRB* rotacionaDireitaAlbumRB(AlbumRB *A) {
    AlbumRB *B = A->esq;
    A->esq = B->dir;
    B->dir = A;
    B->cor = A->cor;
    A->cor = RED;
    return B;
}

static AlbumRB* rotacionaEsquerdaAlbumRB(AlbumRB *A) {
    AlbumRB *B = A->dir;
    A->dir = B->esq;
    B->esq = A;
    B->cor = A->cor;
    A->cor = RED;
    return B;
}

AlbumRB* insereAlbumRB(AlbumRB *H, char titulo[], int ano, int *resp) {
    AlbumRB *novo;
    int cmp;

    if (H == NULL) {
        novo = (AlbumRB*) malloc(sizeof(AlbumRB));
        if (novo == NULL) {
            *resp = 0;
            return NULL;
        }
        strcpy(novo->titulo, titulo);
        novo->ano = ano;
        novo->qtdMusicas = 0;
        novo->musicas = NULL;
        novo->cor = RED;
        novo->esq = NULL;
        novo->dir = NULL;
        *resp = 1;
        return novo;
    }

    cmp = strcmp(titulo, H->titulo);
    if (cmp == 0)
        *resp = 0;  // em todas coloquei para return 0 quando repetir                        
    else if (cmp < 0)
        H->esq = insereAlbumRB(H->esq, titulo, ano, resp);
    else
        H->dir = insereAlbumRB(H->dir, titulo, ano, resp);

    /* as 3 regras da insercao do PDF (linhas 26 a 33) */
    if (corAlbumRB(H->dir) == RED && corAlbumRB(H->esq) == BLACK)
        H = rotacionaEsquerdaAlbumRB(H);

    if (corAlbumRB(H->esq) == RED && corAlbumRB(H->esq->esq) == RED)
        H = rotacionaDireitaAlbumRB(H);

    if (corAlbumRB(H->esq) == RED && corAlbumRB(H->dir) == RED)
        trocaCorAlbumRB(H);

    return H;
}


int insereArvAlbumRB(ArvAlbumRB *raiz, char titulo[], int ano) {
    int resp;
    if (raiz == NULL)
        return 0;
    *raiz = insereAlbumRB(*raiz, titulo, ano, &resp);
    if (*raiz != NULL)
        (*raiz)->cor = BLACK;
    return resp;
}

AlbumRB* buscaAlbumRB(AlbumRB *H, char titulo[]) {
    int cmp;
    if (H == NULL)
        return NULL;
    cmp = strcmp(titulo, H->titulo);
    if (cmp == 0)
        return H;
    else if (cmp < 0)
        return buscaAlbumRB(H->esq, titulo);
    return buscaAlbumRB(H->dir, titulo);
}


int cadastraAlbumRB(ArvArtistaRB *raiz, char nomeArtista[], char titulo[], int ano) {
    ArtistaRB *art;
    int resp;
    if (raiz == NULL)
        return -1;
    art = buscaArtistaRB(*raiz, nomeArtista);
    if (art == NULL)
        return -1;
    resp = insereArvAlbumRB(&(art->albuns), titulo, ano);
    if (resp == 1)
        art->numAlbuns++;
    return resp;
}


int mostraAlbunsAnoRB(AlbumRB *H, int ano) {
    int total = 0;
    if (H == NULL)
        return 0;
    total += mostraAlbunsAnoRB(H->esq, ano);
    if (H->ano == ano) {
        printf("%s | %d | %d musica(s)\n", H->titulo, H->ano, H->qtdMusicas);
        total++;
    }
    total += mostraAlbunsAnoRB(H->dir, ano);
    return total;
}


int mostraAlbunsDoArtistaRB(ArvArtistaRB *raiz, char nomeArtista[], int ano) {
    ArtistaRB *art;
    if (raiz == NULL)
        return -1;
    art = buscaArtistaRB(*raiz, nomeArtista);
    if (art == NULL)
        return -1;
    return mostraAlbunsAnoRB(art->albuns, ano);
}

#endif