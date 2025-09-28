#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura de dados para representar um cômodo da mansão
typedef struct Sala {
    char nome[50];            // Nome da sala
    struct Sala *esq;         // Caminho para a esquerda
    struct Sala *dir;         // Caminho para a direita
} Sala;

// Função para criar uma nova sala na memória
Sala* criarSala(const char* nome) {
    Sala* nova = (Sala*)malloc(sizeof(Sala));  // aloca memória
    if (nova == NULL) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    strcpy(nova->nome, nome);   // copia o nome
    nova->esq = NULL;           // inicialmente sem saídas
    nova->dir = NULL;
    return nova;
}

// Função para montar o mapa fixo da mansão
Sala* montarMapa() {
    // Sala inicial
    Sala* hall = criarSala("Hall de Entrada");

    // Conexões principais
    hall->esq = criarSala("Biblioteca");
    hall->dir = criarSala("Sala de Jantar");

    // Caminhos da biblioteca
    hall->esq->esq = criarSala("Escritorio");
    hall->esq->dir = criarSala("Sala Secreta");

    // Caminhos da sala de jantar
    hall->dir->esq = criarSala("Cozinha");
    hall->dir->dir = criarSala("Jardim de Inverno");

    return hall; // retorna a raiz da árvore
}

// Função principal do jogo: exploração
void explorar(Sala* atual) {
    char escolha;

    while (1) {
        // Mostra o nome da sala atual
        printf("\nVocê está em: %s\n", atual->nome);

        // Se não houver saídas, acaba o jogo
        if (atual->esq == NULL && atual->dir == NULL) {
            printf("Esta sala não possui mais saídas. Fim da exploração!\n");
            break;
        }

        // Mostra opções de caminho disponíveis
        printf("Opções de navegação:\n");
        if (atual->esq != NULL) printf("  [e] Ir para %s\n", atual->esq->nome);
        if (atual->dir != NULL) printf("  [d] Ir para %s\n", atual->dir->nome);
        printf("  [s] Sair do jogo\n");

        // Lê a escolha do jogador
        printf("Escolha: ");
        scanf(" %c", &escolha);

        // Verifica para onde ir
        if (escolha == 'e' && atual->esq != NULL) {
            atual = atual->esq;  // vai para a esquerda
        }
        else if (escolha == 'd' && atual->dir != NULL) {
            atual = atual->dir;  // vai para a direita
        }
        else if (escolha == 's') {
            printf("Você encerrou a exploração.\n");
            break;
        }
        else {
            printf("Opção inválida! Tente novamente.\n");
        }
    }
}

// Função recursiva para liberar a memória da árvore
void liberarMapa(Sala* raiz) {
    if (raiz == NULL) return;
    liberarMapa(raiz->esq);   // libera esquerda
    liberarMapa(raiz->dir);   // libera direita
    free(raiz);               // libera a sala atual
}

int main() {
    // Monta o mapa fixo (árvore binária)
    Sala* mapa = montarMapa();

    // Inicia a exploração a partir do hall
    explorar(mapa);

    // Libera toda a memória usada
    liberarMapa(mapa);

    return 0;
}