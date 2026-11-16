#define WIN32_LEAN_AND_MEAN
#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../arvbbb.c/playlist.h"
#include "../arvAvl.c/playlist.h"
#include "../arvrb.c/playlist.h"

#define N 100
#define TAM 24
#define BUSCAS 10
#define REPS 2000

static const char *nomeArvore[3] = {"binaria", "AVL", "rubro-negra"};
static const char *nomeOrdem[3] = {"crescente", "decrescente", "aleatoria"};

typedef struct {
    long long passos[N];
    double tempo[N];
    Contador total;
    double tempoTotalMedio;
    int altura;
} ResInsercao;

typedef struct {
    long long passos[BUSCAS];
} ResBusca;

static char ordem[3][N][TAM];
static int escolhidos[BUSCAS];
static ResInsercao ins[3][3];
static ResBusca bus[3][3];

static ArvArtista *tBB[4];
static ArvArtistaAVL *tAVL[4];
static ArvArtistaRB *tRB[4];

static unsigned int semente = 42;
static unsigned int sementeInicial = 42;

static unsigned int aleatorio(void) {
    semente = semente * 1103515245u + 12345u;
    return (semente >> 16) & 0x7fff;
}

static double agoraUs(void) {
#ifdef _WIN32
    static LARGE_INTEGER freq;
    LARGE_INTEGER c;
    if (freq.QuadPart == 0)
        QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&c);
    return (double) c.QuadPart * 1000000.0 / (double) freq.QuadPart;
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000000.0 + ts.tv_nsec / 1000.0;
#endif
}

static int cmpNomes(const void *a, const void *b) {
    return strcmp((const char*) a, (const char*) b);
}

static void geraNomes(char nomes[N][TAM]) {
    int i, j, k, tam, igual;
    for (i = 0; i < N; i++) {
        do {
            tam = 5 + aleatorio() % 6;
            nomes[i][0] = 'A' + aleatorio() % 26;
            for (k = 1; k < tam; k++)
                nomes[i][k] = 'a' + aleatorio() % 26;
            nomes[i][tam] = '\0';
            igual = 0;
            for (j = 0; j < i; j++)
                if (strcmp(nomes[i], nomes[j]) == 0)
                    igual = 1;
        } while (igual);
    }
}

static void escolheIndices(void) {
    int k, j, i, repetido;
    for (k = 0; k < BUSCAS; k++) {
        do {
            i = aleatorio() % N;
            repetido = 0;
            for (j = 0; j < k; j++)
                if (escolhidos[j] == i)
                    repetido = 1;
        } while (repetido);
        escolhidos[k] = i;
    }
}

static void preparaOrdens(void) {
    int i;
    geraNomes(ordem[2]);
    memcpy(ordem[0], ordem[2], sizeof(ordem[2]));
    qsort(ordem[0], N, TAM, cmpNomes);
    for (i = 0; i < N; i++)
        strcpy(ordem[1][i], ordem[0][N - 1 - i]);
    escolheIndices();
}

static void criaArvore(int a, int s) {
    if (a == 0)
        tBB[s] = criaArvArtista();
    else if (a == 1)
        tAVL[s] = criaArvArtistaAVL();
    else
        tRB[s] = criaArvArtistaRB();
}

static void liberaArvore(int a, int s) {
    if (a == 0) {
        liberaArtistas(*tBB[s]);
        free(tBB[s]);
    } else if (a == 1) {
        liberaArtistasAVL(*tAVL[s]);
        free(tAVL[s]);
    } else {
        liberaArtistasRB(*tRB[s]);
        free(tRB[s]);
    }
}

static int cadastra(int a, int s, char nome[]) {
    if (a == 0)
        return cadastraArtista(tBB[s], nome, "tipo", "estilo");
    if (a == 1)
        return cadastraArtistaAVL(tAVL[s], nome, "tipo", "estilo");
    return cadastraArtistaRB(tRB[s], nome, "tipo", "estilo");
}

static void busca(int a, int s, char nome[]) {
    if (a == 0)
        buscaArtista(*tBB[s], nome);
    else if (a == 1)
        buscaArtistaAVL(*tAVL[s], nome);
    else
        buscaArtistaRB(*tRB[s], nome);
}

static int niveisBB(Artista *r) {
    int e, d;
    if (r == NULL)
        return 0;
    e = niveisBB(r->esq);
    d = niveisBB(r->dir);
    return 1 + (e > d ? e : d);
}

static int niveisAVL(ArtistaAVL *r) {
    int e, d;
    if (r == NULL)
        return 0;
    e = niveisAVL(r->esq);
    d = niveisAVL(r->dir);
    return 1 + (e > d ? e : d);
}

static int niveisRB(ArtistaRB *r) {
    int e, d;
    if (r == NULL)
        return 0;
    e = niveisRB(r->esq);
    d = niveisRB(r->dir);
    return 1 + (e > d ? e : d);
}

static int niveis(int a, int s) {
    if (a == 0)
        return niveisBB(*tBB[s]);
    if (a == 1)
        return niveisAVL(*tAVL[s]);
    return niveisRB(*tRB[s]);
}

static double tempoTotalMedio(int a, int o) {
    int r, i;
    double t0, soma = 0;
    for (r = 0; r < REPS; r++) {
        criaArvore(a, 3);
        t0 = agoraUs();
        for (i = 0; i < N; i++)
            cadastra(a, 3, ordem[o][i]);
        soma += agoraUs() - t0;
        liberaArvore(a, 3);
    }
    return soma / REPS;
}

static void experimentoInsercao(int a, int o) {
    int i;
    double t0;
    long long antes;
    ResInsercao *r = &ins[a][o];

    criaArvore(a, o);
    zeraContador();
    for (i = 0; i < N; i++) {
        antes = passosInsercao();
        t0 = agoraUs();
        cadastra(a, o, ordem[o][i]);
        r->tempo[i] = agoraUs() - t0;
        r->passos[i] = passosInsercao() - antes;
    }
    r->total = cont;
    r->altura = niveis(a, o);
    r->tempoTotalMedio = tempoTotalMedio(a, o);
}

static void experimentoBusca(int a, int o) {
    int k;
    for (k = 0; k < BUSCAS; k++) {
        zeraContador();
        busca(a, o, ordem[2][escolhidos[k]]);
        bus[a][o].passos[k] = cont.busca;
    }
}

static long long somaV(long long v[], int n) {
    int i;
    long long s = 0;
    for (i = 0; i < n; i++)
        s += v[i];
    return s;
}

static long long maxV(long long v[], int n) {
    int i;
    long long m = v[0];
    for (i = 1; i < n; i++)
        if (v[i] > m)
            m = v[i];
    return m;
}

static long long minV(long long v[], int n) {
    int i;
    long long m = v[0];
    for (i = 1; i < n; i++)
        if (v[i] < m)
            m = v[i];
    return m;
}

static void virgula(double v, char *buf) {
    char *p;
    sprintf(buf, "%.3f", v);
    for (p = buf; *p; p++)
        if (*p == '.')
            *p = ',';
}

static void imprimeResultados(void) {
    int a, o;
    ResInsercao *r;
    ResBusca *b;
    long long ts, tb;

    printf("\n=== A) insercao de %d artistas ===\n", N);
    printf("%-12s %-12s %6s %8s %8s %6s %6s\n", "arvore", "ordem", "niveis", "total", "media", "max", "min");
    for (a = 0; a < 3; a++)
        for (o = 0; o < 3; o++) {
            r = &ins[a][o];
            printf("%-12s %-12s %6d %8lld %8.2f %6lld %6lld\n", nomeArvore[a], nomeOrdem[o], r->altura,
                   somaV(r->passos, N), somaV(r->passos, N) / (double) N, maxV(r->passos, N), minV(r->passos, N));
        }

    printf("\n--- chamadas por funcao (soma das %d insercoes) e tempo ---\n", N);
    printf("%-12s %-12s %7s %8s %8s %8s %8s %11s %11s\n", "arvore", "ordem", "insere", "balanc", "atualiza",
           "rotacao", "trocaCor", "total(us)", "por ins(us)");
    for (a = 0; a < 3; a++)
        for (o = 0; o < 3; o++) {
            r = &ins[a][o];
            printf("%-12s %-12s %7lld %8lld %8lld %8lld %8lld %11.2f %11.3f\n", nomeArvore[a], nomeOrdem[o],
                   r->total.insere, r->total.balanceamento, r->total.atualizaAltura, r->total.rotacao,
                   r->total.trocaCor, r->tempoTotalMedio, r->tempoTotalMedio / N);
        }

    printf("\n=== B) busca de %d artistas ===\n", BUSCAS);
    printf("%-12s %-12s %6s %8s %8s %6s %6s\n", "arvore", "ordem", "niveis", "total", "media", "max", "min");
    for (a = 0; a < 3; a++)
        for (o = 0; o < 3; o++) {
            b = &bus[a][o];
            printf("%-12s %-12s %6d %8lld %8.2f %6lld %6lld\n", nomeArvore[a], nomeOrdem[o], ins[a][o].altura,
                   somaV(b->passos, BUSCAS), somaV(b->passos, BUSCAS) / (double) BUSCAS,
                   maxV(b->passos, BUSCAS), minV(b->passos, BUSCAS));
        }

    printf("\n=== C) insercao x busca (media de passos por operacao) ===\n");
    printf("%-12s %-12s %10s %10s %8s\n", "arvore", "ordem", "insercao", "busca", "ins/busca");
    for (a = 0; a < 3; a++)
        for (o = 0; o < 3; o++) {
            ts = somaV(ins[a][o].passos, N);
            tb = somaV(bus[a][o].passos, BUSCAS);
            printf("%-12s %-12s %10.2f %10.2f %8.2f\n", nomeArvore[a], nomeOrdem[o], ts / (double) N,
                   tb / (double) BUSCAS, (ts / (double) N) / (tb / (double) BUSCAS));
        }
}

static void gravaCsv(void) {
    int a, o, i, k;
    char buf[40];
    FILE *f = fopen("insercao.csv", "w");
    if (f != NULL) {
        fprintf(f, "arvore;ordem;indice;nome;passos;tempo_us\n");
        for (a = 0; a < 3; a++)
            for (o = 0; o < 3; o++)
                for (i = 0; i < N; i++) {
                    virgula(ins[a][o].tempo[i], buf);
                    fprintf(f, "%s;%s;%d;%s;%lld;%s\n", nomeArvore[a], nomeOrdem[o], i + 1, ordem[o][i],
                            ins[a][o].passos[i], buf);
                }
        fclose(f);
    }
    f = fopen("busca.csv", "w");
    if (f != NULL) {
        fprintf(f, "arvore;ordem;nome;passos\n");
        for (a = 0; a < 3; a++)
            for (o = 0; o < 3; o++)
                for (k = 0; k < BUSCAS; k++)
                    fprintf(f, "%s;%s;%s;%lld\n", nomeArvore[a], nomeOrdem[o], ordem[2][escolhidos[k]],
                            bus[a][o].passos[k]);
        fclose(f);
    }
}

int main(int argc, char *argv[]) {
    int a, o, k;

    if (argc > 1)
        semente = (unsigned int) atoi(argv[1]);
    sementeInicial = semente;
    preparaOrdens();

    printf("semente: %u | nomes: %d | buscas: %d | repeticoes do tempo total: %d\n", sementeInicial, N, BUSCAS, REPS);
    printf("nomes buscados:");
    for (k = 0; k < BUSCAS; k++)
        printf(" %s", ordem[2][escolhidos[k]]);
    printf("\n");

    for (a = 0; a < 3; a++)
        for (o = 0; o < 3; o++) {
            experimentoInsercao(a, o);
            experimentoBusca(a, o);
        }

    imprimeResultados();
    gravaCsv();
    printf("\narquivos gerados: insercao.csv e busca.csv\n");

    for (a = 0; a < 3; a++)
        for (o = 0; o < 3; o++)
            liberaArvore(a, o);
    return 0;
}