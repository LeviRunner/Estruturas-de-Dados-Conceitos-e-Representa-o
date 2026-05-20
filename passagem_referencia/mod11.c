#include <stdio.h>

/* Exemplo: passagem por referência (via ponteiro)
 * A função recebe um ponteiro e modifica o conteúdo referenciado.
 */
void dobrar(int* x) {
    if (x != NULL) {
        *x = (*x) * 2;
    }
}

int main(void) {
    int numero = 5;
    dobrar(&numero);
    printf("Número dobrado: %d\n", numero);
    return 0;
}