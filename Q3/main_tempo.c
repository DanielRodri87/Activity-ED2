#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/src.h"
#include <time.h>

#define QUANTIDADECURSOS 2000
#define CODIGOCURSO 2000
#define LOOP 2000
#define QUANTIDADEDISCIPLINAS 2000
#define QUANTIDADEALUNOS 2000
#define QUANTIDADENOTAS 2000

// --------------------------------------  POVOAMENTO CURSOS ---------------------------------

// 1. inserção de cada elemento na árvore de cursos

// forma crescente

void povoamentocrescente_cursos(Arv_Cursos **raiz)
{
    *raiz = NULL;

    for (int i = 1; i <= QUANTIDADECURSOS; i++)
    {
        Cursos_Info *entrada_curso;
        entrada_curso = (Cursos_Info *)malloc(sizeof(Cursos_Info));
        entrada_curso->codigo_curso = i;
        strcpy(entrada_curso->nome_curso, "MEDICINA");
        entrada_curso->quantidade_periodos = 12;

        cadastrar_curso(raiz, entrada_curso);
    }
}

void povoamentodescrecente_cursos(Arv_Cursos **raiz)
{
    *raiz = NULL;



    for (int i = QUANTIDADECURSOS; i >= 1; i--)
    {
        Cursos_Info *entrada_curso;
        entrada_curso = (Cursos_Info *)malloc(sizeof(Cursos_Info));
        entrada_curso->codigo_curso = i;
        strcpy(entrada_curso->nome_curso, "MEDICINA");
        entrada_curso->quantidade_periodos = 12;

        cadastrar_curso(raiz, entrada_curso);
    }
}

// forma aleatória
void povoamentoaleatorio_cursos(Arv_Cursos **raiz)
{
    *raiz = NULL;

    int i = 1;
    while (i <= QUANTIDADECURSOS)
    {
        Cursos_Info *entrada_curso;
        entrada_curso = (Cursos_Info *)malloc(sizeof(Cursos_Info));

        entrada_curso->codigo_curso = rand() % QUANTIDADECURSOS + 1;
        strcpy(entrada_curso->nome_curso, "MEDICINA");
        entrada_curso->quantidade_periodos = 12;

        if (cadastrar_curso(raiz, entrada_curso) == 1)
            i++;
    }
}

// -----------------------------------  POVOAMENTO DISCIPLINAS -------------------------------

// 2. busca de uma nota de uma disciplina de uma aluno

// -----------------------------------  POVOAMENTO MATRICULAS --------------------------------

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

void povoamentoaleatorio_matriculas(Alunos *aluno)
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

        povoamentoaleatorio_matriculas(aluno->prox);
    }
}

// -------------------------------------  POVOAMENTO NOTAS -----------------------------------
void povoamentocrescente_notas(Alunos *aluno)
{
    if (aluno != NULL)
    {
        for (int i = 1; i < QUANTIDADECURSOS; i++)
        {
            int codigo_disciplina = i;
            int semestre = 3;
            float nota_final = (rand() % 10) + 1;
            // printf("Nota final: %.2f\n", nota_final);
            int matricula = aluno->matricula;

            cadastrar_matricula(&aluno, codigo_disciplina, matricula);

            Notas_Info *notas_info;
            notas_info = (Notas_Info *)malloc(sizeof(Notas_Info));

            notas_info->codigo_disciplina = codigo_disciplina;
            notas_info->nota_final = nota_final;
            notas_info->semestre = semestre;

            cadastrar_notas(&aluno, matricula, notas_info);
        }

        povoamentocrescente_notas(aluno->prox);
    }
}

void exibir_notas(Arv_Notas *raiz)
{
    if (raiz != NULL)
    {
        printf("Nota final = %.2f", raiz->info->nota_final);
        exibir_notas(raiz->esq);
        exibir_notas(raiz->dir);
    }
}

void povoamentodecrescente_notas(Alunos *aluno)
{
    if (aluno != NULL)
    {
        for (int i = QUANTIDADECURSOS; i > 1; i--)
        {
            int codigo_disciplina = i;
            int semestre = 3;
            float nota_final = (rand() % 10) + 1;
            int matricula = aluno->matricula;

            cadastrar_matricula(&aluno, codigo_disciplina, matricula);

            Notas_Info *notas_info;
            notas_info = (Notas_Info *)malloc(sizeof(Notas_Info));

            notas_info->codigo_disciplina = codigo_disciplina;
            notas_info->nota_final = nota_final;
            notas_info->semestre = semestre;

            cadastrar_notas(&aluno, matricula, notas_info);
        }

        povoamentodecrescente_notas(aluno->prox);
    }
}

void povoar_notas_aleatorio(Alunos *aluno)
{
    if (aluno != NULL)
    {
        int i = 1, sucesso = 0;
        while (i < QUANTIDADECURSOS)
        {
            int codigo_disciplina = rand() % QUANTIDADEDISCIPLINAS;
            int semestre = 3;
            float nota_final = (rand() % 10) + 1;
            int matricula = aluno->matricula;

            cadastrar_matricula(&aluno, codigo_disciplina, matricula);

            Notas_Info *notas_info;
            notas_info = (Notas_Info *)malloc(sizeof(Notas_Info));

            notas_info->codigo_disciplina = codigo_disciplina;
            notas_info->nota_final = nota_final;
            notas_info->semestre = semestre;

            if (cadastrar_notas(&aluno, matricula, notas_info) == 1)
                i++;
        }

        povoar_notas_aleatorio(aluno->prox);
    }
}

// -----------------------------------  POVOAMENTO DISCIPLINAS -------------------------------
int codigo_disciplina_global_crescente = 1;
void povoamentocrescente_disciplinas(Arv_Cursos **raiz)
{
    if (*raiz != NULL)
    {
        Arv_Disciplina *variavel_disciplina = (Arv_Disciplina *)malloc(sizeof(Arv_Disciplina));
        strcpy(variavel_disciplina->info->nome_disciplina, "Algoritmos e Estruturas de Pacientes");
        variavel_disciplina->info->carga_horaria = 60;
        variavel_disciplina->info->periodo = 3;
        variavel_disciplina->info->codigo_disciplina = codigo_disciplina_global_crescente++;

        cadastrar_disciplina(raiz, variavel_disciplina->info, (*raiz)->info->codigo_curso);

        // Continua o povoamento nos subárvores
        povoamentocrescente_disciplinas(&(*raiz)->esq);
        povoamentocrescente_disciplinas(&(*raiz)->dir);
    }
}

int codigo_disciplina_global_decrescente = QUANTIDADEDISCIPLINAS;

void povoamentodecrescente_disciplinas(Arv_Cursos **raiz)
{
    if (*raiz != NULL)
    {
        Arv_Disciplina *variavel_disciplina = (Arv_Disciplina *)malloc(sizeof(Arv_Disciplina));
        strcpy(variavel_disciplina->info->nome_disciplina, "Algoritmos e Estruturas de Pacientes");
        variavel_disciplina->info->carga_horaria = 60;
        variavel_disciplina->info->periodo = 3;
        variavel_disciplina->info->codigo_disciplina = codigo_disciplina_global_decrescente--;

        cadastrar_disciplina(raiz, variavel_disciplina->info, (*raiz)->info->codigo_curso);

        povoamentodecrescente_disciplinas(&(*raiz)->esq);
        povoamentodecrescente_disciplinas(&(*raiz)->dir);
    }
}

void povoar_disciplinas_aleatorio(Arv_Cursos **raiz)
{
    if (*raiz != NULL)
    {
        srand(time(NULL));

        for (int i = 1; i <= QUANTIDADEDISCIPLINAS; i++)
        {
            Arv_Disciplina *variavel_disciplina = (Arv_Disciplina *)malloc(sizeof(Arv_Disciplina));
            strcpy(variavel_disciplina->info->nome_disciplina, "Algoritmos e Estruturas de Pacientes");
            variavel_disciplina->info->carga_horaria = (rand() % 5 + 2) * 15;
            variavel_disciplina->info->periodo = rand() % (*raiz)->info->quantidade_periodos + 1;
            variavel_disciplina->info->codigo_disciplina = rand() % 1000 + 1;

            cadastrar_disciplina(raiz, variavel_disciplina->info, (*raiz)->info->codigo_curso);
        }

        povoar_disciplinas_aleatorio(&(*raiz)->esq);
        povoar_disciplinas_aleatorio(&(*raiz)->dir);
    }
}

// 3. métricas de tempo

// por inserção na árvore de cursos

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

double tempomedio_insercao_cursos(Arv_Cursos **curso)
{
    clock_t inicio, fim;
    double tempo = 0.0;
    int i;

    for (i = 0; i < LOOP; i++)
    {
        Cursos_Info *entrada_curso;
        entrada_curso = (Cursos_Info *)malloc(sizeof(Cursos_Info));
        
        entrada_curso->codigo_curso = rand() % CODIGOCURSO + 1;
        strcpy(entrada_curso->nome_curso, "MEDICINA");
        entrada_curso->quantidade_periodos = 12;
        Arv_Disciplina *disciplina = NULL;

        inicio = clock();
        cadastrar_curso(curso, entrada_curso);
        fim = clock();

        tempo += ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    }
    free_arvore_cursos(*curso);

    return tempo / LOOP;
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

// -----------------------------------  POVOAMENTO ALUNOS --------------------------------

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

void exibir_todos_alunos(Alunos *aluno)
{
    if (aluno != NULL)
    {
        printf("Matrícula: %d\n", aluno->matricula);
        printf("Nome: %s\n", aluno->nome);
        printf("Código do Curso: %d\n", aluno->codigo_curso);
        printf("----------------------------\n");

        exibir_todos_alunos(aluno->prox);
    }
}

int main()
{
    Arv_Cursos *raiz = NULL;
    Arv_Notas *raiz_notas = NULL;
    Alunos *alunos = NULL;
    int opcao_principal, opcao_submenu, codigo_curso, matricula, codigo_disciplina;
    double tempo;

    srand(time(NULL));

    do
    {
        printf("\nMenu Principal:\n");
        printf("1. Curso\n");
        printf("2. Disciplina\n");
        printf("3. Aluno\n");
        printf("4. Nota\n");
        printf("5. Busca\n");
        printf("6. Testes\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao_principal);

        switch (opcao_principal)
        {
        case 1: // Menu Curso
            printf("\nMenu Curso:\n");
            printf("1. Inserir cursos em ordem crescente\n");
            printf("2. Inserir cursos em ordem decrescente\n");
            printf("3. Inserir cursos de forma aleatória\n");
            printf("4. Conferir inserção de cursos\n");
            printf("0. Voltar\n");
            printf("Escolha uma opção: ");
            scanf("%d", &opcao_submenu);

            switch (opcao_submenu)
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
                if (raiz == NULL)
                {
                    printf("Erro: Nenhum curso cadastrado.\n");
                }
                else
                {
                    exibir_curso(raiz);
                }
                break;
            case 0:
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
            }
            break;

        case 2: // Menu Disciplina
            printf("\nMenu Disciplina:\n");
            printf("1. Inserir disciplinas em ordem crescente\n");
            printf("2. Inserir disciplinas em ordem decrescente\n");
            printf("3. Inserir disciplinas de forma aleatória\n");
            printf("4. Conferir inserção de disciplinas\n");
            printf("0. Voltar\n");
            printf("Escolha uma opção: ");
            scanf("%d", &opcao_submenu);

            switch (opcao_submenu)
            {
            case 1:
                povoamentocrescente_disciplinas(&raiz);
                printf("Disciplinas inseridas em ordem crescente.\n");
                break;
            case 2:
                povoamentodecrescente_disciplinas(&raiz);
                printf("Disciplinas inseridas em ordem decrescente.\n");
                break;
            case 3:
                povoar_disciplinas_aleatorio(&raiz);
                printf("Disciplinas inseridas de forma aleatória.\n");
                break;
            case 4:
                printf("Escolha o curso: ");
                scanf("%d", &codigo_curso);
                exibir_disciplinasporcurso(raiz, codigo_curso);
                break;
            case 0:
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
            }
            break;

        case 3: // Menu Aluno
            printf("\nMenu Aluno:\n");
            printf("1. Inserir alunos em ordem crescente\n");
            printf("2. Inserir alunos em ordem decrescente\n");
            printf("3. Inserir alunos de forma aleatória\n");
            printf("4. Conferir inserção de alunos\n");
            printf("0. Voltar\n");
            printf("Escolha uma opção: ");
            scanf("%d", &opcao_submenu);

            switch (opcao_submenu)
            {
            case 1:
                povoamentocrescente_alunos(&alunos, raiz);
                printf("Alunos inseridos em ordem crescente.\n");
                break;
            case 2:
                povoamentodecrescente_alunos(&alunos, raiz);
                printf("Alunos inseridos em ordem decrescente.\n");
                break;
            case 3:
                povoar_alunos_aleatorio(&alunos, raiz);
                printf("Alunos inseridos de forma aleatória.\n");
                break;
            case 4:
                exibir_todos_alunos(alunos);
                break;
            case 0:
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
            }
            break;

        case 4: // Menu Nota
            printf("\nMenu Nota:\n");
            printf("1. Inserir notas em ordem crescente\n");
            printf("2. Inserir notas em ordem decrescente\n");
            printf("3. Inserir notas de forma aleatória\n");
            printf("4. Conferir inserção de notas\n");
            printf("0. Voltar\n");
            printf("Escolha uma opção: ");
            scanf("%d", &opcao_submenu);

            switch (opcao_submenu)
            {
            case 1:
                povoamentocrescente_notas(alunos);
                break;
            case 2:
                povoamentodecrescente_notas(alunos);
                break;
            case 3:
                povoar_notas_aleatorio(alunos);
                break;
            case 4:
                exibir_notas(alunos->notas);
                break;
            case 0:
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
            }
            break;

        case 5: // Menu Busca
            printf("\nMenu Busca:\n");
            printf("1. Buscar nota de disciplina\n");
            printf("0. Voltar\n");
            printf("Escolha uma opção: ");
            scanf("%d", &opcao_submenu);

            switch (opcao_submenu)
            {
            case 1:
                printf("Informe a matrícula do aluno: ");
                scanf("%d", &matricula);
                printf("Informe o código da disciplina: ");
                scanf("%d", &codigo_disciplina);
                tempo = tempomedio_busca(alunos, raiz, matricula, codigo_disciplina);
                printf("Tempo médio de busca de notas: %.6f segundos\n", tempo);
                break;
            case 0:
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
            }
            break;

        case 6: // Menu Testes
            printf("\nMenu Testes:\n");
            printf("1. Calcular tempo médio de inserção de cursos\n");
            printf("2. Calcular tempo médio de busca de notas\n");
            printf("0. Voltar\n");
            printf("Escolha uma opção: ");
            scanf("%d", &opcao_submenu);

            switch (opcao_submenu)
            {
            case 1:
                tempo = tempomedio_insercao_cursos(&raiz);
                printf("Tempo médio de inserção de cursos: %.6f segundos\n", tempo);
                break;
            case 2:
                printf("Informe a matrícula do aluno: ");
                scanf("%d", &matricula);
                printf("Informe o código da disciplina: ");
                scanf("%d", &codigo_disciplina);
                tempo = tempomedio_busca(alunos, raiz, matricula, codigo_disciplina);
                printf("Tempo médio de busca de notas: %.6f segundos\n", tempo);
                break;
            case 0:
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
            }
            break;

        case 0:
            printf("PROGRAMA FINALIZADO.\n");
            break;
        default:
            printf("Opção inválida! Tente novamente.\n");
        }
    } while (opcao_principal != 0);

    free_arvore_cursos(raiz);
    free_lista_alunos(alunos);

    return 0;
}