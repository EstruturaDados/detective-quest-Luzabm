#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código inicial serve como base para o desenvolvimento das estruturas de navegação, pistas e suspeitos.
// Use as instruções de cada região para desenvolver o sistema completo com árvore binária, árvore de busca e tabela hash.


// 🌱 Nível Novato: Mapa da Mansão com Árvore Binária
//
// - Crie uma struct Sala com nome, e dois ponteiros: esquerda e direita.
// - Use funções como criarSala(), conectarSalas() e explorarSalas().
// - A árvore é fixa: Hall de Entrada, Biblioteca, Cozinha, Sótão etc.
// - O jogador explora indo à esquerda (e) ou à direita (d).
// - Finaliza com (s) para sair.
// - Exibe o nome da sala a cada movimento.
//

typedef struct Sala {
    char nome[50];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;


// 🔍 Nível Aventureiro: Armazenamento de Pistas com Árvore de Busca
// - Crie uma struct Pista com campo texto (string).
// - Crie uma árvore binária de busca (BST) para inserir as pistas coletadas.
// - Ao visitar salas específicas, adicione pistas automaticamente com inserirBST().
// - Implemente uma função para exibir as pistas em ordem alfabética (emOrdem()).
// - Utilize alocação dinâmica e comparação de strings (strcmp) para organizar.
// - Não precisa remover ou balancear a árvore.
// - Use funções para modularizar: inserirPista(), listarPistas().
// - A árvore de pistas deve ser exibida quando o jogador quiser revisar evidências.
//

typedef struct Pista {
    char texto[120];
    struct Pista* esquerda;
    struct Pista* direita;
} Pista;


// Função: criarSala()
// Cria uma sala dinamicamente com nome informado.
Sala* criarSala(const char* nome) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));
    if (!nova) {
        printf("Erro ao alocar memória para sala!\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// Função: conectarSalas()
// Conecta uma sala principal a duas outras (esquerda e direita).
void conectarSalas(Sala* principal, Sala* esquerda, Sala* direita) {
    if (principal == NULL) return;
    principal->esquerda = esquerda;
    principal->direita = direita;
}

// Função: criarPista()
// Cria dinamicamente uma nova pista com o texto informado.
Pista* criarPista(const char* texto) {
    Pista* nova = (Pista*) malloc(sizeof(Pista));
    if (!nova) {
        printf("Erro ao alocar memória para pista!\n");
        exit(1);
    }
    strcpy(nova->texto, texto);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// ---------------------------------------------------
// Função: inserirPista()
// Insere uma pista na árvore de busca (ordem alfabética).
// ---------------------------------------------------
Pista* inserirPista(Pista* raiz, const char* texto) {
    if (raiz == NULL)
        return criarPista(texto);

    if (strcmp(texto, raiz->texto) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, texto);
    else if (strcmp(texto, raiz->texto) > 0)
        raiz->direita = inserirPista(raiz->direita, texto);

    return raiz;
}

// Função: listarPistas()
// Exibe todas as pistas em ordem alfabética (em ordem).
void listarPistas(Pista* raiz) {
    if (raiz == NULL) return;
    listarPistas(raiz->esquerda);
    printf("🧩 %s\n", raiz->texto);
    listarPistas(raiz->direita);
}

// Função: liberarPistas()
// Libera memória da árvore de pistas.
void liberarPistas(Pista* raiz) {
    if (raiz == NULL) return;
    liberarPistas(raiz->esquerda);
    liberarPistas(raiz->direita);
    free(raiz);
}

// Função: explorarSalas()
// Permite explorar a mansão e coletar pistas automaticamente.
void explorarSalas(Sala* atual, Pista** arvorePistas) {
    char opcao;

    while (atual != NULL) {
        printf("\nVocê está na: %s\n", atual->nome);

        // Adiciona pistas automaticamente conforme a sala
        if (strcmp(atual->nome, "Biblioteca") == 0)
            *arvorePistas = inserirPista(*arvorePistas, "Um livro rasgado menciona um segredo da família.");
        else if (strcmp(atual->nome, "Cozinha") == 0)
            *arvorePistas = inserirPista(*arvorePistas, "Há uma faca suja de algo suspeito na pia.");
        else if (strcmp(atual->nome, "Sótão") == 0)
            *arvorePistas = inserirPista(*arvorePistas, "Pegadas de lama levam até uma janela aberta.");
        else if (strcmp(atual->nome, "Jardim de Inverno") == 0)
            *arvorePistas = inserirPista(*arvorePistas, "Uma luva caída perto das flores.");

        // Verifica se chegou ao fim
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Não há mais saídas. Fim da exploração!\n");
            return;
        }

        printf("Deseja ir para (e) esquerda, (d) direita, (p) ver pistas ou (s) sair? ");
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
        else if (opcao == 'p' || opcao == 'P') {
            printf("\n=== Pistas Coletadas ===\n");
            if (*arvorePistas == NULL)
                printf("(Nenhuma pista encontrada ainda.)\n");
            else
                listarPistas(*arvorePistas);
        } 
        else if (opcao == 's' || opcao == 'S') {
            printf("Saindo da mansão...\n");
            return;
        } 
        else {
            printf("Opção inválida! Use 'e', 'd', 'p' ou 's'.\n");
        }
    }
}

// Função: liberarArvore()
// Libera memória de todas as salas da mansão.
void liberarArvore(Sala* raiz) {
    if (raiz == NULL) return;
    liberarArvore(raiz->esquerda);
    liberarArvore(raiz->direita);
    free(raiz);
}

// Função: main()
// Monta o mapa, inicia a exploração e exibe as pistas.
int main() {

    // Criação das salas
    Sala* hall = criarSala("Hall de Entrada");
    Sala* salaEstar = criarSala("Sala de Estar");
    Sala* cozinha = criarSala("Cozinha");
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* jardim = criarSala("Jardim de Inverno");
    Sala* sotao = criarSala("Sótão");
    Sala* quarto = criarSala("Quarto de Hóspedes");

    // Conexões (estrutura de árvore binária)
    conectarSalas(hall, salaEstar, cozinha);
    conectarSalas(salaEstar, biblioteca, jardim);
    conectarSalas(cozinha, sotao, quarto);

    // Árvore de pistas (inicialmente vazia)
    Pista* arvorePistas = NULL;

    printf("=== Detective Quest: A Mansão Misteriosa ===\n");
    printf("Explore os cômodos e colete pistas!\n");

    explorarSalas(hall, &arvorePistas);

    printf("\n=== Revisão Final das Pistas ===\n");
    if (arvorePistas == NULL)
        printf("(Você não encontrou nenhuma pista...)\n");
    else
        listarPistas(arvorePistas);

    liberarArvore(hall);
    liberarPistas(arvorePistas);

    printf("\nMemória liberada. Até a próxima investigação!\n");
    return 0;
}
