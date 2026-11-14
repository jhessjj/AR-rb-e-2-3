#define RED   1
#define BLACK 0

typedef struct MusicaRB {
    char titulo[100];
    int minutos;
    int cor;                      // novo
    struct MusicaRB *esq, *dir;
} MusicaRB;

typedef struct AlbumRB {
    char titulo[100];
    int ano;
    int qtdMusicas;
    MusicaRB *musicas;
    int cor;                      
    struct AlbumRB *esq, *dir;
} AlbumRB;

typedef struct ArtistaRB {
    char nome[100];
    char tipo[30];
    char estilo[50];
    int numAlbuns;
    AlbumRB *albuns;
    int cor;                      
    struct ArtistaRB *esq, *dir;
} ArtistaRB;

typedef struct MusicaPlaylistRB {
    char artista[100];
    char album[100];
    char titulo[100];
    int cor;                      
    struct MusicaPlaylistRB *esq, *dir;
} MusicaPlaylistRB;

typedef struct PlaylistRB {
    char nome[100];
    MusicaPlaylistRB *musicas;
    int cor;                      
    struct PlaylistRB *esq, *dir;
} PlaylistRB;