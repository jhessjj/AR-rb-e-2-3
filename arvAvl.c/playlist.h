#ifndef PLAYLIST_H_AVL
#define PLAYLIST_H_AVL

#include "musica.h"

static int alturaMusicaPlaylistAVL(MusicaPlaylistAVL *No) {
    if (No == NULL)
        return -1;
    return No->altura;
}

static void atualizaAlturaMusicaPlaylistAVL(MusicaPlaylistAVL **R) {
    (*R)->altura = 1 + maiorAVL(alturaMusicaPlaylistAVL((*R)->esq), alturaMusicaPlaylistAVL((*R)->dir));
}

static void rotacaoDirMusicaPlaylistAVL(MusicaPlaylistAVL **P) {
    MusicaPlaylistAVL *A = *P;
    MusicaPlaylistAVL *B = A->esq;
    A->esq = B->dir;
    B->dir = A;
    A->altura = 1 + maiorAVL(alturaMusicaPlaylistAVL(A->esq), alturaMusicaPlaylistAVL(A->dir));
    B->altura = 1 + maiorAVL(alturaMusicaPlaylistAVL(B->esq), alturaMusicaPlaylistAVL(B->dir));
    *P = B;
}

static void rotacaoEsqMusicaPlaylistAVL(MusicaPlaylistAVL **P) {
    MusicaPlaylistAVL *A = *P;
    MusicaPlaylistAVL *B = A->dir;
    A->dir = B->esq;
    B->esq = A;
    A->altura = 1 + maiorAVL(alturaMusicaPlaylistAVL(A->esq), alturaMusicaPlaylistAVL(A->dir));
    B->altura = 1 + maiorAVL(alturaMusicaPlaylistAVL(B->esq), alturaMusicaPlaylistAVL(B->dir));
    *P = B;
}

static void balanceamentoMusicaPlaylistAVL(MusicaPlaylistAVL **P) {
    int fb, fbE, fbD;
    MusicaPlaylistAVL *filhoEsq, *filhoDir;

    fb = alturaMusicaPlaylistAVL((*P)->esq) - alturaMusicaPlaylistAVL((*P)->dir);
    if (fb == 2) {
        filhoEsq = (*P)->esq;
        fbE = alturaMusicaPlaylistAVL(filhoEsq->esq) - alturaMusicaPlaylistAVL(filhoEsq->dir);
        if (fbE < 0)
            rotacaoEsqMusicaPlaylistAVL(&((*P)->esq));
        rotacaoDirMusicaPlaylistAVL(P);
    }
    else if (fb == -2) {
        filhoDir = (*P)->dir;
        fbD = alturaMusicaPlaylistAVL(filhoDir->esq) - alturaMusicaPlaylistAVL(filhoDir->dir);
        if (fbD > 0)
            rotacaoDirMusicaPlaylistAVL(&((*P)->dir));
        rotacaoEsqMusicaPlaylistAVL(P);
    }
}

int comparaMusicaPlaylistAVL(MusicaPlaylistAVL *H, char artista[], char album[], char titulo[]) {
    int c = strcmp(titulo, H->titulo);
    if (c == 0)
        c = strcmp(artista, H->artista);
    if (c == 0)
        c = strcmp(album, H->album);
    return c;
}

MusicaPlaylistAVL* criaNoMusicaPlaylistAVL(char artista[], char album[], char titulo[]) {
    MusicaPlaylistAVL *No = (MusicaPlaylistAVL*) malloc(sizeof(MusicaPlaylistAVL));
    if (No != NULL) {
        strcpy(No->artista, artista);
        strcpy(No->album, album);
        strcpy(No->titulo, titulo);
        No->altura = 0;
        No->esq = NULL;
        No->dir = NULL;
    }
    return No;
}


int insereMusicaPlaylistAVL(MusicaPlaylistAVL **R, MusicaPlaylistAVL *No) {
    int cmp, inseriu;

    if (*R == NULL) {
        *R = No;
        return 1;
    }
    cmp = comparaMusicaPlaylistAVL(*R, No->artista, No->album, No->titulo);
    if (cmp == 0)
        return 0;
    if (cmp < 0)
        inseriu = insereMusicaPlaylistAVL(&((*R)->esq), No);
    else
        inseriu = insereMusicaPlaylistAVL(&((*R)->dir), No);

    balanceamentoMusicaPlaylistAVL(R);
    atualizaAlturaMusicaPlaylistAVL(R);
    return inseriu;
}

MusicaPlaylistAVL* buscaMusicaPlaylistAVL(MusicaPlaylistAVL *H, char artista[], char album[], char titulo[]) {
    int cmp;
    if (H == NULL)
        return NULL;
    cmp = comparaMusicaPlaylistAVL(H, artista, album, titulo);
    if (cmp == 0)
        return H;
    else if (cmp < 0)
        return buscaMusicaPlaylistAVL(H->esq, artista, album, titulo);
    return buscaMusicaPlaylistAVL(H->dir, artista, album, titulo);
}

int mostraMusicasPlaylistAVL(MusicaPlaylistAVL *H) {
    int total = 0;
    if (H == NULL)
        return 0;
    total += mostraMusicasPlaylistAVL(H->esq);
    printf("  %s | artista: %s | album: %s\n", H->titulo, H->artista, H->album);
    total++;
    total += mostraMusicasPlaylistAVL(H->dir);
    return total;
}

MusicaPlaylistAVL* procuraMenorMusicaPlaylistAVL(MusicaPlaylistAVL *atual) {
    MusicaPlaylistAVL *no1 = atual;
    MusicaPlaylistAVL *no2 = atual->esq;
    while (no2 != NULL) {
        no1 = no2;
        no2 = no2->esq;
    }
    return no1;
}


int removeMusicaPlaylistAVL(MusicaPlaylistAVL **R, char artista[], char album[], char titulo[]) {
    MusicaPlaylistAVL *aux;
    int cmp, removeu;

    if (*R == NULL)
        return 0;

    cmp = comparaMusicaPlaylistAVL(*R, artista, album, titulo);
    if (cmp < 0)
        removeu = removeMusicaPlaylistAVL(&((*R)->esq), artista, album, titulo);
    else if (cmp > 0)
        removeu = removeMusicaPlaylistAVL(&((*R)->dir), artista, album, titulo);
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
        aux = procuraMenorMusicaPlaylistAVL((*R)->dir);
        strcpy((*R)->artista, aux->artista);
        strcpy((*R)->album, aux->album);
        strcpy((*R)->titulo, aux->titulo);
        removeu = removeMusicaPlaylistAVL(&((*R)->dir), (*R)->artista, (*R)->album, (*R)->titulo);
    }

    balanceamentoMusicaPlaylistAVL(R);
    atualizaAlturaMusicaPlaylistAVL(R);
    return removeu;
}

static int alturaPlaylistAVL(PlaylistAVL *No) {
    if (No == NULL)
        return -1;
    return No->altura;
}

static void atualizaAlturaPlaylistAVL(PlaylistAVL **R) {
    (*R)->altura = 1 + maiorAVL(alturaPlaylistAVL((*R)->esq), alturaPlaylistAVL((*R)->dir));
}

static void rotacaoDirPlaylistAVL(PlaylistAVL **P) {
    PlaylistAVL *A = *P;
    PlaylistAVL *B = A->esq;
    A->esq = B->dir;
    B->dir = A;
    A->altura = 1 + maiorAVL(alturaPlaylistAVL(A->esq), alturaPlaylistAVL(A->dir));
    B->altura = 1 + maiorAVL(alturaPlaylistAVL(B->esq), alturaPlaylistAVL(B->dir));
    *P = B;
}

static void rotacaoEsqPlaylistAVL(PlaylistAVL **P) {
    PlaylistAVL *A = *P;
    PlaylistAVL *B = A->dir;
    A->dir = B->esq;
    B->esq = A;
    A->altura = 1 + maiorAVL(alturaPlaylistAVL(A->esq), alturaPlaylistAVL(A->dir));
    B->altura = 1 + maiorAVL(alturaPlaylistAVL(B->esq), alturaPlaylistAVL(B->dir));
    *P = B;
}

static void balanceamentoPlaylistAVL(PlaylistAVL **P) {
    int fb, fbE, fbD;
    PlaylistAVL *filhoEsq, *filhoDir;

    fb = alturaPlaylistAVL((*P)->esq) - alturaPlaylistAVL((*P)->dir);
    if (fb == 2) {
        filhoEsq = (*P)->esq;
        fbE = alturaPlaylistAVL(filhoEsq->esq) - alturaPlaylistAVL(filhoEsq->dir);
        if (fbE < 0)
            rotacaoEsqPlaylistAVL(&((*P)->esq));
        rotacaoDirPlaylistAVL(P);
    }
    else if (fb == -2) {
        filhoDir = (*P)->dir;
        fbD = alturaPlaylistAVL(filhoDir->esq) - alturaPlaylistAVL(filhoDir->dir);
        if (fbD > 0)
            rotacaoDirPlaylistAVL(&((*P)->dir));
        rotacaoEsqPlaylistAVL(P);
    }
}

ArvPlaylistAVL* criaArvPlaylistAVL() {
    ArvPlaylistAVL *raiz = (ArvPlaylistAVL*) malloc(sizeof(ArvPlaylistAVL));
    if (raiz != NULL)
        *raiz = NULL;
    return raiz;
}

PlaylistAVL* criaNoPlaylistAVL(char nome[]) {
    PlaylistAVL *No = (PlaylistAVL*) malloc(sizeof(PlaylistAVL));
    if (No != NULL) {
        strcpy(No->nome, nome);
        No->musicas = NULL;
        No->altura = 0;
        No->esq = NULL;
        No->dir = NULL;
    }
    return No;
}

// 1 = inseriu, 0 = nome repetido
int inserePlaylistAVL(PlaylistAVL **R, PlaylistAVL *No) {
    int cmp, inseriu;

    if (*R == NULL) {
        *R = No;
        return 1;
    }
    cmp = strcmp(No->nome, (*R)->nome);
    if (cmp == 0)
        return 0;
    if (cmp < 0)
        inseriu = inserePlaylistAVL(&((*R)->esq), No);
    else
        inseriu = inserePlaylistAVL(&((*R)->dir), No);

    balanceamentoPlaylistAVL(R);
    atualizaAlturaPlaylistAVL(R);
    return inseriu;
}

PlaylistAVL* buscaPlaylistAVL(PlaylistAVL *H, char nome[]) {
    int cmp;
    if (H == NULL)
        return NULL;
    cmp = strcmp(nome, H->nome);
    if (cmp == 0)
        return H;
    else if (cmp < 0)
        return buscaPlaylistAVL(H->esq, nome);
    return buscaPlaylistAVL(H->dir, nome);
}


int cadastraPlaylistAVL(ArvPlaylistAVL *raiz, char nome[]) {
    PlaylistAVL *No;
    int resp;
    if (raiz == NULL)
        return 0;
    No = criaNoPlaylistAVL(nome);
    if (No == NULL)
        return 0;
    resp = inserePlaylistAVL(raiz, No);
    if (resp == 0)
        free(No);
    return resp;
}


int cadastraMusicaPlaylistAVL(ArvArtistaAVL *artistas, ArvPlaylistAVL *playlists, char nomePlaylist[],
                              char artista[], char album[], char titulo[]) {
    PlaylistAVL *pl;
    ArtistaAVL *art;
    AlbumAVL *alb;
    MusicaPlaylistAVL *No;
    int resp;

    if (playlists == NULL || artistas == NULL)
        return -1;
    pl = buscaPlaylistAVL(*playlists, nomePlaylist);
    if (pl == NULL)
        return -1;
    art = buscaArtistaAVL(*artistas, artista);
    if (art == NULL)
        return -2;
    alb = buscaAlbumAVL(art->albuns, album);
    if (alb == NULL)
        return -3;
    if (buscaMusicaAVL(alb->musicas, titulo) == NULL)
        return -4;

    No = criaNoMusicaPlaylistAVL(artista, album, titulo);
    if (No == NULL)
        return 0;
    resp = insereMusicaPlaylistAVL(&(pl->musicas), No);
    if (resp == 0)
        free(No);
    return resp;
}


int mostraPlaylistAVL(ArvPlaylistAVL *raiz, char nome[]) {
    PlaylistAVL *pl;
    if (raiz == NULL)
        return -1;
    pl = buscaPlaylistAVL(*raiz, nome);
    if (pl == NULL)
        return -1;
    printf("Playlist: %s\n", pl->nome);
    return mostraMusicasPlaylistAVL(pl->musicas);
}


int removeMusicaDaPlaylistAVL(ArvPlaylistAVL *raiz, char nomePlaylist[], char artista[], char album[], char titulo[]) {
    PlaylistAVL *pl;
    if (raiz == NULL)
        return -1;
    pl = buscaPlaylistAVL(*raiz, nomePlaylist);
    if (pl == NULL)
        return -1;
    return removeMusicaPlaylistAVL(&(pl->musicas), artista, album, titulo);
}

int musicaEmAlgumaPlaylistAVL(PlaylistAVL *H, char artista[], char album[], char titulo[]) {
    if (H == NULL)
        return 0;
    if (buscaMusicaPlaylistAVL(H->musicas, artista, album, titulo) != NULL)
        return 1;
    if (musicaEmAlgumaPlaylistAVL(H->esq, artista, album, titulo))
        return 1;
    return musicaEmAlgumaPlaylistAVL(H->dir, artista, album, titulo);
}


int removeMusicaDoAlbumAVL(ArvArtistaAVL *artistas, ArvPlaylistAVL *playlists, char nomeArtista[],
                           char tituloAlbum[], char titulo[]) {
    ArtistaAVL *art;
    AlbumAVL *alb;

    if (artistas == NULL)
        return -1;
    art = buscaArtistaAVL(*artistas, nomeArtista);
    if (art == NULL)
        return -1;
    alb = buscaAlbumAVL(art->albuns, tituloAlbum);
    if (alb == NULL)
        return -2;
    if (buscaMusicaAVL(alb->musicas, titulo) == NULL)
        return 0;
    if (playlists != NULL && musicaEmAlgumaPlaylistAVL(*playlists, nomeArtista, tituloAlbum, titulo))
        return -3;

    removeMusicaAVL(&(alb->musicas), titulo);
    alb->qtdMusicas--;
    return 1;
}

void liberaMusicasAVL(MusicaAVL *H) {
    if (H != NULL) {
        liberaMusicasAVL(H->esq);
        liberaMusicasAVL(H->dir);
        free(H);
    }
}

void liberaAlbunsAVL(AlbumAVL *H) {
    if (H != NULL) {
        liberaAlbunsAVL(H->esq);
        liberaAlbunsAVL(H->dir);
        liberaMusicasAVL(H->musicas);
        free(H);
    }
}

void liberaArtistasAVL(ArtistaAVL *H) {
    if (H != NULL) {
        liberaArtistasAVL(H->esq);
        liberaArtistasAVL(H->dir);
        liberaAlbunsAVL(H->albuns);
        free(H);
    }
}

void liberaMusicasPlaylistAVL(MusicaPlaylistAVL *H) {
    if (H != NULL) {
        liberaMusicasPlaylistAVL(H->esq);
        liberaMusicasPlaylistAVL(H->dir);
        free(H);
    }
}

void liberaPlaylistsAVL(PlaylistAVL *H) {
    if (H != NULL) {
        liberaPlaylistsAVL(H->esq);
        liberaPlaylistsAVL(H->dir);
        liberaMusicasPlaylistAVL(H->musicas);
        free(H);
    }
}

void liberaTudoAVL(ArvArtistaAVL *artistas, ArvPlaylistAVL *playlists) {
    if (artistas != NULL) {
        liberaArtistasAVL(*artistas);
        free(artistas);
    }
    if (playlists != NULL) {
        liberaPlaylistsAVL(*playlists);
        free(playlists);
    }
}

#endif