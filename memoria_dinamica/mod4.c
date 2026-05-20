//Conceitos e Representação

/*

Alocação e desalocação dinâmica de memória 
(malloc, calloc e free)

*/

//bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//funcao malloc
// int*vetor = (int*)malloc(5*sizeof(int));

//funcao calloc
// int*vetor = (int*)calloc(5, sizeof(int));

//funcao free
// free(vetor);


/* com erro

int main()
{

int *a, b;
b = 10;
a=(int*) malloc(sizeof(int)); // pedido de memoria
*a = 20;
a = &b; 
free(a);

return 0;

}

*/

//sem erro

int main()
{

int *a;
int b;
a = malloc(sizeof(int)); // pedido de memoria
a=(int*) malloc(sizeof(int)); // pedido de memoria
*a = 20;
free(a);
a = &b; 

return 0;

}