// nivel aventureiro

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ===================== Estruturas ===================== //

// Nó da árvore da mansão (binária comum)
typedef struct Sala {
    char nome[40];
    char *pista; // pode ser NULL
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Nó da árvore de pistas (BST)
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// ===================== Funções utilitárias ===================== //

// Cria dinamicamente uma sala com nome e pista (pode ser NULL)
Sala* criarSala(const char *nome, const char *pistaTexto) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (!nova) {
        printf("Erro ao alocar memória para sala.\n");
        exit(1);
    }
    strncpy(nova->nome, nome, 39);
    nova->nome[39] = '\0';

    if (pistaTexto) {
        nova->pista = (char*) malloc(strlen(pistaTexto) + 1);
        if (!nova->pista) {
            printf("Erro ao alocar memória para pista.\n");
            free(nova);
            exit(1);
        }
        strcpy(nova->pista, pistaTexto);
    } else {
        nova->pista = NULL;
    }

    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// Cria novo nó de pista para BST
PistaNode* criarPistaNode(const char *pistaTexto) {
    PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
    if (!novo) {
        printf("Erro ao alocar memória para nó de pista.\n");
        exit(1);
    }
    strncpy(novo->pista, pistaTexto, 99);
    novo->pista[99] = '\0';
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

// Insere uma nova pista na árvore BST (ordem alfabética)
PistaNode* inserirPista(PistaNode *raiz, const char *pistaTexto) {
    if (raiz == NULL) {
        return criarPistaNode(pistaTexto);
    }

    if (strcmp(pistaTexto, raiz->pista) < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pistaTexto);
    } else if (strcmp(pistaTexto, raiz->pista) > 0) {
        raiz->direita = inserirPista(raiz->direita, pistaTexto);
    }
    // pistas iguais são ignoradas (sem duplicatas)
    return raiz;
}

// Exibe as pistas coletadas em ordem alfabética (in-order)
void exibirPistas(PistaNode *raiz) {
    if (raiz == NULL) return;
    exibirPistas(raiz->esquerda);
    printf("- %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

// Libera memória da árvore BST de pistas
void liberarPistas(PistaNode *raiz) {
    if (raiz == NULL) return;
    liberarPistas(raiz->esquerda);
    liberarPistas(raiz->direita);
    free(raiz);
}

// Libera memória da árvore de salas
void liberarSalas(Sala *raiz) {
    if (raiz == NULL) return;
    liberarSalas(raiz->esquerda);
    liberarSalas(raiz->direita);
    if (raiz->pista) free(raiz->pista);
    free(raiz);
}

// ===================== Função principal de exploração ===================== //

void explorarSalasComPistas(Sala *atual, PistaNode **raizPistas) {
    char opcao;

    while (atual != NULL) {
        printf("\nVocê está na sala: %s\n", atual->nome);

        if (atual->pista) {
            printf("Pista encontrada nesta sala: \"%s\"\n", atual->pista);
            *raizPistas = inserirPista(*raizPistas, atual->pista);
            // Evita inserir novamente se voltar a essa sala (desnecessário na lógica atual)
            free(atual->pista);
            atual->pista = NULL;
        } else {
            printf("Nenhuma pista nesta sala.\n");
        }

        // Opções de navegação
        printf("Escolha o próximo caminho:\n");
        if (atual->esquerda) printf("  (e) Ir para a esquerda -> %s\n", atual->esquerda->nome);
        if (atual->direita)  printf("  (d) Ir para a direita  -> %s\n", atual->direita->nome);
        printf("  (s) Sair da exploração\n");
        printf("Opção: ");
        scanf(" %c", &opcao);

        if ((opcao == 'e' || opcao == 'E') && atual->esquerda) {
            atual = atual->esquerda;
        } else if ((opcao == 'd' || opcao == 'D') && atual->direita) {
            atual = atual->direita;
        } else if (opcao == 's' || opcao == 'S') {
            printf("Encerrando exploração...\n");
            break;
        } else {
            printf("Opção inválida ou caminho inexistente. Tente novamente.\n");
        }
    }
}

// ===================== Montagem da mansão (árvore fixa) ===================== //

Sala* construirMansao() {
    Sala *hall = criarSala("Hall de Entrada", "Pegadas com barro fresco.");
    Sala *salaEstar = criarSala("Sala de Estar", "Um copo quebrado no chão.");
    Sala *cozinha = criarSala("Cozinha", NULL);
    Sala *jardim = criarSala("Jardim", "Marcas de pneu suspeitas.");
    Sala *biblioteca = criarSala("Biblioteca", "Livro com páginas arrancadas.");
    Sala *adega = criarSala("Adega", NULL);

    // Montagem da árvore binária da mansão
    hall->esquerda = salaEstar;
    hall->direita = cozinha;

    salaEstar->esquerda = jardim;
    salaEstar->direita = biblioteca;

    cozinha->esquerda = adega;

    return hall;
}

// ===================== Função principal ===================== //

int main() {
    printf("=== Detective Quest: Coleta de Pistas ===\n");

    // Montar a mansão (árvore de salas)
    Sala *mansao = construirMansao();

    // Ponteiro para a raiz da árvore de pistas
    PistaNode *pistasColetadas = NULL;

    // Exploração a partir do Hall de Entrada
    explorarSalasComPistas(mansao, &pistasColetadas);

    // Exibir pistas ordenadas
    printf("\n=== Pistas coletadas ===\n");
    if (pistasColetadas == NULL) {
        printf("Nenhuma pista foi coletada.\n");
    } else {
        exibirPistas(pistasColetadas);
    }

    // Liberação de memória
    liberarPistas(pistasColetadas);
    liberarSalas(mansao);

    printf("\nMemória liberada. Obrigado por jogar!\n");
    return 0;
}
