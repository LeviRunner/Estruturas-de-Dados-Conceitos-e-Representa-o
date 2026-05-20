/*
 * ============================================================
 *  Tetris Stack — Simulador de Fila de Peças Futuras
 *  ByteBros | Desenvolvedor Técnico: sistema de controle de peças
 * ============================================================
 *
 *  Estrutura principal: Fila Circular (array de tamanho fixo)
 *
 *  Conceitos aplicados:
 *    - Fila circular com reaproveitamento de espaço
 *    - Structs para representar peças
 *    - Modularização por funções
 *    - Entrada/saída via terminal
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* ──────────────────────────────────────────
   CONSTANTES
   ────────────────────────────────────────── */

#define CAPACIDADE_MAXIMA  8   /* Número máximo de peças na fila   */
#define TAMANHO_INICIAL    5   /* Peças geradas na inicialização    */

/* Tipos de peças disponíveis no Tetris */
static const char TIPOS_PECA[] = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'};
#define TOTAL_TIPOS  7

/* ──────────────────────────────────────────
   ESTRUTURAS
   ────────────────────────────────────────── */

/*
 * Peca — representa uma única peça do jogo.
 *   nome : letra que identifica o formato ('I', 'O', 'T', 'L', ...)
 *   id   : número único sequencial atribuído na criação
 */
typedef struct {
    char nome;
    int  id;
} Peca;

/*
 * FilaCircular — fila implementada sobre array com índices circulares.
 *   pecas      : array que armazena as peças
 *   inicio     : índice do primeiro elemento (frente da fila)
 *   fim        : índice onde o próximo elemento será inserido
 *   tamanho    : quantidade atual de peças na fila
 *   capacidade : limite máximo de peças
 */
typedef struct {
    Peca pecas[CAPACIDADE_MAXIMA];
    int  inicio;
    int  fim;
    int  tamanho;
    int  capacidade;
} FilaCircular;

/* Contador global de IDs — garante unicidade entre sessões */
static int proximoId = 0;

/* ──────────────────────────────────────────
   PROTÓTIPOS
   ────────────────────────────────────────── */

void   inicializarFila(FilaCircular *fila);
Peca   gerarPeca(void);
int    filaCheia(const FilaCircular *fila);
int    filaVazia(const FilaCircular *fila);
int    enqueue(FilaCircular *fila, Peca novaPeca);
int    dequeue(FilaCircular *fila, Peca *pecaRemovida);
void   exibirFila(const FilaCircular *fila);
void   exibirMenu(void);

/* ──────────────────────────────────────────
   IMPLEMENTAÇÕES
   ────────────────────────────────────────── */

/*
 * inicializarFila — configura a fila com TAMANHO_INICIAL peças geradas
 *                   aleatoriamente e define os índices de controle.
 */
void inicializarFila(FilaCircular *fila) {
    fila->inicio    = 0;
    fila->fim       = 0;
    fila->tamanho   = 0;
    fila->capacidade = CAPACIDADE_MAXIMA;

    /* Insere as peças iniciais automaticamente */
    for (int i = 0; i < TAMANHO_INICIAL; i++) {
        enqueue(fila, gerarPeca());
    }
}

/*
 * gerarPeca — cria uma peça com tipo aleatório e ID sequencial único.
 */
Peca gerarPeca(void) {
    Peca nova;
    nova.nome = TIPOS_PECA[rand() % TOTAL_TIPOS];
    nova.id   = proximoId++;
    return nova;
}

/*
 * filaCheia — retorna 1 se a fila atingiu a capacidade máxima.
 */
int filaCheia(const FilaCircular *fila) {
    return fila->tamanho == fila->capacidade;
}

/*
 * filaVazia — retorna 1 se não há peças na fila.
 */
int filaVazia(const FilaCircular *fila) {
    return fila->tamanho == 0;
}

/*
 * enqueue — insere uma nova peça no final da fila circular.
 *   Retorna 1 em caso de sucesso, 0 se a fila estiver cheia.
 */
int enqueue(FilaCircular *fila, Peca novaPeca) {
    if (filaCheia(fila)) {
        return 0; /* Operação recusada: sem espaço */
    }

    /* Posiciona a peça no slot 'fim' e avança o índice circularmente */
    fila->pecas[fila->fim] = novaPeca;
    fila->fim = (fila->fim + 1) % fila->capacidade;
    fila->tamanho++;
    return 1;
}

/*
 * dequeue — remove a peça da frente da fila e a devolve via ponteiro.
 *   Retorna 1 em caso de sucesso, 0 se a fila estiver vazia.
 */
int dequeue(FilaCircular *fila, Peca *pecaRemovida) {
    if (filaVazia(fila)) {
        return 0; /* Operação recusada: fila vazia */
    }

    /* Copia a peça da frente e avança o índice 'inicio' circularmente */
    *pecaRemovida = fila->pecas[fila->inicio];
    fila->inicio = (fila->inicio + 1) % fila->capacidade;
    fila->tamanho--;
    return 1;
}

/*
 * exibirFila — imprime todas as peças na ordem frente → fundo.
 *              Formato de cada peça: [TIPO ID]
 */
void exibirFila(const FilaCircular *fila) {
    printf("\n=== Fila de pecas (%d/%d) ===\n",
           fila->tamanho, fila->capacidade);

    if (filaVazia(fila)) {
        printf("  (fila vazia)\n");
    } else {
        printf("  Frente -> ");
        for (int i = 0; i < fila->tamanho; i++) {
            /* Percorre circularmente a partir do índice 'inicio' */
            int indice = (fila->inicio + i) % fila->capacidade;
            printf("[%c %d] ", fila->pecas[indice].nome,
                               fila->pecas[indice].id);
        }
        printf("<- Fundo\n");
    }
    printf("============================\n");
}

/*
 * exibirMenu — exibe as opções disponíveis para o jogador.
 */
void exibirMenu(void) {
    printf("\nOpcoes:\n");
    printf("  1 - Jogar peca  (dequeue — remove da frente)\n");
    printf("  2 - Inserir peca (enqueue — adiciona ao fundo)\n");
    printf("  0 - Sair\n");
    printf("Escolha: ");
}

/* ──────────────────────────────────────────
   PONTO DE ENTRADA
   ────────────────────────────────────────── */

int main(void) {
    /* Inicializa semente aleatória para variação nos tipos de peça */
    srand((unsigned int)time(NULL));

    FilaCircular fila;
    inicializarFila(&fila);

    printf("============================================\n");
    printf("   Tetris Stack — Fila de Pecas Futuras    \n");
    printf("============================================\n");

    int opcao;
    Peca pecaJogada;

    do {
        exibirFila(&fila);
        exibirMenu();

        /* Leitura segura: descarta entradas inválidas */
        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n'); /* limpa buffer */
            opcao = -1;
        }

        switch (opcao) {

            case 1: /* ── DEQUEUE: joga a peça da frente ── */
                if (dequeue(&fila, &pecaJogada)) {
                    printf("\n>> Peca jogada: [%c %d]\n",
                           pecaJogada.nome, pecaJogada.id);
                } else {
                    printf("\n!! Nao ha pecas na fila para jogar.\n");
                }
                break;

            case 2: /* ── ENQUEUE: insere nova peça ao fundo ── */
                if (!filaCheia(&fila)) {
                    Peca nova = gerarPeca();
                    enqueue(&fila, nova);
                    printf("\n>> Nova peca inserida: [%c %d]\n",
                           nova.nome, nova.id);
                } else {
                    printf("\n!! Fila cheia (max %d pecas). "
                           "Jogue uma peca primeiro.\n", CAPACIDADE_MAXIMA);
                }
                break;

            case 0: /* ── SAIR ── */
                printf("\nEncerrando Tetris Stack. Ate logo!\n");
                break;

            default:
                printf("\n!! Opcao invalida. Tente novamente.\n");
                break;
        }

    } while (opcao != 0);

    return 0;
}
