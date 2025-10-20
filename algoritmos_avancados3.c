//nivel mestre

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --------------------------- STRUCTS ----------------------------

// Estrutura de uma sala da mansão (árvore binária)
typedef struct Sala {
    char nome[30];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Estrutura de um nó da árvore de pistas coletadas (BST)
typedef struct PistaNode {
    char pista[50];
    struct PistaNode *esq;
    struct PistaNode *dir;
} PistaNode;

// Estrutura da tabela hash para associar pista a suspeito
#define TAM_HASH 101
typedef struct HashNode {
    char pista[50];
    char suspeito[30];
    struct HashNode* prox;
} HashNode;

HashNode* tabelaHash[TAM_HASH] = {NULL};  // Inicializa a hash com NULL

// ------------------------- FUNÇÕES AUXILIARES -----------------------------

// Função para criar uma sala
Sala* criarSala(char* nome) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// Função para criar uma pista e inserir na BST
PistaNode* inserirPista(PistaNode* raiz, char* pista) {
    if (raiz == NULL) {
        PistaNode* novo = (PistaNode*) malloc(sizeof(PistaNode));
        strcpy(novo->pista, pista);
        novo->esq = NULL;
        novo->dir = NULL;
        return novo;
    }
    if (strcmp(pista, raiz->pista) < 0)
        raiz->esq = inserirPista(raiz->esq, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->dir = inserirPista(raiz->dir, pista);
    return raiz;
}

// Função de hash (soma dos caracteres % tamanho)
int hash(char* chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        soma += chave[i];
    }
    return soma % TAM_HASH;
}

// Insere pista e suspeito na tabela hash
void inserirNaHash(char* pista, char* suspeito) {
    int indice = hash(pista);
    HashNode* novo = (HashNode*) malloc(sizeof(HashNode));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabelaHash[indice];
    tabelaHash[indice] = novo;
}

// Consulta o suspeito relacionado a uma pista
char* encontrarSuspeito(char* pista) {
    int indice = hash(pista);
    HashNode* atual = tabelaHash[indice];
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0)
            return atual->suspeito;
        atual = atual->prox;
    }
    return NULL;
}

// Exibe pistas em ordem alfabética (em-ordem da BST)
void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esq);
        printf("• %s\n", raiz->pista);
        exibirPistas(raiz->dir);
    }
}

// Conta quantas pistas apontam para um suspeito
int contarPistasSuspeito(PistaNode* raiz, char* suspeito) {
    if (raiz == NULL) return 0;
    int cont = 0;
    char* s = encontrarSuspeito(raiz->pista);
    if (s != NULL && strcmp(s, suspeito) == 0)
        cont = 1;
    return cont +
           contarPistasSuspeito(raiz->esq, suspeito) +
           contarPistasSuspeito(raiz->dir, suspeito);
}

// Retorna a pista associada ao nome da sala
char* pistaDaSala(char* nome) {
    if (strcmp(nome, "Hall de Entrada") == 0) return "Pegada na poeira";
    if (strcmp(nome, "Cozinha") == 0) return "Luvas molhadas";
    if (strcmp(nome, "Sala de Estar") == 0) return "Porta entreaberta";
    if (strcmp(nome, "Biblioteca") == 0) return "Livro fora do lugar";
    if (strcmp(nome, "Quarto") == 0) return "Perfume estranho";
    if (strcmp(nome, "Porão") == 0) return "Pegadas de barro";
    return NULL;
}

// --------------------------- EXPLORAÇÃO ----------------------------

void explorarSalas(Sala* salaAtual, PistaNode** arvorePistas) {
    char escolha;
    while (1) {
        printf("\nVocê está em: %s\n", salaAtual->nome);

        char* pista = pistaDaSala(salaAtual->nome);
        if (pista != NULL) {
            printf(">> Pista encontrada: %s\n", pista);
            *arvorePistas = inserirPista(*arvorePistas, pista);
        }

        printf("\nDeseja ir para a esquerda (e), direita (d) ou sair (s)? ");
        scanf(" %c", &escolha);

        if (escolha == 'e' && salaAtual->esquerda != NULL) {
            salaAtual = salaAtual->esquerda;
        } else if (escolha == 'd' && salaAtual->direita != NULL) {
            salaAtual = salaAtual->direita;
        } else if (escolha == 's') {
            printf("\nVocê decidiu encerrar a exploração.\n");
            break;
        } else {
            printf("Caminho inválido.\n");
        }
    }
}

// --------------------------- LIBERAÇÃO DE MEMÓRIA ----------------------------

void liberarArvorePistas(PistaNode* raiz) {
    if (raiz != NULL) {
        liberarArvorePistas(raiz->esq);
        liberarArvorePistas(raiz->dir);
        free(raiz);
    }
}

void liberarSalas(Sala* raiz) {
    if (raiz != NULL) {
        liberarSalas(raiz->esquerda);
        liberarSalas(raiz->direita);
        free(raiz);
    }
}

void liberarHash() {
    for (int i = 0; i < TAM_HASH; i++) {
        HashNode* atual = tabelaHash[i];
        while (atual != NULL) {
            HashNode* temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }
}

// --------------------------- MAIN ----------------------------

int main() {
    // Semente para aleatoriedade
    srand(time(NULL));

    // Monta a mansão (árvore binária)
    Sala* hall = criarSala("Hall de Entrada");
    hall->esquerda = criarSala("Cozinha");
    hall->direita = criarSala("Sala de Estar");
    hall->esquerda->esquerda = criarSala("Porão");
    hall->esquerda->direita = criarSala("Biblioteca");
    hall->direita->direita = criarSala("Quarto");

    // Tabela hash com pistas -> suspeitos
    inserirNaHash("Pegada na poeira", "Sr. Silva");
    inserirNaHash("Luvas molhadas", "Sra. Rosa");
    inserirNaHash("Porta entreaberta", "Sr. Silva");
    inserirNaHash("Livro fora do lugar", "Srta. Azul");
    inserirNaHash("Perfume estranho", "Sra. Rosa");
    inserirNaHash("Pegadas de barro", "Sr. Silva");

    // Inicializa a árvore de pistas
    PistaNode* arvorePistas = NULL;

    // Começa a exploração
    printf("==== DETECTIVE QUEST: CAPÍTULO FINAL ====\n");
    explorarSalas(hall, &arvorePistas);

    // Lista as pistas coletadas
    printf("\nPistas coletadas (ordem alfabética):\n");
    exibirPistas(arvorePistas);

    // Acusação final
    char acusado[30];
    printf("\nQuem você acha que é o culpado? ");
    scanf(" %[^\n]", acusado);

    int cont = contarPistasSuspeito(arvorePistas, acusado);
    if (cont >= 2) {
        printf("✅ Acusação correta! %s foi desmascarado(a) com %d pistas!\n", acusado, cont);
    } else {
        printf("❌ Acusação incorreta. Apenas %d pista(s) apontam para %s.\n", cont, acusado);
    }

    // Libera memória
    liberarArvorePistas(arvorePistas);
    liberarSalas(hall);
    liberarHash();

    return 0;
}
