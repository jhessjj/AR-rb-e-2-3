#ifndef PLAYLIST_H_BB
#define PLAYLIST_H_BB

#include "musica.h"



int comparaMusicaPlaylist(MusicaPlaylist *H, char artista[], char album[], char titulo[]) {
    int c = strcmp(titulo, H->titulo);
    if (c == 0)
        c = strcmp(artista, H->artista);
    if (c == 0)
        c = strcmp(album, H->album);
    return c;
}

MusicaPlaylist* insereMusicaPlaylist(MusicaPlaylist *H, char artista[], char album[], char titulo[], int *resp) {
    MusicaPlaylist *novo;
    int cmp;

    if (H == NULL) {
        novo = (MusicaPlaylist*) malloc(sizeof(MusicaPlaylist));
        if (novo == NULL) {
            *resp = 0;
            return NULL;
        }
        strcpy(novo->artista, artista);
        strcpy(novo->album, album);
        strcpy(novo->titulo, titulo);
        novo->esq = NULL;
        novo->dir = NULL;
        *resp = 1;
        return novo;
    }

    cmp = comparaMusicaPlaylist(H, artista, album, titulo);
    if (cmp == 0)
        *resp = 0;                                
    else if (cmp < 0)
        H->esq = insereMusicaPlaylist(H->esq, artista, album, titulo, resp);
    else
        H->dir = insereMusicaPlaylist(H->dir, artista, album, titulo, resp);

    return H;
}

MusicaPlaylist* buscaMusicaPlaylist(MusicaPlaylist *H, char artista[], char album[], char titulo[]) {
    int cmp;
    if (H == NULL)
        return NULL;
    cmp = comparaMusicaPlaylist(H, artista, album, titulo);
    if (cmp == 0)
        return H;
    else if (cmp < 0)
        return buscaMusicaPlaylist(H->esq, artista, album, titulo);
    return buscaMusicaPlaylist(H->dir, artista, album, titulo);
}


int mostraMusicasPlaylist(MusicaPlaylist *H) {
    int total = 0;
    if (H == NULL)
        return 0;
    total += mostraMusicasPlaylist(H->esq);
    printf("  %s | artista: %s | album: %s\n", H->titulo, H->artista, H->album);
    total++;
    total += mostraMusicasPlaylist(H->dir);
    return total;
}



MusicaPlaylist* procuraMenorMP(MusicaPlaylist *atual) {
    MusicaPlaylist *no1 = atual;
    MusicaPlaylist *no2 = atual->esq;
    while (no2 != NULL) {
        no1 = no2;
        no2 = no2->esq;
    }
    return no1;
}

MusicaPlaylist* removeMenorMP(MusicaPlaylist *H) {
    MusicaPlaylist *aux;
    if (H->esq == NULL) {
        aux = H->dir;
        free(H);
        return aux;
    }
    H->esq = removeMenorMP(H->esq);
    return H;
}


MusicaPlaylist* removeMP(MusicaPlaylist *H, char artista[], char album[], char titulo[], int *resp) {
    MusicaPlaylist *aux;
    int cmp;

    if (H == NULL) {
        *resp = 0;
        return NULL;
    }

    cmp = comparaMusicaPlaylist(H, artista, album, titulo);
    if (cmp < 0)
        H->esq = removeMP(H->esq, artista, album, titulo, resp);
    else if (cmp > 0)
        H->dir = removeMP(H->dir, artista, album, titulo, resp);
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
        aux = procuraMenorMP(H->dir);
        strcpy(H->artista, aux->artista);
        strcpy(H->album, aux->album);
        strcpy(H->titulo, aux->titulo);
        H->dir = removeMenorMP(H->dir);
    }
    return H;
}


ArvPlaylist* criaArvPlaylist() {
    ArvPlaylist *raiz = (ArvPlaylist*) malloc(sizeof(ArvPlaylist));
    if (raiz != NULL)
        *raiz = NULL;
    return raiz;
}

Playlist* inserePlaylist(Playlist *H, char nome[], int *resp) {
    Playlist *novo;
    int cmp;

    if (H == NULL) {
        novo = (Playlist*) malloc(sizeof(Playlist));
        if (novo == NULL) {
            *resp = 0;
            return NULL;
        }
        strcpy(novo->nome, nome);
        novo->musicas = NULL;
        novo->esq = NULL;
        novo->dir = NULL;
        *resp = 1;
        return novo;
    }

    cmp = strcmp(nome, H->nome);
    if (cmp == 0)
        *resp = 0;                                
    else if (cmp < 0)
        H->esq = inserePlaylist(H->esq, nome, resp);
    else
        H->dir = inserePlaylist(H->dir, nome, resp);

    return H;
}

Playlist* buscaPlaylist(Playlist *H, char nome[]) {
    int cmp;
    if (H == NULL)
        return NULL;
    cmp = strcmp(nome, H->nome);
    if (cmp == 0)
        return H;
    else if (cmp < 0)
        return buscaPlaylist(H->esq, nome);
    return buscaPlaylist(H->dir, nome);
}


int cadastraPlaylist(ArvPlaylist *raiz, char nome[]) {
    int resp;
    if (raiz == NULL)
        return 0;
    *raiz = inserePlaylist(*raiz, nome, &resp);
    return resp;
}


int cadastraMusicaPlaylist(ArvArtista *artistas, ArvPlaylist *playlists, char nomePlaylist[],
                           char artista[], char album[], char titulo[]) {
    Playlist *pl;
    Artista *art;
    Album *alb;
    int resp;

    if (playlists == NULL || artistas == NULL)
        return -1;
    pl = buscaPlaylist(*playlists, nomePlaylist);
    if (pl == NULL)
        return -1;
    art = buscaArtista(*artistas, artista);
    if (art == NULL)
        return -2;
    alb = buscaAlbum(art->albuns, album);
    if (alb == NULL)
        return -3;
    if (buscaMusica(alb->musicas, titulo) == NULL)
        return -4;

    pl->musicas = insereMusicaPlaylist(pl->musicas, artista, album, titulo, &resp);
    return resp;
}


int mostraPlaylist(ArvPlaylist *raiz, char nome[]) {
    Playlist *pl;
    if (raiz == NULL)
        return -1;
    pl = buscaPlaylist(*raiz, nome);
    if (pl == NULL)
        return -1;
    printf("Playlist: %s\n", pl->nome);
    return mostraMusicasPlaylist(pl->musicas);
}


int removeMusicaDaPlaylist(ArvPlaylist *raiz, char nomePlaylist[], char artista[], char album[], char titulo[]) {
    Playlist *pl;
    int resp;
    if (raiz == NULL)
        return -1;
    pl = buscaPlaylist(*raiz, nomePlaylist);
    if (pl == NULL)
        return -1;
    pl->musicas = removeMP(pl->musicas, artista, album, titulo, &resp);
    return resp;
}


int musicaEmAlgumaPlaylist(Playlist *H, char artista[], char album[], char titulo[]) {
    if (H == NULL)
        return 0;
    if (buscaMusicaPlaylist(H->musicas, artista, album, titulo) != NULL)
        return 1;
    if (musicaEmAlgumaPlaylist(H->esq, artista, album, titulo))
        return 1;
    return musicaEmAlgumaPlaylist(H->dir, artista, album, titulo);
}


int removeMusicaDoAlbum(ArvArtista *artistas, ArvPlaylist *playlists, char nomeArtista[],
                        char tituloAlbum[], char titulo[]) {
    Artista *art;
    Album *alb;
    int resp;

    if (artistas == NULL)
        return -1;
    art = buscaArtista(*artistas, nomeArtista);
    if (art == NULL)
        return -1;
    alb = buscaAlbum(art->albuns, tituloAlbum);
    if (alb == NULL)
        return -2;
    if (buscaMusica(alb->musicas, titulo) == NULL)
        return 0;
    if (playlists != NULL && musicaEmAlgumaPlaylist(*playlists, nomeArtista, tituloAlbum, titulo))
        return -3;

    alb->musicas = removeMusica(alb->musicas, titulo, &resp);
    alb->qtdMusicas--;
    return 1;
}



void liberaMusicas(Musica *H) {
    if (H != NULL) {
        liberaMusicas(H->esq);
        liberaMusicas(H->dir);
        free(H);
    }
}

void liberaAlbuns(Album *H) {
    if (H != NULL) {
        liberaAlbuns(H->esq);
        liberaAlbuns(H->dir);
        liberaMusicas(H->musicas);
        free(H);
    }
}

void liberaArtistas(Artista *H) {
    if (H != NULL) {
        liberaArtistas(H->esq);
        liberaArtistas(H->dir);
        liberaAlbuns(H->albuns);
        free(H);
    }
}

void liberaMusicasPlaylist(MusicaPlaylist *H) {
    if (H != NULL) {
        liberaMusicasPlaylist(H->esq);
        liberaMusicasPlaylist(H->dir);
        free(H);
    }
}

void liberaPlaylists(Playlist *H) {
    if (H != NULL) {
        liberaPlaylists(H->esq);
        liberaPlaylists(H->dir);
        liberaMusicasPlaylist(H->musicas);
        free(H);
    }
}

void liberaTudo(ArvArtista *artistas, ArvPlaylist *playlists) {
    if (artistas != NULL) {
        liberaArtistas(*artistas);
        free(artistas);
    }
    if (playlists != NULL) {
        liberaPlaylists(*playlists);
        free(playlists);
    }
}

#endif