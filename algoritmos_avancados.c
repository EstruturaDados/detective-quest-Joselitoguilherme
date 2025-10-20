// nivel novato

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struct que representa uma sala na mansão
typedef struct Sala {
    char nome[30];
    struct Sala* esquerda;  // Caminho à esquerda
    struct Sala* direita;   // Caminho à direita
} Sala;

// Função que cria uma nova sala dinamicamente
Sala* criarSala(const char* nome) {
    Sala* novaSala = (Sala*) malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro ao alocar memória para a sala.\n");
        exit(1);
    }

    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;

    return novaSala;
}

// Função que permite o jogador explorar a mansão
void explorarSalas(Sala* atual) {
    char escolha;

    while (atual != NULL) {
        printf("\nVocê está em: %s\n", atual->nome);

        // Verifica se é uma sala terminal (sem caminhos)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Não há mais caminhos a seguir. Fim da exploração!\n");
            break;
        }

        printf("Escolha um caminho:\n");
        if (atual->esquerda != NULL) {
            printf("  (e) Ir para a esquerda: %s\n", atual->esquerda->nome);
        }
        if (atual->direita != NULL) {
            printf("  (d) Ir para a direita: %s\n", atual->direita->nome);
        }
        printf("  (s) Sair da exploração\n");

        printf("Sua escolha: ");
        scanf(" %c", &escolha);

        if (escolha == 'e' && atual->esquerda != NULL) {
            atual = atual->esquerda;
        } else if (escolha == 'd' && atual->direita != NULL) {
            atual = atual->direita;
        } else if (escolha == 's') {
            printf("Você decidiu encerrar a exploração.\n");
            break;
        } else {
            printf("Opção inválida. Tente novamente.\n");
        }
    }
}

// Função principal que monta a árvore da mansão
int main() {
    // Criação das salas
    Sala* hall = criarSala("Hall de Entrada");
    Sala* salaEstar = criarSala("Sala de Estar");
    Sala* cozinha = criarSala("Cozinha");
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* jardim = criarSala("Jardim");
    Sala* laboratorio = criarSala("Laboratório Secreto");

    // Montagem da estrutura da mansão (árvore binária)
    hall->esquerda = salaEstar;
    hall->direita = cozinha;

    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;

    cozinha->direita = laboratorio;

    printf("==== Detective Quest ====\n");
    printf("Você deve explorar a mansão em busca de pistas!\n");

    // Início da exploração
    explorarSalas(hall);

    // Liberação da memória
    free(hall);
    free(salaEstar);
    free(cozinha);
    free(biblioteca);
    free(jardim);
    free(laboratorio);

    printf("Memória liberada. Programa encerrado com sucesso.\n");
    return 0;
}
