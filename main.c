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

    int matricula, codigo_curso, quantidade_periodos, codigo_disciplina, ch, entrada_mat, entrada_disc, saida;
    float nota_final, semestre, periodo;
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
            scanf("%f", &periodo);
            printf("Informe a carga horária: ");
            scanf("%d", &ch);
            cadastrar_disciplina(arv_curso, codigo_curso, codigo_disciplina, nome_disciplina, periodo, ch);
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
            // Cadastrar nota
            printf("Digite a matricula do aluno: ");
            scanf("%d", &matricula);

            printf("Digite o codigo da disciplina: ");
            scanf("%d", &entrada_disc);

            // Garantir que 'aluno' está alocado e que 'notas' está alocado
            if (aluno == NULL)
                aluno = (Alunos *)malloc(sizeof(Alunos));


            if (aluno->notas == NULL)
                aluno->notas = (Arv_Notas *)malloc(sizeof(Arv_Notas));
            

            printf("Digite o semestre: ");
            scanf("%f", &aluno->notas->semestre);

            printf("Digite a nota final: ");
            scanf("%f", &aluno->notas->nota_final);

            // Função de cadastro de nota
            int saida = cadastrar_nota(&aluno, matricula, entrada_disc, aluno->notas->semestre, aluno->notas->nota_final);
            if (saida == 1)
                printf("MAtricula cadastrada com sucesso");

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
            scanf("%f", &periodo);
            exibir_disciplina_periodo(arv_curso, periodo);
            break;
        case 10:
            // Mostrar Disciplinas Matriculadas
            printf("Digite a matrícula do aluno: ");
            scanf("%d", &matricula);
            exibir_disciplinasporaluno(arv_curso, matricula);
            break;
        case 11:
            printf("Digite matricula: ");
            scanf("%d", &matricula);

            printf("Digite o período: ");
            scanf("%f", &periodo);

            notas_disciplina_periodo_aluno(aluno, periodo, matricula);
            break;

        case 12:
            printf("Digite o código do curso: ");
            scanf("%d", &codigo_curso);
            printf("Digite a matrícula do aluno: ");
            scanf("%d", &matricula);
            printf("Digite o código da disciplina: ");
            scanf("%d", &codigo_disciplina);

            exibir_nota_aluno_disciplina(arv_curso, codigo_curso, matricula, codigo_disciplina);
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