#ifndef PLAYLIST_H_RB
#define PLAYLIST_H_RB

#include "musica.h"


static int corMusicaPlaylistRB(MusicaPlaylistRB *H) {
    if (H == NULL)
        return BLACK;
    return H->cor;
}


static void trocaCorMusicaPlaylistRB(MusicaPlaylistRB *H) {
    H->cor = !H->cor;
    if (H->esq != NULL)
        H->esq->cor = !H->esq->cor;
    if (H->dir != NULL)
        H->dir->cor = !H->dir->cor;
}

static MusicaPlaylistRB* rotacionaDireitaMusicaPlaylistRB(MusicaPlaylistRB *A) {
    MusicaPlaylistRB *B = A->esq;
    A->esq = B->dir;
    B->dir = A;
    B->cor = A->cor;
    A->cor = RED;
    return B;
}

static MusicaPlaylistRB* rotacionaEsquerdaMusicaPlaylistRB(MusicaPlaylistRB *A) {
    MusicaPlaylistRB *B = A->dir;
    A->dir = B->esq;
    B->esq = A;
    B->cor = A->cor;
    A->cor = RED;
    return B;
}


static MusicaPlaylistRB* move2EsqREDMusicaPlaylistRB(MusicaPlaylistRB *H) {
    trocaCorMusicaPlaylistRB(H);
    if (corMusicaPlaylistRB(H->dir->esq) == RED) {
        H->dir = rotacionaDireitaMusicaPlaylistRB(H->dir);
        H = rotacionaEsquerdaMusicaPlaylistRB(H);
        trocaCorMusicaPlaylistRB(H);
    }
    return H;
}


static MusicaPlaylistRB* move2DirREDMusicaPlaylistRB(MusicaPlaylistRB *H) {
    trocaCorMusicaPlaylistRB(H);
    if (corMusicaPlaylistRB(H->esq->esq) == RED) {
        H = rotacionaDireitaMusicaPlaylistRB(H);
        trocaCorMusicaPlaylistRB(H);
    }
    return H;
}


static MusicaPlaylistRB* balancearMusicaPlaylistRB(MusicaPlaylistRB *H) {
   
    if (corMusicaPlaylistRB(H->dir) == RED)
        H = rotacionaEsquerdaMusicaPlaylistRB(H);

   
    if (corMusicaPlaylistRB(H->esq) == RED && corMusicaPlaylistRB(H->esq->esq) == RED)
        H = rotacionaDireitaMusicaPlaylistRB(H);

    if (corMusicaPlaylistRB(H->esq) == RED && corMusicaPlaylistRB(H->dir) == RED)
        trocaCorMusicaPlaylistRB(H);

    return H;
}


static MusicaPlaylistRB* procuraMenorMusicaPlaylistRB(MusicaPlaylistRB *atual) {
    MusicaPlaylistRB *no1 = atual;
    MusicaPlaylistRB *no2 = atual->esq;
    while (no2 != NULL) {
        no1 = no2;
        no2 = no2->esq;
    }
    return no1;
}


static MusicaPlaylistRB* removerMenorMusicaPlaylistRB(MusicaPlaylistRB *H) {
    if (H->esq == NULL) {
        free(H);
        return NULL;
    }
    if (corMusicaPlaylistRB(H->esq) == BLACK && corMusicaPlaylistRB(H->esq->esq) == BLACK)
        H = move2EsqREDMusicaPlaylistRB(H);

    H->esq = removerMenorMusicaPlaylistRB(H->esq);
    return balancearMusicaPlaylistRB(H);
}

int comparaMusicaPlaylistRB(MusicaPlaylistRB *H, char artista[], char album[], char titulo[]) {
    int c = strcmp(titulo, H->titulo);
    if (c == 0)
        c = strcmp(artista, H->artista);
    if (c == 0)
        c = strcmp(album, H->album);
    return c;
}

MusicaPlaylistRB* insereMusicaPlaylistRB(MusicaPlaylistRB *H, char artista[], char album[], char titulo[], int *resp) {
    MusicaPlaylistRB *novo;
    int cmp;

    if (H == NULL) {
        novo = (MusicaPlaylistRB*) malloc(sizeof(MusicaPlaylistRB));
        if (novo == NULL) {
            *resp = 0;
            return NULL;
        }
        strcpy(novo->artista, artista);
        strcpy(novo->album, album);
        strcpy(novo->titulo, titulo);
        novo->cor = RED;
        novo->esq = NULL;
        novo->dir = NULL;
        *resp = 1;
        return novo;
    }

    cmp = comparaMusicaPlaylistRB(H, artista, album, titulo);
    if (cmp == 0)
        *resp = 0;                               
    else if (cmp < 0)
        H->esq = insereMusicaPlaylistRB(H->esq, artista, album, titulo, resp);
    else
        H->dir = insereMusicaPlaylistRB(H->dir, artista, album, titulo, resp);

   
    if (corMusicaPlaylistRB(H->dir) == RED && corMusicaPlaylistRB(H->esq) == BLACK)
        H = rotacionaEsquerdaMusicaPlaylistRB(H);

    if (corMusicaPlaylistRB(H->esq) == RED && corMusicaPlaylistRB(H->esq->esq) == RED)
        H = rotacionaDireitaMusicaPlaylistRB(H);

    if (corMusicaPlaylistRB(H->esq) == RED && corMusicaPlaylistRB(H->dir) == RED)
        trocaCorMusicaPlaylistRB(H);

    return H;
}


int insereArvMusicaPlaylistRB(ArvMusicaPlaylistRB *raiz, char artista[], char album[], char titulo[]) {
    int resp;
    if (raiz == NULL)
        return 0;
    *raiz = insereMusicaPlaylistRB(*raiz, artista, album, titulo, &resp);
    if (*raiz != NULL)
        (*raiz)->cor = BLACK;
    return resp;
}

MusicaPlaylistRB* buscaMusicaPlaylistRB(MusicaPlaylistRB *H, char artista[], char album[], char titulo[]) {
    int cmp;
    if (H == NULL)
        return NULL;
    cmp = comparaMusicaPlaylistRB(H, artista, album, titulo);
    if (cmp == 0)
        return H;
    else if (cmp < 0)
        return buscaMusicaPlaylistRB(H->esq, artista, album, titulo);
    return buscaMusicaPlaylistRB(H->dir, artista, album, titulo);
}


int mostraMusicasPlaylistRB(MusicaPlaylistRB *H) {
    int total = 0;
    if (H == NULL)
        return 0;
    total += mostraMusicasPlaylistRB(H->esq);
    printf("  %s | artista: %s | album: %s\n", H->titulo, H->artista, H->album);
    total++;
    total += mostraMusicasPlaylistRB(H->dir);
    return total;
}

MusicaPlaylistRB* removeMPRB(MusicaPlaylistRB *H, char artista[], char album[], char titulo[]) {
    MusicaPlaylistRB *x;

    if (comparaMusicaPlaylistRB(H, artista, album, titulo) < 0) {
        if (corMusicaPlaylistRB(H->esq) == BLACK && corMusicaPlaylistRB(H->esq->esq) == BLACK)
            H = move2EsqREDMusicaPlaylistRB(H);

        H->esq = removeMPRB(H->esq, artista, album, titulo);
    } else {
        if (corMusicaPlaylistRB(H->esq) == RED)
            H = rotacionaDireitaMusicaPlaylistRB(H);

        if (comparaMusicaPlaylistRB(H, artista, album, titulo) == 0 && H->dir == NULL) {
            free(H);
            return NULL;
        }

        if (corMusicaPlaylistRB(H->dir) == BLACK && corMusicaPlaylistRB(H->dir->esq) == BLACK)
            H = move2DirREDMusicaPlaylistRB(H);

        if (comparaMusicaPlaylistRB(H, artista, album, titulo) == 0) {
            x = procuraMenorMusicaPlaylistRB(H->dir);
            strcpy(H->artista, x->artista);
            strcpy(H->album, x->album);
            strcpy(H->titulo, x->titulo);
            H->dir = removerMenorMusicaPlaylistRB(H->dir);
        } else
            H->dir = removeMPRB(H->dir, artista, album, titulo);
    }
    return balancearMusicaPlaylistRB(H);
}


int removeArvMPRB(ArvMusicaPlaylistRB *raiz, char artista[], char album[], char titulo[]) {
    if (raiz == NULL || buscaMusicaPlaylistRB(*raiz, artista, album, titulo) == NULL)
        return 0;
    *raiz = removeMPRB(*raiz, artista, album, titulo);
    if (*raiz != NULL)
        (*raiz)->cor = BLACK;
    return 1;
}


static int corPlaylistRB(PlaylistRB *H) {
    if (H == NULL)
        return BLACK;
    return H->cor;
}


static void trocaCorPlaylistRB(PlaylistRB *H) {
    H->cor = !H->cor;
    if (H->esq != NULL)
        H->esq->cor = !H->esq->cor;
    if (H->dir != NULL)
        H->dir->cor = !H->dir->cor;
}

static PlaylistRB* rotacionaDireitaPlaylistRB(PlaylistRB *A) {
    PlaylistRB *B = A->esq;
    A->esq = B->dir;
    B->dir = A;
    B->cor = A->cor;
    A->cor = RED;
    return B;
}

static PlaylistRB* rotacionaEsquerdaPlaylistRB(PlaylistRB *A) {
    PlaylistRB *B = A->dir;
    A->dir = B->esq;
    B->esq = A;
    B->cor = A->cor;
    A->cor = RED;
    return B;
}

ArvPlaylistRB* criaArvPlaylistRB() {
    ArvPlaylistRB *raiz = (ArvPlaylistRB*) malloc(sizeof(ArvPlaylistRB));
    if (raiz != NULL)
        *raiz = NULL;
    return raiz;
}

PlaylistRB* inserePlaylistRB(PlaylistRB *H, char nome[], int *resp) {
    PlaylistRB *novo;
    int cmp;

    if (H == NULL) {
        novo = (PlaylistRB*) malloc(sizeof(PlaylistRB));
        if (novo == NULL) {
            *resp = 0;
            return NULL;
        }
        strcpy(novo->nome, nome);
        novo->musicas = NULL;
        novo->cor = RED;
        novo->esq = NULL;
        novo->dir = NULL;
        *resp = 1;
        return novo;
    }

    cmp = strcmp(nome, H->nome);
    if (cmp == 0)
        *resp = 0;                                
    else if (cmp < 0)
        H->esq = inserePlaylistRB(H->esq, nome, resp);
    else
        H->dir = inserePlaylistRB(H->dir, nome, resp);

    
    if (corPlaylistRB(H->dir) == RED && corPlaylistRB(H->esq) == BLACK)
        H = rotacionaEsquerdaPlaylistRB(H);

    if (corPlaylistRB(H->esq) == RED && corPlaylistRB(H->esq->esq) == RED)
        H = rotacionaDireitaPlaylistRB(H);

    if (corPlaylistRB(H->esq) == RED && corPlaylistRB(H->dir) == RED)
        trocaCorPlaylistRB(H);

    return H;
}

PlaylistRB* buscaPlaylistRB(PlaylistRB *H, char nome[]) {
    int cmp;
    if (H == NULL)
        return NULL;
    cmp = strcmp(nome, H->nome);
    if (cmp == 0)
        return H;
    else if (cmp < 0)
        return buscaPlaylistRB(H->esq, nome);
    return buscaPlaylistRB(H->dir, nome);
}


int cadastraPlaylistRB(ArvPlaylistRB *raiz, char nome[]) {
    int resp;
    if (raiz == NULL)
        return 0;
    *raiz = inserePlaylistRB(*raiz, nome, &resp);
    if (*raiz != NULL)
        (*raiz)->cor = BLACK;
    return resp;
}


int cadastraMusicaPlaylistRB(ArvArtistaRB *artistas, ArvPlaylistRB *playlists, char nomePlaylist[],
                             char artista[], char album[], char titulo[]) {
    PlaylistRB *pl;
    ArtistaRB *art;
    AlbumRB *alb;

    if (playlists == NULL || artistas == NULL)
        return -1;
    pl = buscaPlaylistRB(*playlists, nomePlaylist);
    if (pl == NULL)
        return -1;
    art = buscaArtistaRB(*artistas, artista);
    if (art == NULL)
        return -2;
    alb = buscaAlbumRB(art->albuns, album);
    if (alb == NULL)
        return -3;
    if (buscaMusicaRB(alb->musicas, titulo) == NULL)
        return -4;

    return insereArvMusicaPlaylistRB(&(pl->musicas), artista, album, titulo);
}


int mostraPlaylistRB(ArvPlaylistRB *raiz, char nome[]) {
    PlaylistRB *pl;
    if (raiz == NULL)
        return -1;
    pl = buscaPlaylistRB(*raiz, nome);
    if (pl == NULL)
        return -1;
    printf("Playlist: %s\n", pl->nome);
    return mostraMusicasPlaylistRB(pl->musicas);
}


int removeMusicaDaPlaylistRB(ArvPlaylistRB *raiz, char nomePlaylist[], char artista[], char album[], char titulo[]) {
    PlaylistRB *pl;
    if (raiz == NULL)
        return -1;
    pl = buscaPlaylistRB(*raiz, nomePlaylist);
    if (pl == NULL)
        return -1;
    return removeArvMPRB(&(pl->musicas), artista, album, titulo);
}


int musicaEmAlgumaPlaylistRB(PlaylistRB *H, char artista[], char album[], char titulo[]) {
    if (H == NULL)
        return 0;
    if (buscaMusicaPlaylistRB(H->musicas, artista, album, titulo) != NULL)
        return 1;
    if (musicaEmAlgumaPlaylistRB(H->esq, artista, album, titulo))
        return 1;
    return musicaEmAlgumaPlaylistRB(H->dir, artista, album, titulo);
}


int removeMusicaDoAlbumRB(ArvArtistaRB *artistas, ArvPlaylistRB *playlists, char nomeArtista[], char tituloAlbum[], char titulo[]) {
    ArtistaRB *art;
    AlbumRB *alb;

    if (artistas == NULL)
        return -1;
    art = buscaArtistaRB(*artistas, nomeArtista);
    if (art == NULL)
        return -1;
    alb = buscaAlbumRB(art->albuns, tituloAlbum);
    if (alb == NULL)
        return -2;
    if (buscaMusicaRB(alb->musicas, titulo) == NULL)
        return 0;
    if (playlists != NULL && musicaEmAlgumaPlaylistRB(*playlists, nomeArtista, tituloAlbum, titulo))
        return -3;

    removeArvMusicaRB(&(alb->musicas), titulo);
    alb->qtdMusicas--;
    return 1;
}

//free em tudo

void liberaMusicasRB(MusicaRB *H) {
    if (H != NULL) {
        liberaMusicasRB(H->esq);
        liberaMusicasRB(H->dir);
        free(H);
    }
}

void liberaAlbunsRB(AlbumRB *H) {
    if (H != NULL) {
        liberaAlbunsRB(H->esq);
        liberaAlbunsRB(H->dir);
        liberaMusicasRB(H->musicas);
        free(H);
    }
}

void liberaArtistasRB(ArtistaRB *H) {
    if (H != NULL) {
        liberaArtistasRB(H->esq);
        liberaArtistasRB(H->dir);
        liberaAlbunsRB(H->albuns);
        free(H);
    }
}

void liberaMusicasPlaylistRB(MusicaPlaylistRB *H) {
    if (H != NULL) {
        liberaMusicasPlaylistRB(H->esq);
        liberaMusicasPlaylistRB(H->dir);
        free(H);
    }
}

void liberaPlaylistsRB(PlaylistRB *H) {
    if (H != NULL) {
        liberaPlaylistsRB(H->esq);
        liberaPlaylistsRB(H->dir);
        liberaMusicasPlaylistRB(H->musicas);
        free(H);
    }
}

void liberaTudoRB(ArvArtistaRB *artistas, ArvPlaylistRB *playlists) {
    if (artistas != NULL) {
        liberaArtistasRB(*artistas);
        free(artistas);
    }
    if (playlists != NULL) {
        liberaPlaylistsRB(*playlists);
        free(playlists);
    }
}

#endif