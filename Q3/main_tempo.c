#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/src.h"
#include <time.h>

#define QUANTIDADECURSOS 2000
#define CODIGOCURSO 2000
#define LOOP 15000
#define QUANTIDADEDISCIPLINAS 2000
#define QUANTIDADEALUNOS 2000

// 1. inserção de cada elemento na árvore de cursos

// forma crescente
void povoamentocrescente_cursos(Arv_Cursos **raiz)
{
    for (int i = 1; i <= QUANTIDADECURSOS; i++)
    {
        cadastrar_curso(raiz, i, "MEDICINA", 12);
    }
}

// forma descrecente
void povoamentodescrecente_cursos(Arv_Cursos **raiz)
{
    for (int i = QUANTIDADECURSOS; i > 0; i--)
    {
        cadastrar_curso(raiz, i, "MEDICINA", 12);
    }
}

// forma aleatória
void povoamentoaleatorio_cursos(Arv_Cursos **raiz)
{
    int i = 1;
    while (i <= QUANTIDADECURSOS)
    {
        int codigo_curso = rand() % QUANTIDADECURSOS + 1;
        cadastrar_curso(raiz, codigo_curso, "MEDICINA", 12);
        i++;
    }
}


// 2. busca de uma nota de uma disciplina de uma aluno

// forma crescente
void povoamentocrescente_disciplinas(Arv_Cursos **raiz)
{
    if (*raiz != NULL)
    {
        int codigo_curso_atual = (*raiz)->info->codigo_curso;

        for (int i = 1; i < QUANTIDADEDISCIPLINAS; i++)
        {
            Arv_Disciplina *disciplina = (Arv_Disciplina *)malloc(sizeof(Arv_Disciplina));
            strcpy(disciplina->info->nome_disciplina, "Anatomia");
            disciplina->info->carga_horaria = 90;
            disciplina->info->periodo = 1;
            disciplina->info->codigo_disciplina = i;

            cadastrar_disciplina(&(*raiz)->info->disciplina, disciplina->info->codigo_disciplina, disciplina->info->nome_disciplina, disciplina->info->periodo, disciplina->info->carga_horaria, 12);
        }

        povoamentocrescente_disciplinas(&(*raiz)->esq);
        povoamentocrescente_disciplinas(&(*raiz)->dir);
    }
}

void povoamentocrescente_matriculas(Alunos *aluno)
{
    if (aluno != NULL)
    {
        for (int i = 1; i < QUANTIDADEDISCIPLINAS; i++)
        {
            int codigo_disciplina = i;
            int numero_matricula = aluno->matricula;

            cadastrar_matricula(&aluno, codigo_disciplina, numero_matricula);
        }

        povoamentocrescente_matriculas(aluno->prox);
    }
}

void povoamentocrescente_notas(Alunos *aluno)
{
    if (aluno != NULL)
    {
        for (int i = 1; i < QUANTIDADECURSOS; i++)
        {
            Notas_Info *n = (Notas_Info*) malloc(sizeof(Notas_Info));
            n->codigo_disciplina = i;
            n->semestre = 3;
            n->nota_final = (rand() % 10) + 1;

            cadastrar_notas(&aluno, 2023, n);

        }

        povoamentocrescente_notas(aluno->prox);
    }
}

void povoamentocrescente_alunos(Alunos **aluno, Arv_Cursos *curso)
{
    char *nomes[5] = {"Eleven", "Mike", "Dustin", "Lucas", "Max"};

    for (int i = 1; i < QUANTIDADEALUNOS; i++)
    {
        int matricula = i;
        char *nome = nomes[rand() % 5];
        int codigo_curso = rand() % QUANTIDADECURSOS;

        cadastrar_aluno(aluno, matricula, nome, codigo_curso);
    }
}

// forma descrecente
void povoamentodecrescente_disciplinas(Arv_Cursos **raiz)
{
    if (*raiz != NULL)
    {
        int codigo_curso_atual = (*raiz)->info->codigo_curso;

        for (int i = QUANTIDADEDISCIPLINAS; i > 1; i--)
        {
            Arv_Disciplina *disciplina = (Arv_Disciplina *)malloc(sizeof(Arv_Disciplina));
            strcpy(disciplina->info->nome_disciplina, "Anatomia");
            disciplina->info->carga_horaria = 90;
            disciplina->info->periodo = 1;
            disciplina->info->codigo_disciplina = i;
            disciplina->esq = NULL;
            disciplina->dir = NULL;

            cadastrar_disciplina(&(*raiz)->info->disciplina, disciplina->info->codigo_disciplina, disciplina->info->nome_disciplina, disciplina->info->periodo, disciplina->info->carga_horaria, 12);
        }

        povoamentodecrescente_disciplinas(&(*raiz)->esq);
        povoamentodecrescente_disciplinas(&(*raiz)->dir);
    }
}

void povoamentodecrescente_matriculas(Alunos *aluno)
{
    if (aluno != NULL)
    {
        for (int i = QUANTIDADEDISCIPLINAS; i > 1; i--)
        {
            int codigo_disciplina = i;
            int numero_matricula = aluno->matricula;

            cadastrar_matricula(&aluno, codigo_disciplina, numero_matricula);
        }

        povoamentodecrescente_matriculas(aluno->prox);
    }
}

void povoamentodecrescente_notas(Alunos *aluno)
{
    if (aluno != NULL)
    {
        for (int i = QUANTIDADECURSOS; i > 1; i--)
        {
            Notas_Info *n = (Notas_Info*) malloc(sizeof(Notas_Info));
            n->codigo_disciplina = i;
            n->semestre = 3;
            n->nota_final = (rand() % 10) + 1;

            cadastrar_notas(&aluno, 2023, n);
        }

        povoamentodecrescente_notas(aluno->prox);
    }
}

void povoamentodecrescente_alunos(Alunos **aluno, Arv_Cursos *curso)
{
    char *nomes[5] = {"Eleven", "Mike", "Dustin", "Lucas", "Max"};

    for (int i = QUANTIDADEALUNOS; i > 1; i--)
    {
        int matricula = i;
        char *nome = nomes[rand() % 5];
        int codigo_curso = rand() % QUANTIDADECURSOS;

        cadastrar_aluno(aluno, matricula, nome, codigo_curso);
    }
}

// forma aleatória
void povoar_disciplinas_aleatorio(Arv_Cursos **raiz)
{
    if (*raiz != NULL)
    {
        int i = 1, sucesso = 0;
        while (i < QUANTIDADEDISCIPLINAS)
        {
            Arv_Disciplina *disciplina = (Arv_Disciplina *)malloc(sizeof(Arv_Disciplina));
            strcpy(disciplina->info->nome_disciplina, "Anatomia");
            disciplina->info->carga_horaria = 90;
            disciplina->info->periodo = 1;
            disciplina->info->codigo_disciplina = rand() % QUANTIDADEDISCIPLINAS;
            disciplina->esq = NULL;
            disciplina->dir = NULL;


            cadastrar_disciplina(&(*raiz)->info->disciplina, disciplina->info->codigo_disciplina, disciplina->info->nome_disciplina, disciplina->info->periodo, disciplina->info->carga_horaria, 12);
            i++;
        }

        povoar_disciplinas_aleatorio(&(*raiz)->esq);
        povoar_disciplinas_aleatorio(&(*raiz)->dir);
    }
}

void povoar_matriculas_aleatorio(Alunos *aluno)
{
    if (aluno != NULL)
    {
        int i = 1, sucesso = 0;
        while (i < QUANTIDADEDISCIPLINAS)
        {
            int codigo_disciplina = rand() % QUANTIDADEDISCIPLINAS;
            int numero_matricula = aluno->matricula;

            cadastrar_matricula(&aluno, codigo_disciplina, numero_matricula);
            i++;
        }

        povoar_matriculas_aleatorio(aluno->prox);
    }
}

void povoar_notas_aleatorio(Alunos *aluno)
{
    if (aluno != NULL)
    {
        int i = 1, sucesso = 0;
        while (i < QUANTIDADECURSOS)
        {
            int semestre = 3;
            float nota_final = (rand() % 10) + 1;
            int matricula = aluno->matricula;

            Notas_Info *n = (Notas_Info*) malloc(sizeof(Notas_Info));
            n->codigo_disciplina = rand() % QUANTIDADEDISCIPLINAS;
            n->semestre = 3;
            n->nota_final = (rand() % 10) + 1;

            if (cadastrar_notas(&aluno, 2023, n) == 1)
                i++;
        }

        povoar_notas_aleatorio(aluno->prox);
    }
}

void povoar_alunos_aleatorio(Alunos **aluno, Arv_Cursos *curso)
{
    char *nomes[5] = {"Eleven", "Mike", "Dustin", "Lucas", "Max"};
    int i = 0;

    while (i < QUANTIDADEALUNOS)
    {
        int matricula = rand() % QUANTIDADEALUNOS;
        char *nome = nomes[rand() % 5];
        int codigo_curso = rand() % QUANTIDADECURSOS;

        cadastrar_aluno(aluno, matricula, nome, codigo_curso);
        i++;
    }
}

// 3. métricas de tempo

// por inserção na árvore de cursos
double tempomedio_insercao_cursos(Arv_Cursos **curso)
{
    clock_t inicio, fim;
    double tempo_total = 0.0, tempomedio_insercao = 0.0;

    int i;

    for (i = 0; i < LOOP; i++)
    {
        int codigo_curso = rand() % CODIGOCURSO + 1;
        const char *nome_curso = "MEDICINA";
        int quantidade_periodos = (rand() % 9) + 4;

        inicio = clock();
        cadastrar_curso(curso, codigo_curso, nome_curso, quantidade_periodos);
        fim = clock();

        tempo_total += ((double)(fim - inicio)) / CLOCKS_PER_SEC;
        tempomedio_insercao = tempo_total / LOOP;
    }

    return tempomedio_insercao;
}

// por busca na árvore de notas
double tempomedio_busca(Alunos *alunos, Arv_Cursos *raiz, int matricula, int codigo_disciplina)
{
    clock_t inicio, fim;
    double tempo_total = 0.0, tempomedio_busca = 0.0;

    for (int i = 0; i < LOOP; i++)
    {
        inicio = clock();

        exibir_nota_aluno_disciplina(alunos, raiz, matricula, codigo_disciplina);

        fim = clock();

        tempo_total += ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    }

    tempomedio_busca = tempo_total / LOOP;

    return tempomedio_busca;
}

// funções free
void free_arvore_cursos(Arv_Cursos *raiz)
{
    if (raiz != NULL)
    {
        free_arvore_cursos(raiz->esq);
        free_arvore_cursos(raiz->dir);
        free(raiz);
    }
}

void free_lista_alunos(Alunos *alunos)
{
    Alunos *temp;
    while (alunos != NULL)
    {
        temp = alunos;
        alunos = alunos->prox;
        free(temp);
    }
}

// 4. int main
int main()
{
    Arv_Cursos *raiz = NULL;
    Alunos *alunos = NULL;
    int opcao, codigo_curso;

    srand(time(NULL));

    do
    {
        printf("\nMenu:\n");
        printf("1. Inserir cursos em ordem crescente\n");
        printf("2. Inserir cursos em ordem decrescente\n");
        printf("3. Inserir cursos de forma aleatória\n");
        printf("4. Inserir disciplinas em ordem crescente\n");
        printf("5. Inserir disciplinas em ordem decrescente\n");
        printf("6. Inserir disciplinas de forma aleatória\n");
        printf("7. Inserir alunos em ordem crescente\n");
        printf("8. Inserir alunos em ordem decrescente\n");
        printf("9. Inserir alunos de forma aleatória\n");
        printf("10. Buscar nota de disciplina\n");
        printf("11. Calcular tempo médio de inserção de cursos\n");
        printf("12. Calcular tempo médio de busca de notas\n");
        printf("13. Conferir insercao curso\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            povoamentocrescente_cursos(&raiz);
            printf("Cursos inseridos em ordem crescente.\n");
            break;
        case 2:
            povoamentodescrecente_cursos(&raiz);
            printf("Cursos inseridos em ordem decrescente.\n");
            break;
        case 3:
            povoamentoaleatorio_cursos(&raiz);
            printf("Cursos inseridos de forma aleatória.\n");
            break;
        case 4:
            povoamentocrescente_disciplinas(&raiz);
            printf("Disciplinas inseridas em ordem crescente.\n");
            break;
        case 5:
            povoamentodecrescente_disciplinas(&raiz);
            printf("Disciplinas inseridas em ordem decrescente.\n");
            break;
        case 6:
            povoar_disciplinas_aleatorio(&raiz);
            printf("Disciplinas inseridas de forma aleatória.\n");
            break;
        case 7:
            povoamentocrescente_alunos(&alunos, raiz);
            printf("Alunos inseridos em ordem crescente.\n");
            break;
        case 8:
            povoamentodecrescente_alunos(&alunos, raiz);
            printf("Alunos inseridos em ordem decrescente.\n");
            break;
        case 9:
            povoar_alunos_aleatorio(&alunos, raiz);
            printf("Alunos inseridos de forma aleatória.\n");
            break;
        case 10:
        {
            int matricula, codigo_disciplina;
            printf("Informe a matrícula do aluno: ");
            scanf("%d", &matricula);
            printf("Informe o código da disciplina: ");
            scanf("%d", &codigo_disciplina);
            double tempo = tempomedio_busca(alunos, raiz, matricula, codigo_disciplina);
            printf("Tempo médio de busca: %.6f segundos\n", tempo);
            break;
        }
        case 11:
        {
            double tempo = tempomedio_insercao_cursos(&raiz);
            printf("Tempo médio de inserção de cursos: %.6f segundos\n", tempo);
            break;
        }
        case 12:
        {
            int matricula, codigo_disciplina;
            printf("Informe a matrícula do aluno: ");
            scanf("%d", &matricula);
            printf("Informe o código da disciplina: ");
            scanf("%d", &codigo_disciplina);
            double tempo = tempomedio_busca(alunos, raiz, matricula, codigo_disciplina);
            printf("Tempo médio de busca de notas: %.6f segundos\n", tempo);
            break;
        }
        case 13:
            // Mostrar Cursos do Campus
            if (raiz == NULL)
            {
                printf("Erro: Nenhum curso cadastrado.\n");
                break;
            }
            exibir_curso(raiz);
            break;
            break;
        case 0:
            printf("PROGRAMA FINALIZADO.\n");
            break;
        default:
            printf("Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 0);

    free_arvore_cursos(raiz);
    free_lista_alunos(alunos);

    return 0;
}