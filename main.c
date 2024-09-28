#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/src.h"


void tela_inicial()
{
    printf("|                      ---------                      |\n");
    printf("|--------------------- BEM-VINDO ---------------------|\n");
    printf("|                      ---------                      |\n");
    printf("|                                                     |\n");
    printf("|                                                     |\n");
    printf("|      1. -> Gerenciar Disciplinas                    |\n");
    printf("|      2. -> Gerenciar Cursos                         |\n");
    printf("|      3. -> Gerenciar Alunos                         |\n");
    printf("|      4. -> Gerenciar Matriculas                     |\n");
    printf("|      5. -> Gerenciar Notas                          |\n");
    printf("|      6. -> Gerenciar Disciplinas                    |\n");
    printf("|      7. -> Sair                                     |\n");
    printf("|                                                     |\n");
    printf("|_____________________________________________________|\n");

}

int main()
{
    // Variáveis necessárias para o main
    int menu_inicial;

    tela_inicial();

    printf("Escolha a opção desejada: ");
    scanf("%d", &menu_inicial);
    return 0;
}