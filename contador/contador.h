#ifndef CONTADOR_H
#define CONTADOR_H

typedef struct {
    long long insere;
    long long busca;
    long long balanceamento;
    long long atualizaAltura;
    long long rotacao;
    long long trocaCor;
} Contador;

static Contador cont;

static inline long long passosInsercao(void) {
    return cont.insere + cont.balanceamento + cont.atualizaAltura + cont.rotacao + cont.trocaCor;
}

static inline void zeraContador(void) {
    Contador zero = {0, 0, 0, 0, 0, 0};
    cont = zero;
}

#endif