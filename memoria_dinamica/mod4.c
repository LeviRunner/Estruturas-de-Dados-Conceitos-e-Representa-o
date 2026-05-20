/* Exemplo: alocação dinâmica de memória
 * Mostra uso correto de malloc e free e aponta cuidados básicos.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
	int *a = malloc(sizeof(int)); // aloca memória para um int
	if (a == NULL) {
		fprintf(stderr, "Falha na alocação\n");
		return 1;
	}
	*a = 20;
	printf("Valor em *a: %d\n", *a);
	free(a); // libera a memória alocada

	int b = 10;
	a = &b; // agora aponta para variável local (não liberar)

	return 0;
}