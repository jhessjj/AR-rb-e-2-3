#ifndef ESTRUTURA_AVL_H
#define ESTRUTURA_AVL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct MusicaAVL {
    char titulo[100];
    int minutos;
    int altura;                   // novo
    struct MusicaAVL *esq, *dir;
} MusicaAVL;
typedef MusicaAVL* ArvMusicaAVL;

typedef struct AlbumAVL {
    char titulo[100];
    int ano;
    int qtdMusicas;
    MusicaAVL *musicas;
    int altura;                   
    struct AlbumAVL *esq, *dir;
} AlbumAVL;
typedef AlbumAVL* ArvAlbumAVL;

typedef struct ArtistaAVL {
    char nome[100];
    char tipo[30];
    char estilo[50];
    int numAlbuns;
    AlbumAVL *albuns;
    int altura;                  
    struct ArtistaAVL *esq, *dir;
} ArtistaAVL;
typedef ArtistaAVL* ArvArtistaAVL;

typedef struct MusicaPlaylistAVL {
    char artista[100];
    char album[100];
    char titulo[100];
    int altura;                  
    struct MusicaPlaylistAVL *esq, *dir;
} MusicaPlaylistAVL;
typedef MusicaPlaylistAVL* ArvMusicaPlaylistAVL;

typedef struct PlaylistAVL {
    char nome[100];
    MusicaPlaylistAVL *musicas;
    int altura;                  
    struct PlaylistAVL *esq, *dir;
} PlaylistAVL;
typedef PlaylistAVL* ArvPlaylistAVL;

#endif