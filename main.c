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
    printf("|      6. -> Menu Geral                               |\n");
    printf("|      7. -> Sair                                     |\n");
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
    printf("|     5. Exibir Matriculas                            |\n");
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

void menu_geral()
{
    printf("|                          ------                     |\n");
    printf("|--------------------- Menu Geral --------------------|\n");
    printf("|                          ------                     |\n");
    printf("|                                                     |\n");
    printf("|     1. Cadastrar Aluno (i)                          |\n");
    printf("|     2. Cadastrar Curso (ii)                         |\n");
    printf("|     3. Cadastrar Disciplina (iii)                   |\n");
    printf("|     4. Cadastrar Matrícula (iv)                     |\n");
    printf("|     5. Cadastrar Nota (v)                           |\n");
    printf("|     6. Mostrar Alunos de um Curso (vi)              |\n");
    printf("|     7. Mostrar Cursos do Campus (vii)               |\n");
    printf("|     8. Mostrar Disciplinas de um Curso (viii)       |\n");
    printf("|     9. Mostrar Disciplinas de um Período (ix)       |\n");
    printf("|    10. Mostrar Disciplinas Matriculadas (x)         |\n");
    printf("|    11. Mostrar Notas por Período (xi)               |\n");
    printf("|    12. Mostrar Nota de uma Disciplina (xii)         |\n");
    printf("|    13. Remover Disciplina de Curso (xiii)           |\n");
    printf("|    14. Remover Disciplina da Matrícula (xiv)        |\n");
    printf("|    15. Mostrar Histórico do Aluno (xv)              |\n");
    printf("|    16. Voltar                                       |\n");
    printf("|_____________________________________________________|\n");
}

int main()
{
    int menu_inicial, menu_caso1, menu_caso2, menu_caso3, menu_caso4, menu_caso5, menu_caso6;

    Alunos *aluno;
    Arv_Cursos *arv_curso;
    Arv_Disciplina *arv_disciplina;
    Arv_Notas *arv_nota;
    Arv_Matricula *arv_matricula;

    arv_curso = NULL;
    arv_disciplina = NULL;
    arv_nota = NULL;
    arv_matricula = NULL;

    int matricula, codigo_curso, quantidade_periodos, codigo_disciplina, periodo, ch, entrada_mat, saida;

    char nome[40], nome_curso[100], nome_disciplina[100];
    float nota_final, semestre;
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
                    cadastrar_disciplina(arv_curso, codigo_curso, codigo_disciplina, nome_disciplina, periodo, ch);
                }
                break;
                case 2: // Remover Disciplina
                {
                    int codigo_curso, codigo_disciplina;
                    printf("Informe o código do curso: ");
                    scanf("%d", &codigo_curso);
                    printf("Informe o código da disciplina a remover: ");
                    scanf("%d", &codigo_disciplina);
                    remover_disciplina_curso(&arv_curso, aluno, codigo_curso, codigo_disciplina);
                }
                break;
                case 3:
                    exibir_disciplinasporcurso(arv_curso, 0);
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
                    cadastrar_curso(&arv_curso, codigo_curso, nome_curso, quantidade_periodos);
                }
                break;
                case 2:
                    // Essa função ainda não existe
                    break;
                case 3:
                    exibir_curso(arv_curso);
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

            switch (menu_caso3)
            {
            case 1:
            {
                // desativei aqui
                break;
            }
            break;
            case 2:
                // Essa função não existe
                break;
            case 3:
                printf("Digite o código do curso: ");
                scanf("%d", &codigo_curso);

                exibir_alunosporcurso(arv_curso, codigo_curso);
                break;
            case 4:
                // em breve
                break;
            case 5:
                exibir_alunos(aluno);
                break;
            case 6:
                printf("Saindo...\n");
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
                int entrada_mat, saida;
                // int cadastrar_matricula(Arv_Matricula **r, int matricula)
                printf("Digite a matrícula que voce deseja cadastrar: ");
                scanf("%d", &entrada_mat);

                saida = cadastrar_matricula(&arv_matricula, entrada_mat);
                if (saida)
                    printf("Matricula realizada com sucesso\n");
                else
                    printf("Matricula nao realizada\n");
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                exibir_matriculas(arv_matricula);
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

            switch (menu_caso5)
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
            menu_geral();
            printf("Escolha a opcao desejada: ");
            scanf("%d", &menu_caso6);

            switch (menu_caso6)
            {
            case 1:
                printf("Informe o nome do aluno: ");
                scanf("%s", nome);
                printf("Informe a matricula: ");
                scanf("%d", &matricula);
                printf("Informe o código do curso: ");
                scanf("%d", &codigo_curso);
                cadastrar_aluno(&aluno, matricula, nome, codigo_curso);
                break;
            case 2:
                printf("Informe o código do curso: ");
                scanf("%d", &codigo_curso);
                printf("Informe o nome do curso: ");
                scanf("%s", nome_curso);
                printf("Informe a quantidade de períodos: ");
                scanf("%d", &quantidade_periodos);
                cadastrar_curso(&arv_curso, codigo_curso, nome_curso, quantidade_periodos);
                break;
            case 3:
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
                cadastrar_disciplina(arv_curso, codigo_curso, codigo_disciplina, nome_disciplina, periodo, ch);
                break;
            case 4:
                printf("Digite a matrícula que voce deseja cadastrar: ");
                scanf("%d", &entrada_mat);

                saida = cadastrar_matricula(&arv_matricula, entrada_mat);
                if (saida)
                    printf("Matricula realizada com sucesso\n");
                else
                    printf("Matricula nao realizada\n");
                break;
            case 5:
                printf("Informe o semestre: ");
                scanf("%f", &semestre);
                printf("Informe a nota final: ");
                scanf("%f", &nota_final);
                printf("Informe o código da disciplina: ");
                scanf("%d", &codigo_disciplina);
                cadastrar_nota(&arv_matricula, &arv_nota, codigo_disciplina, nota_final, semestre);
                break;
            case 6:
                printf("Digite o código do curso: ");
                scanf("%d", &codigo_curso);

                exibir_alunosporcurso(arv_curso, codigo_curso);
                break;
            case 7:
                break;
            case 8:
                break;
            case 9:
                break;
            case 10:
                break;
            case 11:
                break;
            case 12:
                break;
            case 13:
                break;
            case 14:
                break;
            case 15:
                break;
            case 16:
                printf("Saindo...\n");
                break;

            default:
                break;
            }
            break;

        case 7:
            printf("Saindo...\n");
            break;

        default:
            printf("Opção inválida\n");
            break;
        }

    } while (menu_inicial != 7);

    return 0;
}