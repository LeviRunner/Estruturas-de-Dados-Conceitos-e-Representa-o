//biblioteca
#include <stdio.h>

//funcao void

void saudacao() {
        printf("Oi\n")
}

int main(){

    //declarando um ponteiro 'ptr' para uma funcao
    //que nao retorna nada (void) e nao tem parametros ().
    void (*ptr)() = saudacao;
    
    //chamando a funcao usando o ponteiro 'ptr'
    ptr();

    return 0;
}