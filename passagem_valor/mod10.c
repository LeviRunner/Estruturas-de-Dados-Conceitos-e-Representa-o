#include <stdio.h>

/* Exemplo: passagem por valor
 * Argumentos escalares (int) são copiados para a função; modificações
 * dentro da função não afetam a variável original.
 */
void dobrar(int x) {
    x = x * 2;
    printf("Dentro da função: %d\n", x);
}

int main(void) {
    int numero = 5;
    dobrar(numero);
    printf("Fora da função: %d\n", numero);
    return 0;
}