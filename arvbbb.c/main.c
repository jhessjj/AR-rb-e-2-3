#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "artista.h"
#include "album.h"
#include "musica.h"
#include "playlist.h"


void lerTexto(char s[], int tam) {
    if (fgets(s, tam, stdin) == NULL)
        s[0] = '\0';
    s[strcspn(s, "\n")] = '\0';
}


int lerInt() {
    char buf[20];
    lerTexto(buf, 20);
    return atoi(buf);
}

void menu() {
    printf("\n========== BIBLIOTECA DE MUSICA ARVBB ==========\n");
    printf(" 1 - Cadastrar artista\n");
    printf(" 2 - Cadastrar album\n");
    printf(" 3 - Cadastrar musica\n");
    printf(" 4 - Mostrar todos os artistas\n");
    printf(" 5 - Mostrar albuns de um ano de um artista\n");
    printf(" 6 - Mostrar dados de uma musica\n");
    printf(" 7 - Criar playlist\n");
    printf(" 8 - Adicionar musica a uma playlist\n");
    printf(" 9 - Mostrar uma playlist\n");
    printf("10 - Remover musica de uma playlist\n");
    printf("11 - Remover musica de um album\n");
    printf(" 0 - Sair\n");
    printf("Opcao: ");
}

int main() {
    ArvArtista *artistas = criaArvArtista();
    ArvPlaylist *playlists = criaArvPlaylist();
    char nome[100], tipo[30], estilo[50], album[100], titulo[100], playlist[100];
    int op, ano, minutos, resp;

    if (artistas == NULL || playlists == NULL) {
        printf("Erro: sem memoria!\n");
        return 1;
    }

    do {
        menu();
        op = lerInt();

        switch (op) {
        case 1:  // artista
            printf("Nome do artista: ");                lerTexto(nome, 100);
            printf("Tipo (cantor, dupla, banda...): ");  lerTexto(tipo, 30);
            printf("Estilo musical: ");                 lerTexto(estilo, 50);
            if (cadastraArtista(artistas, nome, tipo, estilo))
                printf("Artista cadastrado!\n");
            else
                printf("Nao foi possivel cadastrar: artista ja existe.\n");
            break;

        case 2:  // album
            printf("Nome do artista: ");  lerTexto(nome, 100);
            printf("Titulo do album: ");  lerTexto(album, 100);
            printf("Ano de lancamento: "); ano = lerInt();
            resp = cadastraAlbum(artistas, nome, album, ano);
            if (resp == 1)
                printf("Album cadastrado!\n");
            else if (resp == 0)
                printf("Esse artista ja tem um album com esse titulo.\n");
            else
                printf("Artista nao cadastrado.\n");
            break;

        case 3:  //cadastrar musica
            printf("Nome do artista: ");  lerTexto(nome, 100);
            printf("Titulo do album: ");  lerTexto(album, 100);
            printf("Titulo da musica: "); lerTexto(titulo, 100);
            printf("Duracao (minutos): "); minutos = lerInt();
            resp = cadastraMusica(artistas, nome, album, titulo, minutos);
            if (resp == 1)
                printf("Musica cadastrada!\n");
            else if (resp == 0)
                printf("Esse album ja tem uma musica com esse titulo.\n");
            else if (resp == -1)
                printf("Artista nao cadastrado.\n");
            else
                printf("Album nao cadastrado para esse artista.\n");
            break;

        case 4:  //print atirtsas
            if (*artistas == NULL)
                printf("Nenhum artista cadastrado.\n");
            else
                mostraArtistas(*artistas);
            break;

        case 5:  // album por ano
            printf("Nome do artista: ");  lerTexto(nome, 100);
            printf("Ano: ");              ano = lerInt();
            resp = mostraAlbunsDoArtista(artistas, nome, ano);
            if (resp == -1)
                printf("Artista nao cadastrado.\n");
            else if (resp == 0)
                printf("Nenhum album desse artista em %d.\n", ano);
            break;

        case 6:  // dados musicas 
            printf("Titulo da musica: "); lerTexto(titulo, 100);
            if (mostraDadosMusica(*artistas, titulo) == 0)
                printf("Musica nao encontrada.\n");
            break;

        case 7:  //criar play
            printf("Nome da playlist: "); lerTexto(playlist, 100);
            if (cadastraPlaylist(playlists, playlist))
                printf("Playlist criada!\n");
            else
                printf("Ja existe uma playlist com esse nome.\n");
            break;

        case 8:  //add play
            printf("Nome da playlist: ");  lerTexto(playlist, 100);
            printf("Nome do artista: ");   lerTexto(nome, 100);
            printf("Titulo do album: ");   lerTexto(album, 100);
            printf("Titulo da musica: ");  lerTexto(titulo, 100);
            resp = cadastraMusicaPlaylist(artistas, playlists, playlist, nome, album, titulo);
            if (resp == 1)
                printf("Musica adicionada a playlist!\n");
            else if (resp == 0)
                printf("Essa musica ja esta na playlist.\n");
            else if (resp == -1)
                printf("Playlist nao existe.\n");
            else if (resp == -2)
                printf("Artista nao cadastrado.\n");
            else if (resp == -3)
                printf("Album nao cadastrado para esse artista.\n");
            else
                printf("Musica nao cadastrada nesse album.\n");
            break;

        case 9:  //mostrar
            printf("Nome da playlist: "); lerTexto(playlist, 100);
            resp = mostraPlaylist(playlists, playlist);
            if (resp == -1)
                printf("Playlist nao existe.\n");
            else if (resp == 0)
                printf("  (playlist vazia)\n");
            break;

        case 10: //remosver musica da playlist
            printf("Nome da playlist: ");  lerTexto(playlist, 100);
            printf("Nome do artista: ");   lerTexto(nome, 100);
            printf("Titulo do album: ");   lerTexto(album, 100);
            printf("Titulo da musica: ");  lerTexto(titulo, 100);
            resp = removeMusicaDaPlaylist(playlists, playlist, nome, album, titulo);
            if (resp == 1)
                printf("Musica removida da playlist!\n");
            else if (resp == 0)
                printf("Essa musica nao esta na playlist.\n");
            else
                printf("Playlist nao existe.\n");
            break;

        case 11: //remover musica do album
            printf("Nome do artista: ");  lerTexto(nome, 100);
            printf("Titulo do album: ");  lerTexto(album, 100);
            printf("Titulo da musica: "); lerTexto(titulo, 100);
            resp = removeMusicaDoAlbum(artistas, playlists, nome, album, titulo);
            if (resp == 1)
                printf("Musica removida do album!\n");
            else if (resp == 0)
                printf("Musica nao encontrada nesse album.\n");
            else if (resp == -1)
                printf("Artista nao cadastrado.\n");
            else if (resp == -2)
                printf("Album nao cadastrado para esse artista.\n");
            else
                printf("Nao pode remover: a musica faz parte de uma playlist.\n");
            break;

        case 0:
            printf("Encerrando...\n");
            break;

        default:
            printf("Opcao invalida!\n");
        }
    } while (op != 0);

    liberaTudo(artistas, playlists);
    return 0;
}