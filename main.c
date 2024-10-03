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
    Arv_Disciplina *arv_disciplina = NULL;
    Arv_Notas *arv_nota = NULL;
    Arv_Matricula *arv_matricula = NULL;

    int matricula, codigo_curso, quantidade_periodos, codigo_disciplina, periodo, ch, entrada_mat, saida;
    float nota_final, semestre;
    char nome[40], nome_curso[100], nome_disciplina[100];

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
            cadastrar_aluno(arv_curso, matricula, nome, codigo_curso);
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
            // Cadastrar Disciplina
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
            // Cadastrar Matrícula
            printf("Digite a matrícula que voce deseja cadastrar: ");
            scanf("%d", &entrada_mat);

            saida = cadastrar_matricula(&arv_matricula, entrada_mat);
            if (saida)
                printf("Matricula realizada com sucesso\n");
            else
                printf("Matricula nao realizada\n");
            break;

        case 5:
            printf("Informe a matrícula do aluno: ");
            scanf("%d", &matricula);

            Alunos *current_aluno = buscar_aluno_por_matricula(aluno, matricula);
            if (current_aluno == NULL)
            {
                printf("Aluno não encontrado!\n");
                break;
            }

            printf("Informe o código da disciplina: ");
            scanf("%d", &codigo_disciplina);
            printf("Informe o semestre: ");
            scanf("%f", &semestre); 
            printf("Informe a nota final: ");
            scanf("%f", &nota_final); 

            if (current_aluno->notas == NULL)
            {
                current_aluno->notas = (Arv_Notas *)malloc(sizeof(Arv_Notas));
                if (current_aluno->notas == NULL)
                {
                    printf("Erro ao alocar memória para notas.\n");
                    break;
                }
                
                current_aluno->notas->esq = NULL;
                current_aluno->notas->dir = NULL;
            }

            // Cadastrar nota
            if (cadastro_nota(&current_aluno, matricula, codigo_disciplina, semestre, nota_final))
            {
                printf("Nota cadastrada com sucesso.\n");
            }
            else
                printf("Erro ao cadastrar a nota. Verifique se a disciplina está matriculada.\n");
            break;

        case 6:
            // Mostrar Alunos de um Curso
            printf("Digite o código do curso: ");
            scanf("%d", &codigo_curso);
            exibir_alunosporcurso(arv_curso, codigo_curso);
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
            scanf("%d", &periodo);
            exibir_disciplina_periodo(arv_curso, periodo);
            break;
        case 10:
            // Mostrar Disciplinas Matriculadas
            printf("Digite a matrícula do aluno: ");
            scanf("%d", &matricula);
            exibir_disciplinasporaluno(arv_curso, matricula);
            break;
        case 11:
            // Mostrar Notas por Período
            printf("Digite o período desejado: ");
            scanf("%d", &periodo);
            exibir_notas_periodo(arv_nota, arv_disciplina, periodo);
            break;
        case 12:
            // Mostrar Nota de uma Disciplina
            printf("Digite a matrícula do aluno: ");
            scanf("%d", &matricula);
            printf("Digite o código da disciplina: ");
            scanf("%d", &codigo_disciplina);
            exibir_notadisciplina(aluno, arv_curso, matricula, codigo_disciplina);
            break;
        case 13:
            // Remover Disciplina de Curso
            printf("Digite o código do curso: ");
            scanf("%d", &codigo_curso);
            printf("Digite o código da disciplina que deseja remover: ");
            scanf("%d", &codigo_disciplina);
            remover_disciplina_curso(&arv_curso, aluno, codigo_curso, codigo_disciplina);
            break;
        case 14:
            // Remover Disciplina da Matrícula
            printf("Digite o código da disciplina que deseja remover: ");
            scanf("%d", &codigo_disciplina);
            remover_disciplinaaluno(&arv_matricula, codigo_disciplina);
            break;
        case 15:
            // Mostrar Histórico do Aluno
            printf("Digite a matrícula do aluno: ");
            scanf("%d", &matricula);
            buscar_aluno_xv(aluno, matricula, arv_curso);
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
