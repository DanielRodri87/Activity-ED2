#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/src.h"

void menu_geral()
{
    printf("|                      ----------                     |\n");
    printf("|--------------------- Menu Geral --------------------|\n");
    printf("|                      ----------                     |\n");
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
    int menu_inicial;

    Alunos *aluno = NULL;
    Arv_Cursos *arv_curso = NULL;
    Arv_Notas *arv_notas = NULL;
    Arv_Disciplina *arv_disciplina = NULL, *disc;
    Arv_Matricula *arv_matricula = NULL;

    int matricula, codigo_curso, quantidade_periodos, codigo_disciplina, entrada_mat, entrada_disc, saida, coddisc, input_semestre;
    float periodo, input_nota_final;
    char nome[40], nome_curso[100];

    do
    {
        menu_geral();
        printf("Escolha a opcao desejada: ");
        scanf("%d", &menu_inicial);

        switch (menu_inicial)
        {
        case 1:
            // Cadastrar Aluno
            if (arv_curso == NULL)
            {
                printf("Erro: Nenhum curso cadastrado. Cadastre um curso primeiro.\n");
                break;
            }
            printf("Informe o nome do aluno: ");
            scanf(" %[^\n]s", nome); // Permite entrada com espaços
            printf("Informe a matricula: ");
            scanf("%d", &matricula);
            printf("Informe o código do curso: ");
            scanf("%d", &codigo_curso);
            cadastrar_aluno(&aluno, matricula, nome, codigo_curso);
            break;

        case 2:
            // Cadastrar Curso
            printf("Informe o código do curso: ");
            scanf("%d", &codigo_curso);
            printf("Informe o nome do curso: ");
            scanf(" %[^\n]s", nome_curso); // Permite entrada com espaços
            printf("Informe a quantidade de períodos: ");
            scanf("%d", &quantidade_periodos);
            cadastrar_curso(&arv_curso, codigo_curso, nome_curso, quantidade_periodos);
            break;

        case 3:
            if (arv_curso == NULL)
            {
                printf("Erro: Nenhum curso cadastrado. Cadastre um curso primeiro.\n");
                break;
            }

            printf("Digite o código do curso: ");
            scanf("%d", &codigo_curso);

            Arv_Cursos *curso_encontrado = buscar_curso(arv_curso, codigo_curso);
            if (!curso_encontrado)
            {
                printf("Erro: Curso não encontrado.\n");
                break;
            }

            printf("Digite o nome da disciplina: ");
            char nome_disciplina[100];
            scanf(" %[^\n]s", nome_disciplina);

            printf("Digite a carga horaria da disciplina (30 a 90, múltiplo de 15): ");
            int carga_horaria;
            scanf("%d", &carga_horaria);

            printf("Digite o período da disciplina: ");
            scanf("%f", &periodo);

            // Gerar código para a disciplina
            int coddisc;
            gerar_codigo_disc(&coddisc);

            // Cadastrar disciplina no curso encontrado
            cadastrar_disciplina(&(curso_encontrado->info->disciplina), coddisc, nome_disciplina, periodo, carga_horaria, curso_encontrado->info->quantidade_periodos);

            printf("\nCódigo da disciplina gerado: %d\n", coddisc);
            printf("\nDisciplina cadastrada com sucesso!\n");
            break;

        case 4:
            // Cadastrar Matrícula
            if (aluno == NULL)
            {
                printf("Erro: Nenhum aluno cadastrado. Cadastre um aluno primeiro.\n");
                break;
            }
            printf("Digite a matrícula da disciplina: ");
            scanf("%d", &entrada_disc);
            printf("Digite a matrícula do aluno: ");
            scanf("%d", &entrada_mat);

            cadastrar_matricula(&aluno, entrada_disc, entrada_mat);
            break;

        case 5:
            // Cadastrar Nota

            Notas_Info *n = (Notas_Info*) malloc(sizeof(Notas_Info));

            if (aluno == NULL)
            {
                printf("Erro: Nenhum aluno cadastrado. Cadastre um aluno primeiro.\n");
                break;
            }
            printf("Digite a matrícula do aluno: ");
            scanf("%d", &matricula);
            printf("Digite o código da disciplina: ");
            scanf("%d", &codigo_disciplina);
            printf("Digite o semestre (número inteiro): ");
            scanf("%d", &input_semestre);
            printf("Digite a nota final: ");
            scanf("%f", &input_nota_final);

            n->codigo_disciplina = codigo_disciplina;
            n->semestre = input_semestre;
            n->nota_final = input_nota_final;

            saida = cadastrar_notas(&aluno, matricula, n);
            if (saida == 1)
                printf("Nota cadastrada com sucesso e disciplina removida da árvore de matrículas.\n");
            else
                printf("Erro: Não foi possível cadastrar a nota. Verifique se a disciplina está matriculada.\n");

            break;

        case 6:
            // Mostrar Alunos de um Curso
            if (aluno == NULL)
            {
                printf("Erro: Nenhum aluno cadastrado.\n");
                break;
            }
            printf("Digite o código do curso: ");
            scanf("%d", &codigo_curso);
            alunosporcurso(aluno, codigo_curso);
            break;

        case 7:
            // Mostrar Cursos do Campus
            if (arv_curso == NULL)
            {
                printf("Erro: Nenhum curso cadastrado.\n");
                break;
            }
            exibir_curso(arv_curso);
            break;

        case 8:
            // Mostrar Disciplinas de um Curso
            if (arv_curso == NULL)
            {
                printf("Erro: Nenhum curso cadastrado.\n");
                break;
            }
            printf("Digite o código do curso: ");
            scanf("%d", &codigo_curso);
            exibir_disciplinasporcurso(arv_curso, codigo_curso);
            break;

        case 9:
            // Mostrar Disciplinas de um Período
            if (arv_curso == NULL)
            {
                printf("Erro: Nenhum curso cadastrado.\n");
                break;
            }
            printf("Digite o período desejado: ");
            scanf("%f", &periodo);
            exibir_disciplina_periodo(arv_curso, periodo);
            break;

        case 10:
            // Mostrar Disciplinas Matriculadas
            if (aluno == NULL)
            {
                printf("Erro: Nenhum aluno cadastrado.\n");
                break;
            }
            printf("Digite a matrícula do aluno: ");
            scanf("%d", &matricula);
            exibir_disciplinasporaluno(aluno, arv_curso, matricula);
            break;

        case 11:
            // Mostrar Notas por Período
            if (aluno == NULL)
            {
                printf("Erro: Nenhum aluno cadastrado.\n");
                break;
            }
            printf("Digite matricula: ");
            scanf("%d", &matricula);
            printf("Digite o período: ");
            scanf("%f", &periodo);

            notas_disciplina_periodo_aluno(aluno, periodo, matricula);
            break;

        case 12:
            // Mostrar Nota de uma Disciplina
            if (aluno == NULL)
            {
                printf("Erro: Nenhum aluno cadastrado.\n");
                break;
            }
            printf("Digite a matrícula do aluno: ");
            scanf("%d", &matricula);
            printf("Digite o código da disciplina: ");
            scanf("%d", &codigo_disciplina);
        // case 13:
        //     // Remover Disciplina de Curso
        //     if (arv_curso == NULL)
        //     {
        //         printf("Erro: Nenhum curso cadastrado.\n");
        //         break;
        //     }
        //     printf("Digite o código do curso: ");
        //     scanf("%d", &codigo_curso);
        //     printf("Digite o código da disciplina a ser removida: ");
        //     scanf("%d", &codigo_disciplina);

        //     saida = remover_disciplina_curso(&arv_curso, aluno, codigo_curso, codigo_disciplina);

        //     if (saida)
        //     {
        //         printf("Disciplina removida com sucesso do curso.\n");
        //     }
        //     else
        //     {
        //         printf("Erro: Não foi possível remover a disciplina. Verifique se o curso e a disciplina estão cadastrados.\n");
        //     }
        //     break;

        case 14:
            // Remover Disciplina da Matrícula
            if (aluno == NULL)
            {
                printf("Erro: Nenhum aluno cadastrado.\n");
                break;
            }
            Alunos *aluno_encontrado = buscar_aluno(aluno, matricula);

            if (aluno_encontrado == NULL)
            {
                printf("Erro: Aluno não encontrado.\n");
            }
            else
            {
                printf("Digite o código da disciplina que deseja remover: ");
                scanf("%d", &codigo_disciplina);

                remover_matricula(&aluno_encontrado->mat, codigo_disciplina);
                printf("Disciplina removida da matrícula do aluno.\n");
            }
            break;

        case 15:
            // Mostrar Histórico do Aluno
            if (aluno == NULL)
            {
                printf("Erro: Nenhum aluno cadastrado.\n");
                break;
            }
            printf("Digite a matrícula do aluno: ");
            scanf("%d", &matricula);

            consultar_historico(aluno, arv_curso, matricula);
            break;

        case 16:
            // Voltar
            printf("Retornando ao menu anterior...\n");
            break;

        default:
            printf("Opção inválida! Tente novamente.\n");
            break;
        }
    } while (menu_inicial != 16);

    return 0;
}
