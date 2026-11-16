#ifndef MUSICA_H_RB
#define MUSICA_H_RB

#include "album.h"


static int corMusicaRB(MusicaRB *H) {
    if (H == NULL)
        return BLACK;
    return H->cor;
}


static void trocaCorMusicaRB(MusicaRB *H) {
    H->cor = !H->cor;
    if (H->esq != NULL)
        H->esq->cor = !H->esq->cor;
    if (H->dir != NULL)
        H->dir->cor = !H->dir->cor;
}

static MusicaRB* rotacionaDireitaMusicaRB(MusicaRB *A) {
    MusicaRB *B = A->esq;
    A->esq = B->dir;
    B->dir = A;
    B->cor = A->cor;
    A->cor = RED;
    return B;
}

static MusicaRB* rotacionaEsquerdaMusicaRB(MusicaRB *A) {
    MusicaRB *B = A->dir;
    A->dir = B->esq;
    B->esq = A;
    B->cor = A->cor;
    A->cor = RED;
    return B;
}


static MusicaRB* move2EsqREDMusicaRB(MusicaRB *H) {
    trocaCorMusicaRB(H);
    if (corMusicaRB(H->dir->esq) == RED) {
        H->dir = rotacionaDireitaMusicaRB(H->dir);
        H = rotacionaEsquerdaMusicaRB(H);
        trocaCorMusicaRB(H);
    }
    return H;
}


static MusicaRB* move2DirREDMusicaRB(MusicaRB *H) {
    trocaCorMusicaRB(H);
    if (corMusicaRB(H->esq->esq) == RED) {
        H = rotacionaDireitaMusicaRB(H);
        trocaCorMusicaRB(H);
    }
    return H;
}

static MusicaRB* balancearMusicaRB(MusicaRB *H) {
    
    if (corMusicaRB(H->dir) == RED)
        H = rotacionaEsquerdaMusicaRB(H);

    
    if (corMusicaRB(H->esq) == RED && corMusicaRB(H->esq->esq) == RED)
        H = rotacionaDireitaMusicaRB(H);

    
    if (corMusicaRB(H->esq) == RED && corMusicaRB(H->dir) == RED)
        trocaCorMusicaRB(H);

    return H;
}


static MusicaRB* procuraMenorMusicaRB(MusicaRB *atual) {
    MusicaRB *no1 = atual;
    MusicaRB *no2 = atual->esq;
    while (no2 != NULL) {
        no1 = no2;
        no2 = no2->esq;
    }
    return no1;
}


static MusicaRB* removerMenorMusicaRB(MusicaRB *H) {
    if (H->esq == NULL) {
        free(H);
        return NULL;
    }
    if (corMusicaRB(H->esq) == BLACK && corMusicaRB(H->esq->esq) == BLACK)
        H = move2EsqREDMusicaRB(H);

    H->esq = removerMenorMusicaRB(H->esq);
    return balancearMusicaRB(H);
}

MusicaRB* insereMusicaRB(MusicaRB *H, char titulo[], int minutos, int *resp) {
    MusicaRB *novo;
    int cmp;

    if (H == NULL) {
        novo = (MusicaRB*) malloc(sizeof(MusicaRB));
        if (novo == NULL) {
            *resp = 0;
            return NULL;
        }
        strcpy(novo->titulo, titulo);
        novo->minutos = minutos;
        novo->cor = RED;
        novo->esq = NULL;
        novo->dir = NULL;
        *resp = 1;
        return novo;
    }

    cmp = strcmp(titulo, H->titulo);
    if (cmp == 0)
        *resp = 0;                                
    else if (cmp < 0)
        H->esq = insereMusicaRB(H->esq, titulo, minutos, resp);
    else
        H->dir = insereMusicaRB(H->dir, titulo, minutos, resp);

    /* as 3 regras da insercao do PDF (linhas 26 a 33) */
    if (corMusicaRB(H->dir) == RED && corMusicaRB(H->esq) == BLACK)
        H = rotacionaEsquerdaMusicaRB(H);

    if (corMusicaRB(H->esq) == RED && corMusicaRB(H->esq->esq) == RED)
        H = rotacionaDireitaMusicaRB(H);

    if (corMusicaRB(H->esq) == RED && corMusicaRB(H->dir) == RED)
        trocaCorMusicaRB(H);

    return H;
}


int insereArvMusicaRB(ArvMusicaRB *raiz, char titulo[], int minutos) {
    int resp;
    if (raiz == NULL)
        return 0;
    *raiz = insereMusicaRB(*raiz, titulo, minutos, &resp);
    if (*raiz != NULL)
        (*raiz)->cor = BLACK;
    return resp;
}

MusicaRB* buscaMusicaRB(MusicaRB *H, char titulo[]) {
    int cmp;
    if (H == NULL)
        return NULL;
    cmp = strcmp(titulo, H->titulo);
    if (cmp == 0)
        return H;
    else if (cmp < 0)
        return buscaMusicaRB(H->esq, titulo);
    return buscaMusicaRB(H->dir, titulo);
}


int cadastraMusicaRB(ArvArtistaRB *raiz, char nomeArtista[], char tituloAlbum[], char titulo[], int minutos) {
    ArtistaRB *art;
    AlbumRB *alb;
    int resp;
    if (raiz == NULL)
        return -1;
    art = buscaArtistaRB(*raiz, nomeArtista);
    if (art == NULL)
        return -1;
    alb = buscaAlbumRB(art->albuns, tituloAlbum);
    if (alb == NULL)
        return -2;
    resp = insereArvMusicaRB(&(alb->musicas), titulo, minutos);
    if (resp == 1)
        alb->qtdMusicas++;
    return resp;
}


int mostraMusicaNosAlbunsRB(AlbumRB *A, ArtistaRB *art, char titulo[]) {
    MusicaRB *m;
    int total = 0;
    if (A == NULL)
        return 0;
    total += mostraMusicaNosAlbunsRB(A->esq, art, titulo);
    m = buscaMusicaRB(A->musicas, titulo);
    if (m != NULL) {
        printf("Musica : %s (%d min)\n", m->titulo, m->minutos);
        printf("Artista: %s (%s)\n", art->nome, art->tipo);
        printf("Album  : %s (%d)\n", A->titulo, A->ano);
        printf("Estilo : %s\n\n", art->estilo);
        total++;
    }
    total += mostraMusicaNosAlbunsRB(A->dir, art, titulo);
    return total;
}


int mostraDadosMusicaRB(ArtistaRB *R, char titulo[]) {
    int total = 0;
    if (R == NULL)
        return 0;
    total += mostraDadosMusicaRB(R->esq, titulo);
    total += mostraMusicaNosAlbunsRB(R->albuns, R, titulo);
    total += mostraDadosMusicaRB(R->dir, titulo);
    return total;
}

//talvez o remove nao esteja confiavel
MusicaRB* removeMusicaRB(MusicaRB *H, char titulo[]) {
    MusicaRB *x;

    if (strcmp(titulo, H->titulo) < 0) {
        if (corMusicaRB(H->esq) == BLACK && corMusicaRB(H->esq->esq) == BLACK)
            H = move2EsqREDMusicaRB(H);

        H->esq = removeMusicaRB(H->esq, titulo);
    } else {
        if (corMusicaRB(H->esq) == RED)
            H = rotacionaDireitaMusicaRB(H);

        if (strcmp(titulo, H->titulo) == 0 && H->dir == NULL) {
            free(H);
            return NULL;
        }

        if (corMusicaRB(H->dir) == BLACK && corMusicaRB(H->dir->esq) == BLACK)
            H = move2DirREDMusicaRB(H);

        if (strcmp(titulo, H->titulo) == 0) {
            x = procuraMenorMusicaRB(H->dir);
            strcpy(H->titulo, x->titulo);
            H->minutos = x->minutos;
            H->dir = removerMenorMusicaRB(H->dir);
        } else
            H->dir = removeMusicaRB(H->dir, titulo);
    }
    return balancearMusicaRB(H);
}


int removeArvMusicaRB(ArvMusicaRB *raiz, char titulo[]) {
    if (raiz == NULL || buscaMusicaRB(*raiz, titulo) == NULL)
        return 0;
    *raiz = removeMusicaRB(*raiz, titulo);
    if (*raiz != NULL)
        (*raiz)->cor = BLACK;
    return 1;
}

#endif