/*
 * ============================================================
 *  Tetris Stack — Nível Avançado
 *  Fila Circular + Pilha de Reserva + Trocas Estratégicas
 *  ByteBros | Desenvolvedor Técnico
 * ============================================================
 *
 *  Estruturas de dados:
 *    - Fila Circular  (FIFO) : peças futuras, sempre mantida cheia
 *    - Pilha Linear   (LIFO) : peças de reserva, máximo 3 slots
 *
 *  Ações disponíveis:
 *    1 - Jogar peça da frente da fila
 *    2 - Reservar peça (frente da fila → topo da pilha)
 *    3 - Usar peça do topo da pilha
 *    4 - Trocar peça da frente da fila ↔ topo da pilha
 *    5 - Troca múltipla: 3 primeiros da fila ↔ 3 peças da pilha
 *    0 - Sair
 *
 *  Regra de reposição:
 *    Toda vez que uma peça sai da FILA (ações 1, 2, 4, 5),
 *    uma nova é gerada e adicionada ao fundo para mantê-la cheia.
 *    Peças removidas da pilha (ação 3) não repõem a fila.
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* ──────────────────────────────────────────
   CONSTANTES
   ────────────────────────────────────────── */

#define CAP_FILA         8   /* Slots físicos da fila circular          */
#define TAM_INICIAL      5   /* Peças geradas ao inicializar            */
#define CAP_PILHA        3   /* Capacidade máxima da pilha de reserva   */
#define TAM_TROCA_BLOCO  3   /* Peças envolvidas na troca múltipla      */

/* Tipos de peças disponíveis no Tetris */
static const char TIPOS[] = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'};
#define TOTAL_TIPOS  7

/* ──────────────────────────────────────────
   ESTRUTURAS
   ────────────────────────────────────────── */

/*
 * Peca — unidade básica do jogo.
 *   nome : letra identificando o formato ('I', 'O', 'T', ...)
 *   id   : número único sequencial atribuído na criação
 */
typedef struct {
    char nome;
    int  id;
} Peca;

/*
 * FilaCircular — FIFO implementada sobre array com índices circulares.
 *   pecas      : array de peças armazenadas
 *   inicio     : índice da frente (próxima a sair)
 *   fim        : índice do próximo slot de inserção
 *   tamanho    : quantidade atual de peças
 *   capacidade : limite máximo (CAP_FILA)
 */
typedef struct {
    Peca pecas[CAP_FILA];
    int  inicio;
    int  fim;
    int  tamanho;
    int  capacidade;
} FilaCircular;

/*
 * PilhaReserva — LIFO implementada sobre array.
 *   pecas      : array de peças (índice 0 = base)
 *   topo       : índice do slot mais recente (-1 = vazia)
 *   capacidade : limite máximo (CAP_PILHA)
 */
typedef struct {
    Peca pecas[CAP_PILHA];
    int  topo;
    int  capacidade;
} PilhaReserva;

/* Contador global — garante IDs únicos em toda a sessão */
static int proximoId = 0;

/* ──────────────────────────────────────────
   PROTÓTIPOS
   ────────────────────────────────────────── */

Peca gerarPeca(void);

/* Fila */
void inicializarFila(FilaCircular *f);
int  filaCheia(const FilaCircular *f);
int  filaVazia(const FilaCircular *f);
int  enqueue(FilaCircular *f, Peca p);
int  dequeue(FilaCircular *f, Peca *saiu);
void exibirFila(const FilaCircular *f);

/* Pilha */
void inicializarPilha(PilhaReserva *p);
int  pilhaCheia(const PilhaReserva *p);
int  pilhaVazia(const PilhaReserva *p);
int  push(PilhaReserva *p, Peca nova);
int  pop(PilhaReserva *p, Peca *saiu);
void exibirPilha(const PilhaReserva *p);

/* Interface */
void exibirEstado(const FilaCircular *f, const PilhaReserva *p);
void exibirMenu(void);

/* Ações */
void acaoJogarPeca    (FilaCircular *f);
void acaoReservar     (FilaCircular *f, PilhaReserva *p);
void acaoUsarReservada(FilaCircular *f, PilhaReserva *p);
void acaoTrocarUma    (FilaCircular *f, PilhaReserva *p);
void acaoTrocarBloco  (FilaCircular *f, PilhaReserva *p);

/* ──────────────────────────────────────────
   UTILITÁRIO
   ────────────────────────────────────────── */

/*
 * gerarPeca — cria uma peça com tipo aleatório e ID sequencial único.
 */
Peca gerarPeca(void) {
    Peca nova;
    nova.nome = TIPOS[rand() % TOTAL_TIPOS];
    nova.id   = proximoId++;
    return nova;
}

/* ──────────────────────────────────────────
   FILA CIRCULAR
   ────────────────────────────────────────── */

void inicializarFila(FilaCircular *f) {
    f->inicio    = 0;
    f->fim       = 0;
    f->tamanho   = 0;
    f->capacidade = CAP_FILA;
    for (int i = 0; i < TAM_INICIAL; i++)
        enqueue(f, gerarPeca());
}

int filaCheia(const FilaCircular *f) { return f->tamanho == f->capacidade; }
int filaVazia(const FilaCircular *f) { return f->tamanho == 0; }

/*
 * enqueue — insere no fundo da fila circular.
 *   O índice 'fim' avança circularmente com módulo da capacidade.
 */
int enqueue(FilaCircular *f, Peca p) {
    if (filaCheia(f)) return 0;
    f->pecas[f->fim] = p;
    f->fim = (f->fim + 1) % f->capacidade;
    f->tamanho++;
    return 1;
}

/*
 * dequeue — remove da frente da fila circular.
 *   O índice 'inicio' avança circularmente com módulo da capacidade.
 */
int dequeue(FilaCircular *f, Peca *saiu) {
    if (filaVazia(f)) return 0;
    *saiu    = f->pecas[f->inicio];
    f->inicio = (f->inicio + 1) % f->capacidade;
    f->tamanho--;
    return 1;
}

void exibirFila(const FilaCircular *f) {
    printf("  Fila de pecas    : ");
    if (filaVazia(f)) {
        printf("(vazia)");
    } else {
        for (int i = 0; i < f->tamanho; i++) {
            int idx = (f->inicio + i) % f->capacidade;
            printf("[%c %d] ", f->pecas[idx].nome, f->pecas[idx].id);
        }
    }
    printf("(%d/%d)\n", f->tamanho, f->capacidade);
}

/* ──────────────────────────────────────────
   PILHA DE RESERVA
   ────────────────────────────────────────── */

void inicializarPilha(PilhaReserva *p) {
    p->topo      = -1;
    p->capacidade = CAP_PILHA;
}

int pilhaCheia(const PilhaReserva *p) { return p->topo == p->capacidade - 1; }
int pilhaVazia(const PilhaReserva *p) { return p->topo == -1; }

int push(PilhaReserva *p, Peca nova) {
    if (pilhaCheia(p)) return 0;
    p->pecas[++p->topo] = nova;
    return 1;
}

int pop(PilhaReserva *p, Peca *saiu) {
    if (pilhaVazia(p)) return 0;
    *saiu = p->pecas[p->topo--];
    return 1;
}

void exibirPilha(const PilhaReserva *p) {
    printf("  Pilha de reserva : ");
    if (pilhaVazia(p)) {
        printf("(vazia)");
    } else {
        printf("(Topo -> Base): ");
        for (int i = p->topo; i >= 0; i--)
            printf("[%c %d] ", p->pecas[i].nome, p->pecas[i].id);
    }
    printf("(%d/%d)\n", p->topo + 1, p->capacidade);
}

/* ──────────────────────────────────────────
   INTERFACE
   ────────────────────────────────────────── */

void exibirEstado(const FilaCircular *f, const PilhaReserva *p) {
    printf("\n+----------------------------------------------------+\n");
    printf("|                  ESTADO ATUAL                     |\n");
    printf("+----------------------------------------------------+\n");
    exibirFila(f);
    exibirPilha(p);
    printf("+----------------------------------------------------+\n");
}

void exibirMenu(void) {
    printf("\n  Opcoes disponiveis:\n");
    printf("    1 - Jogar peca da frente da fila\n");
    printf("    2 - Enviar peca da fila para a pilha de reserva\n");
    printf("    3 - Usar peca da pilha de reserva\n");
    printf("    4 - Trocar frente da fila <-> topo da pilha\n");
    printf("    5 - Trocar os 3 primeiros da fila com as 3 da pilha\n");
    printf("    0 - Sair\n");
    printf("  Opcao: ");
}

/* ──────────────────────────────────────────
   AÇÕES
   ────────────────────────────────────────── */

/*
 * acaoJogarPeca — remove a peça da frente (dequeue) e a descarta.
 *   Repõe a fila gerando uma nova peça ao fundo.
 */
void acaoJogarPeca(FilaCircular *f) {
    Peca jogada;
    if (!dequeue(f, &jogada)) {
        printf("  !! Fila vazia — nao ha peca para jogar.\n");
        return;
    }
    printf("  >> Peca jogada e descartada: [%c %d]\n",
           jogada.nome, jogada.id);

    Peca nova = gerarPeca();
    enqueue(f, nova);
    printf("  >> Nova peca adicionada ao fundo: [%c %d]\n",
           nova.nome, nova.id);
}

/*
 * acaoReservar — move a frente da fila para o topo da pilha.
 *   Gera nova peça para repor a fila.
 *   Falha silenciosa se pilha cheia ou fila vazia.
 */
void acaoReservar(FilaCircular *f, PilhaReserva *p) {
    if (pilhaCheia(p)) {
        printf("  !! Pilha cheia (max %d). Use uma peca reservada antes.\n",
               CAP_PILHA);
        return;
    }
    if (filaVazia(f)) {
        printf("  !! Fila vazia — nao ha peca para reservar.\n");
        return;
    }

    Peca reservada;
    dequeue(f, &reservada);
    push(p, reservada);
    printf("  >> Reservada: [%c %d] -> topo da pilha\n",
           reservada.nome, reservada.id);

    Peca nova = gerarPeca();
    enqueue(f, nova);
    printf("  >> Nova peca adicionada ao fundo: [%c %d]\n",
           nova.nome, nova.id);
}

/*
 * acaoUsarReservada — remove e descarta a peça do topo da pilha.
 *   A fila NÃO é reabastecida (nenhuma peça saiu dela).
 *   O parâmetro 'f' é recebido para manter assinatura uniforme,
 *   mas não é modificado nessa ação.
 */
void acaoUsarReservada(FilaCircular *f, PilhaReserva *p) {
    (void)f; /* fila não alterada nessa ação */

    Peca usada;
    if (!pop(p, &usada)) {
        printf("  !! Pilha vazia — nenhuma peca reservada disponivel.\n");
        return;
    }
    printf("  >> Peca reservada usada e descartada: [%c %d]\n",
           usada.nome, usada.id);
}

/*
 * acaoTrocarUma — troca a peça da frente da fila com o topo da pilha.
 *
 *   Antes: fila = [A, B, C, ...]   pilha topo = X
 *   Depois: fila = [X, B, C, ...]  pilha topo = A
 *
 *   Implementação:
 *     1. Retira A da frente da fila (dequeue).
 *     2. Retira X do topo da pilha (pop).
 *     3. Reinsere A no topo da pilha (push).
 *     4. Reinsere X na FRENTE da fila.
 *        Como a fila é circular, "inserir na frente" é feito recuando
 *        o índice 'inicio' em 1 posição (sentido anti-horário).
 *
 *   Nenhuma nova peça é gerada: o tamanho total não muda.
 */
void acaoTrocarUma(FilaCircular *f, PilhaReserva *p) {
    if (filaVazia(f)) {
        printf("  !! Fila vazia — nao ha peca para trocar.\n");
        return;
    }
    if (pilhaVazia(p)) {
        printf("  !! Pilha vazia — nao ha peca reservada para trocar.\n");
        return;
    }

    /* Passo 1 e 2: retira de ambas as estruturas */
    Peca daFila, daPilha;
    dequeue(f, &daFila);
    pop(p, &daPilha);

    /* Passo 3: antiga frente da fila vai para o topo da pilha */
    push(p, daFila);

    /*
     * Passo 4: antigo topo da pilha vai para a FRENTE da fila.
     * Recuamos 'inicio' em 1 posição (circularmente) e escrevemos
     * diretamente, incrementando tamanho.
     */
    f->inicio = (f->inicio - 1 + f->capacidade) % f->capacidade;
    f->pecas[f->inicio] = daPilha;
    f->tamanho++;

    printf("  >> Troca realizada: frente da fila [%c %d] <-> topo da pilha [%c %d]\n",
           daFila.nome, daFila.id, daPilha.nome, daPilha.id);
}

/*
 * acaoTrocarBloco — troca as 3 primeiras peças da fila com as 3 da pilha.
 *
 *   Exemplo (conforme enunciado):
 *     Antes:  fila  = [I0, L1, T2, O3, I4]   pilha (topo→base) = [O8, L7, T6]
 *     Depois: fila  = [O8, L7, T6, O3, I4]   pilha (topo→base) = [T2, L1, I0]
 *
 *   Regras de mapeamento:
 *     - As 3 peças da fila (posições 0,1,2 a partir da frente)
 *       vão para a pilha em ordem INVERTIDA (topo ← peça mais antiga).
 *     - As 3 peças da pilha (topo primeiro) entram na frente da fila
 *       na mesma ordem em que estavam no topo.
 *
 *   Implementação:
 *     1. Retira as 3 primeiras da fila → buffer temporário filaTemp[].
 *     2. Retira as 3 da pilha          → buffer temporário pilhaTemp[].
 *     3. Insere pilhaTemp[0..2] na frente da fila (ordem: topo primeiro).
 *        Usamos o mesmo truque de recuar 'inicio' 3 vezes.
 *     4. Insere filaTemp[0..2] na pilha em ordem reversa
 *        (filaTemp[2] entra primeiro → filaTemp[0] fica no topo).
 *
 *   Nenhuma nova peça é gerada: os tamanhos não mudam.
 */
void acaoTrocarBloco(FilaCircular *f, PilhaReserva *p) {
    /* Validação: ambas precisam ter pelo menos TAM_TROCA_BLOCO peças */
    if (f->tamanho < TAM_TROCA_BLOCO) {
        printf("  !! A fila precisa ter ao menos %d pecas para a troca em bloco.\n",
               TAM_TROCA_BLOCO);
        return;
    }
    if (p->topo + 1 < TAM_TROCA_BLOCO) {
        printf("  !! A pilha precisa ter ao menos %d pecas para a troca em bloco.\n",
               TAM_TROCA_BLOCO);
        return;
    }

    Peca filaTemp[TAM_TROCA_BLOCO];  /* buffer para as peças retiradas da fila  */
    Peca pilhaTemp[TAM_TROCA_BLOCO]; /* buffer para as peças retiradas da pilha */

    /* Passo 1: retira as 3 primeiras da fila (em ordem de saída) */
    for (int i = 0; i < TAM_TROCA_BLOCO; i++)
        dequeue(f, &filaTemp[i]);

    /* Passo 2: retira as 3 da pilha (do topo para a base) */
    for (int i = 0; i < TAM_TROCA_BLOCO; i++)
        pop(p, &pilhaTemp[i]);

    /*
     * Passo 3: insere as peças da pilha na FRENTE da fila.
     * pilhaTemp[0] = antigo topo   → deve ficar na frente da fila.
     * pilhaTemp[2] = mais profunda → vai para a posição 2 da fila.
     * Inserimos da mais profunda para a mais recente (ordem reversa),
     * recuando 'inicio' a cada inserção para que o topo fique na frente.
     */
    for (int i = TAM_TROCA_BLOCO - 1; i >= 0; i--) {
        f->inicio = (f->inicio - 1 + f->capacidade) % f->capacidade;
        f->pecas[f->inicio] = pilhaTemp[i];
        f->tamanho++;
    }

    /*
     * Passo 4: insere as peças da fila na pilha em ordem reversa.
     * filaTemp[0] = antiga frente → deve ficar no TOPO da pilha.
     * filaTemp[2] = terceira      → vai para a base do bloco.
     * Empilhamos da última para a primeira, assim filaTemp[0] fica no topo.
     */
    for (int i = TAM_TROCA_BLOCO - 1; i >= 0; i--)
        push(p, filaTemp[i]);

    printf("  >> Troca em bloco realizada: "
           "3 primeiros da fila <-> 3 pecas da pilha.\n");
}

/* ──────────────────────────────────────────
   PONTO DE ENTRADA
   ────────────────────────────────────────── */

int main(void) {
    srand((unsigned int)time(NULL));

    FilaCircular fila;
    PilhaReserva pilha;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    printf("==============================================\n");
    printf("  Tetris Stack — Nivel Avancado              \n");
    printf("  Fila Circular + Pilha + Trocas Estrategicas\n");
    printf("==============================================\n");

    int opcao;

    do {
        exibirEstado(&fila, &pilha);
        exibirMenu();

        /* Leitura segura: descarta entradas inválidas */
        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n');
            opcao = -1;
        }

        printf("\n");

        switch (opcao) {
            case 1: acaoJogarPeca(&fila);             break;
            case 2: acaoReservar(&fila, &pilha);      break;
            case 3: acaoUsarReservada(&fila, &pilha); break;
            case 4: acaoTrocarUma(&fila, &pilha);     break;
            case 5: acaoTrocarBloco(&fila, &pilha);   break;
            case 0: printf("  Encerrando. Ate logo!\n"); break;
            default: printf("  !! Opcao invalida. Use 0-5.\n"); break;
        }

    } while (opcao != 0);

    return 0;
}
