#ifndef ALBUM_H_BB
#define ALBUM_H_BB

#include "artista.h"


Album* insereAlbum(Album *H, char titulo[], int ano, int *resp) {
    Album *novo;
    int cmp;

    if (H == NULL) {
        novo = (Album*) malloc(sizeof(Album));
        if (novo == NULL) {
            *resp = 0;
            return NULL;
        }
        strcpy(novo->titulo, titulo);
        novo->ano = ano;
        novo->qtdMusicas = 0;
        novo->musicas = NULL;
        novo->esq = NULL;
        novo->dir = NULL;
        *resp = 1;
        return novo;
    }

    cmp = strcmp(titulo, H->titulo);
    if (cmp == 0)
        *resp = 0;                              
    else if (cmp < 0)
        H->esq = insereAlbum(H->esq, titulo, ano, resp);
    else
        H->dir = insereAlbum(H->dir, titulo, ano, resp);

    return H;
}

Album* buscaAlbum(Album *H, char titulo[]) {
    int cmp;
    if (H == NULL)
        return NULL;
    cmp = strcmp(titulo, H->titulo);
    if (cmp == 0)
        return H;
    else if (cmp < 0)
        return buscaAlbum(H->esq, titulo);
    return buscaAlbum(H->dir, titulo);
}


int cadastraAlbum(ArvArtista *raiz, char nomeArtista[], char titulo[], int ano) {
    Artista *art;
    int resp;
    if (raiz == NULL)
        return -1;
    art = buscaArtista(*raiz, nomeArtista);
    if (art == NULL)
        return -1;
    art->albuns = insereAlbum(art->albuns, titulo, ano, &resp);
    if (resp == 1)
        art->numAlbuns++;
    return resp;
}


int mostraAlbunsAno(Album *H, int ano) {
    int total = 0;
    if (H == NULL)
        return 0;
    total += mostraAlbunsAno(H->esq, ano);
    if (H->ano == ano) {
        printf("%s | %d | %d musica(s)\n", H->titulo, H->ano, H->qtdMusicas);
        total++;
    }
    total += mostraAlbunsAno(H->dir, ano);
    return total;
}


int mostraAlbunsDoArtista(ArvArtista *raiz, char nomeArtista[], int ano) {
    Artista *art;
    if (raiz == NULL)
        return -1;
    art = buscaArtista(*raiz, nomeArtista);
    if (art == NULL)
        return -1;
    return mostraAlbunsAno(art->albuns, ano);
}

#endif