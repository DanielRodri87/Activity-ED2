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
    printf("|      6. -> Sair                                     |\n");
    printf("|                                                     |\n");
    printf("|_____________________________________________________|\n");
}

void tela_disciplinas()
{
    printf("|                     ----------                      |\n");
    printf("|-------------------- Disciplina ---------------------|\n");
    printf("|                     ----------                      |\n");
    printf("|                                                     |\n");
    printf("|     1. Cadastrar Disciplina                         |\n");
    printf("|     2. Remover Disciplina                           |\n");
    printf("|     3. Mostrar todas disciplinas                    |\n");
    printf("|     4. Mostrar Disciplina por Curso                 |\n");
    printf("|     5. Mostrar Disciplinas por Período do Curso     |\n");
    printf("|     6. Mostrar Disciplinas por aluno Matriculado    |\n");
    printf("|     7. Voltar                                       |\n");
    printf("|_____________________________________________________|\n");
}

void tela_curso()
{
    printf("|                        -----                        |\n");
    printf("|----------------------- Curso -----------------------|\n");
    printf("|                        -----                        |\n");
    printf("|                                                     |\n");
    printf("|     1. Cadastrar Curso                              |\n");
    printf("|     2. Remover Curso                                |\n");
    printf("|     3. Mostrar todos os cursos                      |\n");
    printf("|     4. Voltar                                       |\n");
    printf("|_____________________________________________________|\n");
}

void tela_notas()
{
    printf("|                          -----                      |\n");
    printf("|------------------------- Notas ---------------------|\n");
    printf("|                          -----                      |\n");
    printf("|                                                     |\n");
    printf("|     1. Cadastrar Nota                               |\n");
    printf("|     2. Remover Nota                                 |\n");
    printf("|     3. Mostrar todas Notas                          |\n");
    printf("|     4. Mostrar Notas por Curso                      |\n");
    printf("|     5. Mostrar nota por aluno e período e CH        |\n");
    printf("|     6. Exibir Notas                                 |\n");
    printf("|     7. Voltar                                       |\n");
    printf("|_____________________________________________________|\n");
}

void tela_matriculas()
{
    printf("|                      ----------                     |\n");
    printf("|--------------------- Matriculas --------------------|\n");
    printf("|                      ----------                     |\n");
    printf("|                                                     |\n");
    printf("|     1. Cadastrar Matricula                          |\n");
    printf("|     2. Remover Matricula                            |\n");
    printf("|     3. Mostrar Disciplinas aluno matriculado        |\n");
    printf("|     4. Desmatricular Aluno                          |\n");
    printf("|     5. Exibir Matricula                             |\n");
    printf("|     6. Voltar                                       |\n");
    printf("|_____________________________________________________|\n");
}

void tela_alunos()
{
    printf("|                          ------                     |\n");
    printf("|------------------------- Alunos --------------------|\n");
    printf("|                          ------                     |\n");
    printf("|                                                     |\n");
    printf("|     1. Cadastrar Aluno                              |\n");
    printf("|     2. Remover Aluno                                |\n");
    printf("|     3. Mostrar Alunos no Curso                      |\n");
    printf("|     4. Mostrar Histórico de Aluno                   |\n");
    printf("|     5. Exibir Aluno                                 |\n");
    printf("|     6. Voltar                                       |\n");
    printf("|_____________________________________________________|\n");
}

int main()
{
    int menu_inicial, menu_caso1, menu_caso2, menu_caso3, menu_caso4, menu_caso5;

    Alunos *aluno;
    Arv_Cursos *curso;
    Arv_Disciplina *disciplina;
    Arv_Notas *nota;
    Arv_Matricula *matricula;

    curso = NULL;
    disciplina = NULL;
    nota = NULL;
    matricula = NULL;
    do
    {
        tela_inicial();

        printf("Escolha a opção desejada: ");
        scanf("%d", &menu_inicial);

        switch (menu_inicial)
        {
        case 1: // Gerenciar Disciplinas
            do
            {
                tela_disciplinas();
                printf("Escolha a opção desejada: ");
                scanf("%d", &menu_caso1);
                switch (menu_caso1)
                {
                case 1: // Cadastrar Disciplina
                {
                    int codigo_curso, codigo_disciplina, periodo, ch;
                    char nome_disciplina[100];
                    printf("Informe o código do curso: ");
                    scanf("%d", &codigo_curso);
                    printf("Informe o código da disciplina: ");
                    scanf("%d", &codigo_disciplina);
                    printf("Informe o nome da disciplina: ");
                    scanf("%s", nome_disciplina);
                    printf("Informe o período: ");
                    scanf("%d", &periodo);
                    printf("Informe a carga horária: ");
                    scanf("%d", &ch);
                    cadastrar_disciplina(curso, codigo_curso, codigo_disciplina, nome_disciplina, periodo, ch);
                }
                break;
                case 2: // Remover Disciplina
                {
                    int codigo_curso, codigo_disciplina;
                    printf("Informe o código do curso: ");
                    scanf("%d", &codigo_curso);
                    printf("Informe o código da disciplina a remover: ");
                    scanf("%d", &codigo_disciplina);
                    remover_disciplina_curso(&curso, aluno, codigo_curso, codigo_disciplina);
                }
                break;
                case 3:
                    exibir_disciplinasporcurso(curso, 0);
                    break;
                case 4:
                    break;
                
                case 5:
                    break;
                
                case 6:
                    break;
                case 7:
                    break;
                default:
                    printf("Opção inválida.\n");
                    break;
                }
            } while (menu_caso1 != 7);
            break;

        case 2:
            do
            {
                tela_curso();
                printf("Escolha a opção desejada: ");
                scanf("%d", &menu_caso2);
                switch (menu_caso2)
                {
                case 1:
                {
                    int codigo_curso, quantidade_periodos;
                    char nome_curso[100];
                    printf("Informe o código do curso: ");
                    scanf("%d", &codigo_curso);
                    printf("Informe o nome do curso: ");
                    scanf("%s", nome_curso);
                    printf("Informe a quantidade de períodos: ");
                    scanf("%d", &quantidade_periodos);
                    cadastrar_curso(&curso, codigo_curso, nome_curso, quantidade_periodos);
                }
                break;
                case 2:
                    // Essa função ainda não existe
                    break;
                case 3:
                    exibir_curso(curso);
                    break;
                case 4:
                    break;
                default:
                    printf("Opção inválida.\n");
                    break;
                }
            } while (menu_caso2 != 4);
            break;

        case 3:
            tela_alunos();
            printf("Escolha a opção desejada: ");
            scanf("%d", &menu_caso3);

            int matricula, codigo_curso;
            char nome[40];

            switch (menu_caso3)
            {
            case 1:
            {
                printf("Informe o nome do aluno: ");
                scanf("%s", nome);
                printf("Informe a matricula: ");
                scanf("%d", &matricula);
                printf("Informe o código do curso: ");
                scanf("%d", &codigo_curso);
                cadastrar_aluno(&aluno, curso, matricula, nome, codigo_curso);
            }
                break;
            case 2:
                // Essa função não existe
                break;
            case 3:
                printf("Digite o código do curso: ");
                scanf("%d", &codigo_curso);

                exibir_alunosporcurso(curso, codigo_curso);
                break;
            case 4:
                // Em breve
                break;
            case 5:
                // Em breve
                break;
            case 6:
                break;

            default:
                break;
            }
        break;
        case 4:
            tela_matriculas();
            printf("Escolha a opção desejada: ");
            scanf("%d", &menu_caso4);

            switch (menu_caso4)
            {
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                break;

            default:
                break;
            }
            break;

        case 5:
            tela_notas();
            printf("Escolha a opção desejada: ");
            scanf("%d", &menu_caso5);

            switch (menu_caso1)
            {
            case 1:
                
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                break;

            default:
                break;
            }
            break;

        case 6:
            printf("Saindo...\n");
            break;

        default:
            printf("Opção inválida\n");
            break;
        }

    } while (menu_inicial != 6);

    return 0;
}