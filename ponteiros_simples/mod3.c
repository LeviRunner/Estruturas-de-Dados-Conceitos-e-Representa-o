#include <stdio.h>

/*

int a;
int *p;
p = &a;  // 'p' agora guarda o endereço de 'x' 

O código também poderia ter sido inicializado assim:  
int *p = &a; 

*/

int main() {
    int x = 10;
    int* p = &x; // ponteiro para x

    printf("Valor de x: %d\n", x);
    printf("Endereço de x: %p\n", &x);
    printf("Conteúdo de p: %p\n", p);
    printf("Valor apontado por p: %d\n", *p);

    return 0;
} 