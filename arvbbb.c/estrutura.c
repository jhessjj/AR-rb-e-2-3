#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Musica {
    char titulo[100];
    int minutos;
    struct Musica *esq, *dir;
} Musica;


typedef struct Album {
    char titulo[100];
    int ano;
    int qtdMusicas;
    Musica *musicas;          
    struct Album *esq, *dir;
} Album;


typedef struct Artista {
    char nome[100];
    char tipo[30];    /* solo, dupla, banda*/ 
    char estilo[50];
    int numAlbuns;
    Album *albuns;   
    struct Artista *esq, *dir;
} Artista;


typedef struct MusicaPlaylist {
    char artista[100];
    char album[100];
    char titulo[100];
    struct MusicaPlaylist *esq, *dir;
} MusicaPlaylist;

typedef struct Playlist {
    char nome[100];
    MusicaPlaylist *musicas;  
    struct Playlist *esq, *dir;
} Playlist;