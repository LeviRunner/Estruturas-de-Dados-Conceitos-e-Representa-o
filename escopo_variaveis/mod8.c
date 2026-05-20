#include <stdio.h>

int global = 10; // escopo global

void mostrarGlobal() {
    printf("Global: %d\n", global);
    }

    int main() {
        int local = 5; // escopo local
            printf("Local: %d\n", local);
                mostrarGlobal();
                    return 0;
                    } 