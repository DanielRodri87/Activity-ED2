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

    int matricula, codigo_curso, codigo_curso_teste, quantidade_periodos, codigo_disciplina, entrada_mat, entrada_disc, saida, coddisc;
    float periodo;
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
            printf("Informe o nome do aluno: ");
            scanf("%s", nome);
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
            scanf("%s", nome_curso);
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

            disc = (Arv_Disciplina *)malloc(sizeof(Arv_Disciplina));
            if (!disc)
            {
                printf("Erro ao alocar memória para disciplina.\n");
                break;
            }

            printf("Digite o nome da disciplina: ");
            scanf("%s", disc->nome_disciplina);
            printf("Digite o código do curso: ");
            scanf("%d", &codigo_curso);
            printf("Digite a carga horaria da disciplina: ");
            scanf("%d", &disc->carga_horaria);
            printf("Digite o periodo da disciplina: ");
            scanf("%d", &disc->periodo);

            // Verificar se o curso existe
            Arv_Cursos *curso_encontrado = buscar_curso(arv_curso, codigo_curso); // Função para buscar curso
            if (!curso_encontrado)
            {
                printf("Erro: Curso não encontrado.\n");
                free(disc);
                break;
            }

            gerar_codigo_disc(&coddisc);
            disc->codigo_disciplina = coddisc;

            // Tentar cadastrar a disciplina
            saida = cadastrar_disciplina(&curso_encontrado, disc, codigo_curso);
            if (saida)
            {
                printf("\nCódigo da disciplina gerado: %d\n", coddisc);
                printf("\nDisciplina cadastrada com sucesso!\n");
            }
            else
            {
                printf("Erro ao cadastrar disciplina.\n");
                free(disc);
            }
            break;

        case 4:
            // Cadastrar Matrícula
            printf("Digite a matrícula da disciplina: ");
            scanf("%d", &entrada_disc);
            printf("Digite a matrícula do aluno: ");
            scanf("%d", &entrada_mat);

            cadastrar_matricula(&aluno, entrada_disc, entrada_mat);
            break;

        case 5:
            printf("Digite a matrícula do aluno: ");
            scanf("%d", &matricula);

            printf("Digite o código da disciplina: ");
            scanf("%d", &codigo_disciplina);

            printf("Digite o semestre (número inteiro): ");
            scanf("%d", &quantidade_periodos);

            printf("Digite a nota final: ");
            scanf("%f", &periodo);

            // Tentar cadastrar a nota
            saida = cadastrar_nota(&aluno, matricula, codigo_disciplina, quantidade_periodos, periodo);

            if (saida == 1)
            {
                printf("Nota cadastrada com sucesso e disciplina removida da árvore de matrículas.\n");
            }
            else
            {
                printf("Erro: Não foi possível cadastrar a nota. Verifique se a disciplina está matriculada.\n");
            }
            break;

        case 6:
            // Mostrar Alunos de um Curso
            printf("Digite o código do curso: ");
            scanf("%d", &codigo_curso);
            alunosporcurso(aluno, codigo_curso);
            break;
        case 7:
            // Mostrar Cursos do Campus
            exibir_curso(arv_curso);
            break;
        case 8:
            // Mostrar Disciplinas de um Curso
            printf("Digite o código do curso: ");
            scanf("%d", &codigo_curso);
            exibir_disciplinasporcurso(arv_curso, codigo_curso);
            break;
        case 9:
            // Mostrar Disciplinas de um Período
            printf("Digite o período desejado: ");
            scanf("%f", &periodo);
            exibir_disciplina_periodo(arv_curso, periodo);
            break;
        case 10:
            // Mostrar Disciplinas Matriculadas
            printf("Digite a matrícula do aluno: ");
            scanf("%d", &matricula);
            exibir_disciplinasporaluno(aluno, arv_curso, matricula);
            break;
        case 11:
            printf("Digite matricula: ");
            scanf("%d", &matricula);
            printf("Digite o período: ");
            scanf("%f", &periodo);

            notas_disciplina_periodo_aluno(aluno, periodo, matricula);
            break;

        case 12:
            printf("Digite a matrícula do aluno: ");
            scanf("%d", &matricula);
            printf("Digite o código da disciplina: ");
            scanf("%d", &codigo_disciplina);

            exibir_nota_aluno_disciplina(aluno, arv_curso, matricula, codigo_disciplina);
            break;
        case 13:
            // Remover Disciplina de Curso
            printf("Digite o código do curso: ");
            scanf("%d", &codigo_curso);

            printf("Digite o código da disciplina que deseja remover: ");
            scanf("%d", &codigo_disciplina);

            Arv_Cursos *curso_encontrado_xiii = buscar_curso(arv_curso, codigo_curso);
            if (curso_encontrado_xiii == NULL)
            {
                printf("Erro: Curso não encontrado.\n");
                break;
            }

            int alunos_matriculados = verificar_matriculas_alunos(curso_encontrado->alunos, codigo_disciplina);

            if (alunos_matriculados)
                printf("Erro: Não é possível remover a disciplina. Há alunos matriculados nela.\n");

            else
            {
                int removido = remover_disciplina_curso(&(curso_encontrado->disciplina), codigo_disciplina);
                if (removido)
                    printf("Disciplina removida com sucesso.\n");
                else
                    printf("Erro: Disciplina não encontrada no curso.\n");
            }
            break;
        case 14:
            printf("Digite a matrícula do aluno: ");
            scanf("%d", &matricula);

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
            printf("Digite a matrícula do aluno: ");
            scanf("%d", &matricula);
            // buscar_aluno_xv(aluno, matricula, arv_curso);
            break;
        case 16:
            // Sair do programa
            printf("Saindo...\n");
            break;
        default:
            printf("Opcao invalida! Tente novamente.\n");
            break;
        }
    } while (menu_inicial != 16);

    return 0;
}