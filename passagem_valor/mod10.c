include <stdio.h>

void dobrar(int x) {
    x = x * 2;
        printf("Dentro da função: %d
        ", x);
        }

        int main() {
            int numero = 5;
                dobrar(numero);
                    printf("Fora da função: %d
                    ", numero );
                        return 0;
                        }