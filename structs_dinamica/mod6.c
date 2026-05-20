//bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//estrutura

typedef struct {
    char nome[50];
    int idade;
} Pessoa;

//funcao

int main() {
    //criando um ponteiro para a estrutura Pessoa
    Pessoa *p = (Pessoa *)malloc(sizeof(Pessoa));

    //verificando se a alocacao de memoria foi bem sucedida
    if (p == NULL) {
        printf("Erro ao alocar memoria\n");
        return 1;
    }

    //preenchendo os dados da pessoa
    strcpy(p->nome, "Joao");
    p->idade = 30;

    //imprimindo os dados da pessoa
    printf("Nome: %s\n", p->nome);
    printf("Idade: %d\n", p->idade);

    //liberando a memoria alocada
    free(p);

    return 0;
}
