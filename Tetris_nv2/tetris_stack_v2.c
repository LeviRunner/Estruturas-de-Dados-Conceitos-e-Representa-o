/*
 * ============================================================
 *  Tetris Stack — Fila de Peças Futuras + Pilha de Reserva
 *  ByteBros | Nível Intermediário
 * ============================================================
 *
 *  Estruturas de dados utilizadas:
 *    - Fila Circular  : peças futuras (FIFO)
 *    - Pilha Linear   : peças reservadas (LIFO)
 *
 *  Regras principais:
 *    - A fila é mantida sempre cheia: após qualquer remoção,
 *      uma nova peça é gerada e adicionada ao fundo.
 *    - Peças removidas (jogadas ou usadas da pilha) saem do jogo.
 *    - Reservar = move frente da fila → topo da pilha (se houver espaço).
 *    - Usar reservada = remove do topo da pilha (peça sai do jogo).
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* ──────────────────────────────────────────
   CONSTANTES
   ────────────────────────────────────────── */

#define CAPACIDADE_FILA   8   /* Slots disponíveis na fila circular  */
#define TAMANHO_INICIAL   5   /* Peças geradas na inicialização       */
#define CAPACIDADE_PILHA  3   /* Limite máximo da pilha de reserva    */

/* Tipos de peças disponíveis */
static const char TIPOS_PECA[] = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'};
#define TOTAL_TIPOS  7

/* ──────────────────────────────────────────
   ESTRUTURAS
   ────────────────────────────────────────── */

/*
 * Peca — unidade básica do jogo.
 *   nome : letra que identifica o formato ('I', 'O', 'T', ...)
 *   id   : número único sequencial atribuído na criação
 */
typedef struct {
    char nome;
    int  id;
} Peca;

/*
 * FilaCircular — fila implementada sobre array com índices circulares.
 *   pecas      : array que armazena as peças
 *   inicio     : índice da frente da fila
 *   fim        : índice onde o próximo elemento será inserido
 *   tamanho    : quantidade atual de peças
 *   capacidade : limite máximo
 */
typedef struct {
    Peca pecas[CAPACIDADE_FILA];
    int  inicio;
    int  fim;
    int  tamanho;
    int  capacidade;
} FilaCircular;

/*
 * PilhaReserva — pilha linear (LIFO) para peças reservadas.
 *   pecas      : array que armazena as peças (índice 0 = base)
 *   topo       : índice do próximo slot livre (-1 = vazia)
 *   capacidade : limite máximo
 */
typedef struct {
    Peca pecas[CAPACIDADE_PILHA];
    int  topo;
    int  capacidade;
} PilhaReserva;

/* Contador global de IDs — garante unicidade entre todas as peças geradas */
static int proximoId = 0;

/* ──────────────────────────────────────────
   PROTÓTIPOS
   ────────────────────────────────────────── */

/* Utilitário */
Peca gerarPeca(void);

/* Fila circular */
void inicializarFila(FilaCircular *fila);
int  filaCheia(const FilaCircular *fila);
int  filaVazia(const FilaCircular *fila);
int  enqueue(FilaCircular *fila, Peca nova);
int  dequeue(FilaCircular *fila, Peca *removida);
void exibirFila(const FilaCircular *fila);

/* Pilha de reserva */
void inicializarPilha(PilhaReserva *pilha);
int  pilhaCheia(const PilhaReserva *pilha);
int  pilhaVazia(const PilhaReserva *pilha);
int  push(PilhaReserva *pilha, Peca nova);
int  pop(PilhaReserva *pilha, Peca *removida);
void exibirPilha(const PilhaReserva *pilha);

/* Interface */
void exibirEstado(const FilaCircular *fila, const PilhaReserva *pilha);
void exibirMenu(void);

/* Ações do jogador */
void jogarPeca(FilaCircular *fila);
void reservarPeca(FilaCircular *fila, PilhaReserva *pilha);
void usarReservada(FilaCircular *fila, PilhaReserva *pilha);

/* ──────────────────────────────────────────
   UTILITÁRIO
   ────────────────────────────────────────── */

/*
 * gerarPeca — cria uma peça com tipo aleatório e ID sequencial único.
 */
Peca gerarPeca(void) {
    Peca nova;
    nova.nome = TIPOS_PECA[rand() % TOTAL_TIPOS];
    nova.id   = proximoId++;
    return nova;
}

/* ──────────────────────────────────────────
   FILA CIRCULAR
   ────────────────────────────────────────── */

/*
 * inicializarFila — configura índices e preenche com TAMANHO_INICIAL peças.
 */
void inicializarFila(FilaCircular *fila) {
    fila->inicio    = 0;
    fila->fim       = 0;
    fila->tamanho   = 0;
    fila->capacidade = CAPACIDADE_FILA;

    for (int i = 0; i < TAMANHO_INICIAL; i++) {
        enqueue(fila, gerarPeca());
    }
}

/* filaCheia — retorna 1 se a fila atingiu a capacidade máxima. */
int filaCheia(const FilaCircular *fila) {
    return fila->tamanho == fila->capacidade;
}

/* filaVazia — retorna 1 se não há peças na fila. */
int filaVazia(const FilaCircular *fila) {
    return fila->tamanho == 0;
}

/*
 * enqueue — insere nova peça no fundo da fila circular.
 *   Retorna 1 se bem-sucedido, 0 se a fila estiver cheia.
 */
int enqueue(FilaCircular *fila, Peca nova) {
    if (filaCheia(fila)) return 0;

    fila->pecas[fila->fim] = nova;
    fila->fim = (fila->fim + 1) % fila->capacidade; /* avança circularmente */
    fila->tamanho++;
    return 1;
}

/*
 * dequeue — remove a peça da frente da fila e a devolve via ponteiro.
 *   Retorna 1 se bem-sucedido, 0 se a fila estiver vazia.
 */
int dequeue(FilaCircular *fila, Peca *removida) {
    if (filaVazia(fila)) return 0;

    *removida    = fila->pecas[fila->inicio];
    fila->inicio = (fila->inicio + 1) % fila->capacidade;
    fila->tamanho--;
    return 1;
}

/*
 * exibirFila — imprime as peças na ordem frente → fundo.
 */
void exibirFila(const FilaCircular *fila) {
    printf("  Fila de pecas      : ");
    if (filaVazia(fila)) {
        printf("(vazia)");
    } else {
        printf("Frente -> ");
        for (int i = 0; i < fila->tamanho; i++) {
            int idx = (fila->inicio + i) % fila->capacidade;
            printf("[%c %d] ", fila->pecas[idx].nome, fila->pecas[idx].id);
        }
        printf("<- Fundo");
    }
    printf("  (%d/%d)\n", fila->tamanho, fila->capacidade);
}

/* ──────────────────────────────────────────
   PILHA DE RESERVA
   ────────────────────────────────────────── */

/*
 * inicializarPilha — prepara a pilha vazia.
 */
void inicializarPilha(PilhaReserva *pilha) {
    pilha->topo      = -1;           /* -1 indica pilha vazia */
    pilha->capacidade = CAPACIDADE_PILHA;
}

/* pilhaCheia — retorna 1 se a pilha atingiu a capacidade máxima. */
int pilhaCheia(const PilhaReserva *pilha) {
    return pilha->topo == pilha->capacidade - 1;
}

/* pilhaVazia — retorna 1 se não há peças na pilha. */
int pilhaVazia(const PilhaReserva *pilha) {
    return pilha->topo == -1;
}

/*
 * push — empilha uma peça no topo.
 *   Retorna 1 se bem-sucedido, 0 se a pilha estiver cheia.
 */
int push(PilhaReserva *pilha, Peca nova) {
    if (pilhaCheia(pilha)) return 0;

    pilha->topo++;
    pilha->pecas[pilha->topo] = nova;
    return 1;
}

/*
 * pop — remove e devolve a peça do topo.
 *   Retorna 1 se bem-sucedido, 0 se a pilha estiver vazia.
 */
int pop(PilhaReserva *pilha, Peca *removida) {
    if (pilhaVazia(pilha)) return 0;

    *removida = pilha->pecas[pilha->topo];
    pilha->topo--;
    return 1;
}

/*
 * exibirPilha — imprime as peças do topo para a base.
 */
void exibirPilha(const PilhaReserva *pilha) {
    printf("  Pilha de reserva   : ");
    if (pilhaVazia(pilha)) {
        printf("(vazia)");
    } else {
        printf("Topo -> ");
        for (int i = pilha->topo; i >= 0; i--) {
            printf("[%c %d] ", pilha->pecas[i].nome, pilha->pecas[i].id);
        }
        printf("<- Base");
    }
    printf("  (%d/%d)\n", pilha->topo + 1, pilha->capacidade);
}

/* ──────────────────────────────────────────
   INTERFACE
   ────────────────────────────────────────── */

/*
 * exibirEstado — painel com o estado atual da fila e da pilha.
 */
void exibirEstado(const FilaCircular *fila, const PilhaReserva *pilha) {
    printf("\n+--------------------------------------------------+\n");
    printf("|               ESTADO ATUAL                      |\n");
    printf("+--------------------------------------------------+\n");
    exibirFila(fila);
    exibirPilha(pilha);
    printf("+--------------------------------------------------+\n");
}

/*
 * exibirMenu — exibe as quatro opções disponíveis.
 */
void exibirMenu(void) {
    printf("\n  Opcoes:\n");
    printf("    1 - Jogar peca       (remove da frente da fila)\n");
    printf("    2 - Reservar peca    (frente da fila -> topo da pilha)\n");
    printf("    3 - Usar reservada   (remove do topo da pilha)\n");
    printf("    0 - Sair\n");
    printf("  Opcao: ");
}

/* ──────────────────────────────────────────
   AÇÕES DO JOGADOR
   ────────────────────────────────────────── */

/*
 * jogarPeca — remove a peça da frente da fila (ela sai do jogo)
 *             e gera automaticamente uma nova peça para repor a fila.
 */
void jogarPeca(FilaCircular *fila) {
    Peca jogada;

    if (!dequeue(fila, &jogada)) {
        printf("\n  !! Fila vazia — nao ha peca para jogar.\n");
        return;
    }

    printf("\n  >> Peca jogada: [%c %d]\n", jogada.nome, jogada.id);

    /* Repõe automaticamente ao fundo */
    Peca nova = gerarPeca();
    enqueue(fila, nova);
    printf("  >> Nova peca gerada e adicionada ao fundo: [%c %d]\n",
           nova.nome, nova.id);
}

/*
 * reservarPeca — move a peça da frente da fila para o topo da pilha.
 *                Uma nova peça é gerada para manter a fila cheia.
 *                Falha se a pilha estiver cheia.
 */
void reservarPeca(FilaCircular *fila, PilhaReserva *pilha) {
    if (pilhaCheia(pilha)) {
        printf("\n  !! Pilha de reserva cheia (max %d). "
               "Use uma peca reservada antes.\n", CAPACIDADE_PILHA);
        return;
    }
    if (filaVazia(fila)) {
        printf("\n  !! Fila vazia — nao ha peca para reservar.\n");
        return;
    }

    Peca reservada;
    dequeue(fila, &reservada);         /* retira da frente da fila  */
    push(pilha, reservada);            /* empilha na reserva        */

    printf("\n  >> Peca reservada: [%c %d] -> topo da pilha\n",
           reservada.nome, reservada.id);

    /* Repõe automaticamente ao fundo */
    Peca nova = gerarPeca();
    enqueue(fila, nova);
    printf("  >> Nova peca gerada e adicionada ao fundo: [%c %d]\n",
           nova.nome, nova.id);
}

/*
 * usarReservada — remove e usa a peça do topo da pilha (sai do jogo).
 *                 A fila NÃO precisa ser reabastecida nessa operação,
 *                 pois nenhuma peça foi removida dela.
 */
void usarReservada(FilaCircular *fila, PilhaReserva *pilha) {
    /* 'fila' recebida mas não modificada aqui; mantido para consistência */
    (void)fila;

    Peca usada;
    if (!pop(pilha, &usada)) {
        printf("\n  !! Pilha de reserva vazia — nenhuma peca disponivel.\n");
        return;
    }

    printf("\n  >> Peca reservada usada: [%c %d] (removida da pilha)\n",
           usada.nome, usada.id);
}

/* ──────────────────────────────────────────
   PONTO DE ENTRADA
   ────────────────────────────────────────── */

int main(void) {
    /* Inicializa semente para variação aleatória dos tipos de peça */
    srand((unsigned int)time(NULL));

    FilaCircular fila;
    PilhaReserva pilha;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    printf("============================================\n");
    printf("  Tetris Stack — Fila + Pilha de Reserva  \n");
    printf("============================================\n");

    int opcao;

    do {
        exibirEstado(&fila, &pilha);
        exibirMenu();

        /* Leitura segura: descarta entradas não numéricas */
        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n');
            opcao = -1;
        }

        switch (opcao) {
            case 1:
                jogarPeca(&fila);
                break;
            case 2:
                reservarPeca(&fila, &pilha);
                break;
            case 3:
                usarReservada(&fila, &pilha);
                break;
            case 0:
                printf("\n  Encerrando Tetris Stack. Ate logo!\n");
                break;
            default:
                printf("\n  !! Opcao invalida. Digite 0, 1, 2 ou 3.\n");
                break;
        }

    } while (opcao != 0);

    return 0;
}
