typedef struct MusicaAVL {
    char titulo[100];
    int minutos;
    int altura;                   // novo
    struct MusicaAVL *esq, *dir;
} MusicaAVL;

typedef struct AlbumAVL {
    char titulo[100];
    int ano;
    int qtdMusicas;
    MusicaAVL *musicas;
    int altura;                   
    struct AlbumAVL *esq, *dir;
} AlbumAVL;

typedef struct ArtistaAVL {
    char nome[100];
    char tipo[30];
    char estilo[50];
    int numAlbuns;
    AlbumAVL *albuns;
    int altura;                  
    struct ArtistaAVL *esq, *dir;
} ArtistaAVL;

typedef struct MusicaPlaylistAVL {
    char artista[100];
    char album[100];
    char titulo[100];
    int altura;                  
    struct MusicaPlaylistAVL *esq, *dir;
} MusicaPlaylistAVL;

typedef struct PlaylistAVL {
    char nome[100];
    MusicaPlaylistAVL *musicas;
    int altura;                  
    struct PlaylistAVL *esq, *dir;
} PlaylistAVL;