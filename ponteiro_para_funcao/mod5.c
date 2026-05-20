#include <stdio.h>

/* Exemplo: ponteiro para função
 * - mostra como declarar e usar um ponteiro para função
 * - ponteiro aponta para função que retorna void e não recebe parâmetros
 */
void saudacao(void) {
    printf("Oi\n");
}

int main(void) {
    /* Declaração de um ponteiro para função:
       void (*ptr)(void) - aponta para função que retorna void e não recebe args */
    void (*ptr)(void) = saudacao;

    /* Chamada via ponteiro para função */
    ptr();

    return 0;
}