#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código inicial serve como base para o desenvolvimento das estruturas de navegação, pistas e suspeitos.
// Use as instruções de cada região para desenvolver o sistema completo com árvore binária, árvore de busca e tabela hash.

// ===================================================
// 🌱 Nível Novato: Mapa da Mansão com Árvore Binária
// ===================================================
//
// - Cria uma struct Sala com nome e ponteiros para a esquerda e direita.
// - As funções criarSala(), conectarSalas() e explorarSalas() constroem e exploram o mapa.
// - A navegação é feita por escolhas: esquerda (e), direita (d) ou sair (s).
// - A estrutura é fixa, representando cômodos da mansão.
// - Nenhuma inserção dinâmica de novas salas ocorre durante a execução.
//

// Estrutura da sala (nó da árvore binária)
typedef struct Sala {
    char nome[50];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

// ---------------------------------------------------
// Função: criarSala()
// Cria uma nova sala dinamicamente e retorna seu ponteiro.
// ---------------------------------------------------
Sala* criarSala(const char* nome) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro ao alocar memória para a sala!\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// ---------------------------------------------------
// Função: conectarSalas()
// Conecta uma sala principal a duas outras (esquerda e direita).
// ---------------------------------------------------
void conectarSalas(Sala* principal, Sala* esquerda, Sala* direita) {
    if (principal == NULL) return;
    principal->esquerda = esquerda;
    principal->direita = direita;
}

// ---------------------------------------------------
// Função: explorarSalas()
// Permite que o jogador explore a mansão interativamente.
// ---------------------------------------------------
void explorarSalas(Sala* atual) {
    char opcao;

    while (atual != NULL) {
        printf("\nVocê está na: %s\n", atual->nome);

        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Não há mais saídas. Fim da exploração!\n");
            return;
        }

        printf("Deseja ir para (e) esquerda, (d) direita ou (s) sair? ");
        scanf(" %c", &opcao);

        if (opcao == 'e' || opcao == 'E') {
            if (atual->esquerda != NULL)
                atual = atual->esquerda;
            else
                printf("Não há sala à esquerda!\n");
        } 
        else if (opcao == 'd' || opcao == 'D') {
            if (atual->direita != NULL)
                atual = atual->direita;
            else
                printf("Não há sala à direita!\n");
        } 
        else if (opcao == 's' || opcao == 'S') {
            printf("Saindo da mansão...\n");
            return;
        } 
        else {
            printf("Opção inválida! Use 'e', 'd' ou 's'.\n");
        }
    }
}

// ---------------------------------------------------
// Função: liberarArvore()
// Libera a memória de todas as salas recursivamente.
// ---------------------------------------------------
void liberarArvore(Sala* raiz) {
    if (raiz == NULL) return;
    liberarArvore(raiz->esquerda);
    liberarArvore(raiz->direita);
    free(raiz);
}

// ---------------------------------------------------
// Função: main()
// Monta o mapa da mansão e inicia o jogo.
// ---------------------------------------------------
int main() {

    // Criação das salas principais
    Sala* hall = criarSala("Hall de Entrada");
    Sala* salaEstar = criarSala("Sala de Estar");
    Sala* cozinha = criarSala("Cozinha");
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* jardim = criarSala("Jardim de Inverno");
    Sala* sotao = criarSala("Sótão");
    Sala* quarto = criarSala("Quarto de Hóspedes");

    // Conexões entre as salas (árvore binária)
    conectarSalas(hall, salaEstar, cozinha);
    conectarSalas(salaEstar, biblioteca, jardim);
    conectarSalas(cozinha, sotao, quarto);

    // Início da exploração
    printf("=== Detective Quest: Exploração da Mansão ===\n");
    explorarSalas(hall);

    // Liberação da memória
    liberarArvore(hall);
    printf("\nMemória liberada. Até a próxima investigação!\n");

    return 0;
}
