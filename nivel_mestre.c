#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Implementação do Nível Mestre: integra mapa (árvore binária),
// armazenamento de pistas (BST) e relacionamento pista↔suspeito via tabela hash.

// Configurações e tamanhos
#define MAX_NOME 50
#define MAX_TEXTO 120
#define TAM_HASH 13   // número primo simples para reduzir colisões

// Estruturas de dados

// Nó da árvore binária que representa uma sala
typedef struct Sala {
    char nome[MAX_NOME];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

// Nó da BST que guarda pistas (ordenadas alfabeticamente)
typedef struct Pista {
    char texto[MAX_TEXTO];
    struct Pista* esquerda;
    struct Pista* direita;
} Pista;

// Lista encadeada de pistas dentro de um suspeito (associação)
typedef struct Relacao {
    char pista[MAX_TEXTO];
    struct Relacao* prox;
} Relacao;

// Entrada da tabela hash: suspeito com lista de pistas e encadeamento para colisões
typedef struct Suspeito {
    char nome[MAX_NOME];
    Relacao* pistas;           // lista de pistas associadas a este suspeito
    struct Suspeito* prox;     // próximo na mesma bucket (encadeamento)
} Suspeito;

// Tabela hash global
Suspeito* tabelaHash[TAM_HASH];

// Funções auxiliares: Hash

// Função de hash simples: soma ASCII dos caracteres modulo TAM_HASH
int calcularHash(const char* chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++) soma += (unsigned char)chave[i];
    return soma % TAM_HASH;
}

// Inicializa a tabela com NULLs
void inicializarHash() {
    for (int i = 0; i < TAM_HASH; i++) tabelaHash[i] = NULL;
}

// Procura um suspeito por nome na tabela hash; retorna ponteiro ou NULL se não achar
Suspeito* buscarSuspeito(const char* nome) {
    int idx = calcularHash(nome);
    Suspeito* cur = tabelaHash[idx];
    while (cur != NULL) {
        if (strcmp(cur->nome, nome) == 0) return cur;
        cur = cur->prox;
    }
    return NULL;
}

// Insere um novo suspeito na bucket (sem pistas ainda); retorna ponteiro criado
Suspeito* criarSuspeito(const char* nome) {
    Suspeito* s = (Suspeito*) malloc(sizeof(Suspeito));
    if (!s) { printf("Erro malloc Suspeito\n"); exit(1); }
    strncpy(s->nome, nome, MAX_NOME-1);
    s->nome[MAX_NOME-1] = '\0';
    s->pistas = NULL;
    s->prox = NULL;
    return s;
}

// Adiciona uma pista à lista de um suspeito (insere no início)
void adicionarRelacaoASuspeito(Suspeito* s, const char* pista) {
    if (!s) return;
    Relacao* r = (Relacao*) malloc(sizeof(Relacao));
    if (!r) { printf("Erro malloc Relacao\n"); exit(1); }
    strncpy(r->pista, pista, MAX_TEXTO-1);
    r->pista[MAX_TEXTO-1] = '\0';
    r->prox = s->pistas;
    s->pistas = r;
}

// Inserir associação pista ↔ suspeito na tabela hash.
// Se o suspeito não existir, ele é criado.
void inserirHash(const char* nomeSuspeito, const char* pista) {
    int idx = calcularHash(nomeSuspeito);
    Suspeito* cur = tabelaHash[idx];

    // procura suspeito existente
    while (cur != NULL) {
        if (strcmp(cur->nome, nomeSuspeito) == 0) {
            adicionarRelacaoASuspeito(cur, pista);
            return;
        }
        cur = cur->prox;
    }

    // não encontrou: cria novo suspeito e o insere no início da bucket
    Suspeito* novo = criarSuspeito(nomeSuspeito);
    novo->prox = tabelaHash[idx];
    tabelaHash[idx] = novo;
    adicionarRelacaoASuspeito(novo, pista);
}

// Lista todos os suspeitos e suas pistas
void listarAssociacoes() {
    printf("\n=== Relação de Suspeitos e Pistas ===\n");
    int contadorTotal = 0;
    for (int i = 0; i < TAM_HASH; i++) {
        Suspeito* s = tabelaHash[i];
        while (s != NULL) {
            printf("\n👤 Suspeito: %s\n", s->nome);
            Relacao* r = s->pistas;
            if (r == NULL) printf("   (nenhuma pista associada)\n");
            while (r != NULL) {
                printf("   - %s\n", r->pista);
                r = r->prox;
            }
            s = s->prox;
            contadorTotal++;
        }
    }
    if (contadorTotal == 0) printf("(Nenhum suspeito registrado ainda)\n");
}

// Retorna o nome do suspeito com mais pistas associadas (mostra no stdout também)
void suspeitoMaisProvavel() {
    int max = 0;
    char nomeMax[MAX_NOME] = "Desconhecido";

    for (int i = 0; i < TAM_HASH; i++) {
        Suspeito* s = tabelaHash[i];
        while (s != NULL) {
            int cont = 0;
            Relacao* r = s->pistas;
            while (r != NULL) {
                cont++;
                r = r->prox;
            }
            if (cont > max) {
                max = cont;
                strncpy(nomeMax, s->nome, MAX_NOME-1);
                nomeMax[MAX_NOME-1] = '\0';
            }
            s = s->prox;
        }
    }

    printf("\n🕵️ Suspeito mais provável: %s (%d pistas associadas)\n", nomeMax, max);
}

// Libera memória das relações de um suspeito
void liberarRelacoes(Relacao* r) {
    while (r != NULL) {
        Relacao* tmp = r;
        r = r->prox;
        free(tmp);
    }
}

// Libera toda a tabela hash
void liberarHash() {
    for (int i = 0; i < TAM_HASH; i++) {
        Suspeito* s = tabelaHash[i];
        while (s != NULL) {
            Suspeito* tmp = s;
            s = s->prox;
            liberarRelacoes(tmp->pistas);
            free(tmp);
        }
        tabelaHash[i] = NULL;
    }
}

// Funções para salas (árvore)

Sala* criarSala(const char* nome) {
    Sala* s = (Sala*) malloc(sizeof(Sala));
    if (!s) { printf("Erro malloc Sala\n"); exit(1); }
    strncpy(s->nome, nome, MAX_NOME-1);
    s->nome[MAX_NOME-1] = '\0';
    s->esquerda = s->direita = NULL;
    return s;
}

void conectarSalas(Sala* principal, Sala* esquerda, Sala* direita) {
    if (!principal) return;
    principal->esquerda = esquerda;
    principal->direita = direita;
}

void liberarArvore(Sala* raiz) {
    if (!raiz) return;
    liberarArvore(raiz->esquerda);
    liberarArvore(raiz->direita);
    free(raiz);
}

// Funções para pistas (BST)

Pista* criarPista(const char* texto) {
    Pista* p = (Pista*) malloc(sizeof(Pista));
    if (!p) { printf("Erro malloc Pista\n"); exit(1); }
    strncpy(p->texto, texto, MAX_TEXTO-1);
    p->texto[MAX_TEXTO-1] = '\0';
    p->esquerda = p->direita = NULL;
    return p;
}

// Insere texto na BST (ordenado por strcmp); evita duplicatas exatas
Pista* inserirPista(Pista* raiz, const char* texto) {
    if (raiz == NULL) return criarPista(texto);
    int cmp = strcmp(texto, raiz->texto);
    if (cmp < 0) raiz->esquerda = inserirPista(raiz->esquerda, texto);
    else if (cmp > 0) raiz->direita = inserirPista(raiz->direita, texto);
    // se igual, não insere duplicata
    return raiz;
}

void listarPistas(Pista* raiz) {
    if (!raiz) return;
    listarPistas(raiz->esquerda);
    printf("🧩 %s\n", raiz->texto);
    listarPistas(raiz->direita);
}

void liberarPistas(Pista* raiz) {
    if (!raiz) return;
    liberarPistas(raiz->esquerda);
    liberarPistas(raiz->direita);
    free(raiz);
}

// Exploração: integração total

// A função explorarSalas navega pela árvore de salas.
// Ao entrar em determinadas salas, adiciona pista na BST e associa a suspeitos na hash.
void explorarSalas(Sala* atual, Pista** arvorePistas) {
    char opcao;
    while (atual != NULL) {
        printf("\nVocê está na: %s\n", atual->nome);

        // Regras de coleta: cada sala pode gerar uma pista e associações
        if (strcmp(atual->nome, "Biblioteca") == 0) {
            *arvorePistas = inserirPista(*arvorePistas, "Livro antigo com anotações sobre Blackwood.");
            inserirHash("Sr. Blackwood", "Livro antigo com anotações sobre Blackwood.");
        }
        else if (strcmp(atual->nome, "Cozinha") == 0) {
            *arvorePistas = inserirPista(*arvorePistas, "Faca suja com iniciais M.W.");
            inserirHash("Mary White", "Faca suja com iniciais M.W.");
            inserirHash("Mary White", "Manchas suspeitas na pia.");
        }
        else if (strcmp(atual->nome, "Sótão") == 0) {
            *arvorePistas = inserirPista(*arvorePistas, "Pegadas de lama levando à janela do sótão.");
            inserirHash("Empregada", "Pegadas de lama no sótão.");
        }
        else if (strcmp(atual->nome, "Jardim de Inverno") == 0) {
            *arvorePistas = inserirPista(*arvorePistas, "Luvas de seda pertencentes à Sra. Green.");
            inserirHash("Sra. Green", "Luvas de seda encontradas no jardim.");
        }

        // Se for folha, termina
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Não há mais saídas. Fim da exploração!\n");
            return;
        }

        printf("Deseja ir para (e) esquerda, (d) direita, (p) ver pistas, (h) ver suspeitos ou (s) sair? ");
        scanf(" %c", &opcao);

        if (opcao == 'e' || opcao == 'E') {
            if (atual->esquerda != NULL) atual = atual->esquerda;
            else printf("Não há sala à esquerda!\n");
        }
        else if (opcao == 'd' || opcao == 'D') {
            if (atual->direita != NULL) atual = atual->direita;
            else printf("Não há sala à direita!\n");
        }
        else if (opcao == 'p' || opcao == 'P') {
            printf("\n=== Pistas Coletadas ===\n");
            if (*arvorePistas == NULL) printf("(Nenhuma pista encontrada ainda)\n");
            else listarPistas(*arvorePistas);
        }
        else if (opcao == 'h' || opcao == 'H') {
            listarAssociacoes();
        }
        else if (opcao == 's' || opcao == 'S') {
            printf("Saindo da mansão...\n");
            return;
        }
        else {
            printf("Opção inválida! Use 'e', 'd', 'p', 'h' ou 's'.\n");
        }
    }
}

// main: monta tudo e executa
int main() {
    // inicializa a hash
    inicializarHash();

    // Monta mapa da mansão (árvore binária)
    Sala* hall = criarSala("Hall de Entrada");
    Sala* salaEstar = criarSala("Sala de Estar");
    Sala* cozinha = criarSala("Cozinha");
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* jardim = criarSala("Jardim de Inverno");
    Sala* sotao = criarSala("Sótão");
    Sala* quarto = criarSala("Quarto de Hóspedes");

    conectarSalas(hall, salaEstar, cozinha);
    conectarSalas(salaEstar, biblioteca, jardim);
    conectarSalas(cozinha, sotao, quarto);

    // Árvore de pistas (BST) iniciamente vazia
    Pista* arvorePistas = NULL;

    // Introdução
    printf("=== Detective Quest: Nível Mestre ===\n");
    printf("Explore, colete pistas e relacione suspeitos.\n");

    // Exploração (coleta de pistas e associações)
    explorarSalas(hall, &arvorePistas);

    // Revisão final
    printf("\n=== Revisão Final das Pistas (ordem alfabética) ===\n");
    if (arvorePistas == NULL) printf("(Nenhuma pista coletada)\n");
    else listarPistas(arvorePistas);

    listarAssociacoes();
    suspeitoMaisProvavel();

    // Libera memória
    liberarArvore(hall);
    liberarPistas(arvorePistas);
    liberarHash();

    printf("\nMemória liberada. Caso encerrado! 🕵️‍♀️\n");
    return 0;
}
