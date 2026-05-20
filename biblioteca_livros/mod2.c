
//bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//constantes globais
#define MAX_LIVROS 50
#define TAM_STRING 100

//estrutura

struct Livro {
    char nome[TAM_STRING];
    char autor[TAM_STRING];
    char editora[TAM_STRING];
    int edicao;
};

// funcao limpar buffer de entrada

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

//funcao principal
int main() {
    struct Livro biblioteca[MAX_LIVROS];
    int totalLivros = 0;
    int opcao;

    //laco principal menu
    do {
        //exibe menu de opcoes
        printf("**************************\n");
        printf("        BIBLIOTECA        \n");
        printf("**************************\n");
        printf("1 - CADASTRAR NOVO LIVRO  \n");
        printf("2 - LISTAR TODOS OS LIVROS\n");
        printf("0 - SAIR                  \n");
        printf("**************************\n");
        printf("ESCOLHA UMA OPCAO:        ");

        //le a opcao do usuario
        if (scanf("%d", &opcao) != 1) {
            limparBufferEntrada();
            opcao = -1;
        }
        limparBufferEntrada(); //limpa o '\n' deixado pelo scanf

        //processamento da opcao
        switch (opcao) {
            case 1: //cadastro de livro7
                printf("***CADASTRO DE NOVO LIVRO***\n\n");

                if (totalLivros < MAX_LIVROS) {
                    printf("DIGITE O NOME DO LIVRO: ");
                    fgets(biblioteca[totalLivros].nome, TAM_STRING, stdin);

                    printf("DIGITE O AUTOR: ");
                    fgets(biblioteca[totalLivros].autor, TAM_STRING, stdin);

                    printf("DIGITE A EDITORA: ");
                    fgets(biblioteca[totalLivros].editora, TAM_STRING, stdin);

                    biblioteca[totalLivros].nome[strcspn(biblioteca[totalLivros].nome, "\n")] = '\0';
                    biblioteca[totalLivros].autor[strcspn(biblioteca[totalLivros].autor, "\n")] = '\0';
                    biblioteca[totalLivros].editora[strcspn(biblioteca[totalLivros].editora, "\n")] = '\0';

                    printf("DIGITE A EDICAO: ");
                    scanf("%d", &biblioteca[totalLivros].edicao);
                    limparBufferEntrada();

                    totalLivros++;

                    printf("\nLIVRO CADASTRADO COM SUCESSO!\n");
                } else {
                    printf("BIBLIOTECA CHEIA! NAO E POSSIVEL CADASTRAR MAIS LIVROS.\n");
                }
                break;

            case 2: //listagem de livro
                printf("***LISTA DE LIVROS CADASTRADOS***\n\n");

                if (totalLivros == 0){
                    printf("NENHUM LIVRO CADASTRADO\n");
                } else {
                    for (int i = 0; i < totalLivros; i++) {
                        printf("***********************************\n");
                        printf("LIVRO %d\n", i + 1);
                        printf("NOME %s\n", biblioteca[i].nome);
                        printf("AUTOR %s\n", biblioteca[i].autor);
                        printf("EDITORA %s\n", biblioteca[i].editora);
                        printf("EDICAO %d\n", biblioteca[i].edicao);
                    }
                    printf("***********************************\n");
                }

                //a pausa e crucial para que o usuario veja a lista
                //do proximo loop limpar a tela
                printf("\nPRECIONE ENTER PARA CONTINUAR...");
                getchar();
                break;

            case 0: //sair
                printf("\nSAINDO DO SISTEMA...\n");
                break;

            default: // opcao invalida
                printf("\nOPCAO INVALIDA TENTE NOVAMENTE.\n");
                printf("\nPRESSIONE ENTER PARA CONTINUAR...");
                getchar();
                break;
        }

    } while (opcao != 0);

    return 0; //fim do programa
}
