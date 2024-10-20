#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/src.h"
#include <time.h>

#define QUANTIDADECURSOS 1000
#define CODIGOCURSO 1000
#define QUANTIDADEDISCIPLINAS 1000
#define QUANTIDADEALUNOS 1000
#define QUANTIDADENOTAS 1000

double tempos_insercao_crescente[QUANTIDADECURSOS];
double tempos_insercao_decrescente[QUANTIDADECURSOS];
double tempos_insercao_aleatorio[QUANTIDADECURSOS];

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
// --------------------------------------  POVOAMENTO CURSOS ---------------------------------

void povoamentocrescente_cursos(Arv_Cursos **raiz)
{
    *raiz = NULL;
    for (int i = 1; i <= QUANTIDADECURSOS; i++)
    {
        Cursos_Info *entrada_curso = (Cursos_Info *)malloc(sizeof(Cursos_Info));
        entrada_curso->codigo_curso = i;
        strcpy(entrada_curso->nome_curso, "MEDICINA");
        entrada_curso->quantidade_periodos = 12;

        clock_t inicio = clock(); 
        cadastrar_curso(raiz, entrada_curso);
        clock_t fim = clock(); 

        tempos_insercao_crescente[i - 1] = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000;
    }
}

void povoamentodecrescente_cursos(Arv_Cursos **raiz)
{
    *raiz = NULL;
    for (int i = QUANTIDADECURSOS; i >= 1; i--)
    {
        Cursos_Info *entrada_curso = (Cursos_Info *)malloc(sizeof(Cursos_Info));
        entrada_curso->codigo_curso = i;
        strcpy(entrada_curso->nome_curso, "MEDICINA");
        entrada_curso->quantidade_periodos = 12;

        clock_t inicio = clock(); 
        cadastrar_curso(raiz, entrada_curso);
        clock_t fim = clock(); 

        tempos_insercao_decrescente[QUANTIDADECURSOS - i] = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000;
    }
}

void povoamentoaleatorio_cursos(Arv_Cursos **raiz)
{
    *raiz = NULL;
    int i = 1;
    while (i <= QUANTIDADECURSOS)
    {
        Cursos_Info *entrada_curso = (Cursos_Info *)malloc(sizeof(Cursos_Info));
        entrada_curso->codigo_curso = rand() % QUANTIDADECURSOS + 1;
        strcpy(entrada_curso->nome_curso, "MEDICINA");
        entrada_curso->quantidade_periodos = 12;

        clock_t inicio = clock(); // Início da medição de tempo
        if (cadastrar_curso(raiz, entrada_curso) == 1)
        {
            clock_t fim = clock(); // Fim da medição de tempo

            // Calcula o tempo em milissegundos e armazena no array
            tempos_insercao_aleatorio[i - 1] = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000;
            i++;
        }
    }
}

// -----------------------------------  POVOAMENTO DISCIPLINAS -------------------------------
int codigo_disciplina_global_crescente = 1;

void povoamentocrescente_disciplinas(Arv_Cursos **raiz)
{
    if (*raiz != NULL)
    {
        Arv_Disciplina *d = (Arv_Disciplina *)malloc(sizeof(Arv_Disciplina));
        if (d == NULL)
            printf("Erro ao alocar memória para disciplina.\n");

        d->info = (Disciplinas_Info *)malloc(sizeof(Disciplinas_Info));
        if (d->info == NULL)
        {
            printf("Erro ao alocar memória para info da disciplina.\n");
            free(d);  
        }

        strcpy(d->info->nome_disciplina, "Algoritmos e Estruturas de Pacientes");
        d->info->carga_horaria = 60;
        d->info->periodo = 3;
        d->info->codigo_disciplina = codigo_disciplina_global_crescente++;

        cadastrar_disciplina(raiz, d->info, (*raiz)->info->codigo_curso);

        povoamentocrescente_disciplinas(&(*raiz)->esq);
        povoamentocrescente_disciplinas(&(*raiz)->dir);
    }
}

int codigo_disciplina_global_decrescente = QUANTIDADEDISCIPLINAS;

void povoamentodecrescente_disciplinas(Arv_Cursos **raiz)
{
    if (*raiz != NULL)
    {
        Arv_Disciplina *d = (Arv_Disciplina *)malloc(sizeof(Arv_Disciplina));
        if (d == NULL)
            printf("Erro ao alocar memória para disciplina.\n");

        d->info = (Disciplinas_Info *)malloc(sizeof(Disciplinas_Info));
        if (d->info == NULL)
        {
            printf("Erro ao alocar memória para info da disciplina.\n");
            free(d);  
        }

        strcpy(d->info->nome_disciplina, "Algoritmos e Estruturas de Pacientes");
        d->info->carga_horaria = 60;
        d->info->periodo = 3;
        d->info->codigo_disciplina = codigo_disciplina_global_decrescente--;

        cadastrar_disciplina(raiz, d->info, (*raiz)->info->codigo_curso);

        povoamentodecrescente_disciplinas(&(*raiz)->esq);
        povoamentodecrescente_disciplinas(&(*raiz)->dir);
    }
}

void povoar_disciplinas_aleatorio(Arv_Cursos **raiz)
{
    if (*raiz != NULL)
    {
        Arv_Disciplina *d = (Arv_Disciplina *)malloc(sizeof(Arv_Disciplina));
        if (d == NULL)
            printf("Erro ao alocar memória para disciplina.\n");

        d->info = (Disciplinas_Info *)malloc(sizeof(Disciplinas_Info));
        if (d->info == NULL)
        {
            printf("Erro ao alocar memória para info da disciplina.\n");
            free(d);  
        }

        strcpy(d->info->nome_disciplina, "Algoritmos e Estruturas de Pacientes");
        d->info->carga_horaria = (rand() % 5 + 2) * 15;
        d->info->periodo = rand() % (*raiz)->info->quantidade_periodos + 1;

        d->info->codigo_disciplina = (rand() % QUANTIDADEDISCIPLINAS) + 1;

        if (cadastrar_disciplina(raiz, d->info, (*raiz)->info->codigo_curso) == 1)
            cadastrar_disciplina(raiz, d->info, (*raiz)->info->codigo_curso);

        povoar_disciplinas_aleatorio(&(*raiz)->esq);
        povoar_disciplinas_aleatorio(&(*raiz)->dir);
    }
}


// --------------------------------------  POVOAMENTO ALUNOS ---------------------------------
void povoamentocrescente_alunos(Alunos **aluno, Arv_Cursos *curso)
{
    char *nomes[5] = {"Eleven", "Mike", "Dustin", "Lucas", "Max"};

    for (int i = 1; i <= QUANTIDADEALUNOS; i++)
    {
        int matricula = i;
        char *nome = nomes[rand() % 5];
        int codigo_curso = i;

        cadastrar_aluno(aluno, matricula, nome, codigo_curso);
    }
}

void povoamentodecrescente_alunos(Alunos **aluno, Arv_Cursos *curso)
{
    char *nomes[5] = {"Eleven", "Mike", "Dustin", "Lucas", "Max"};

    for (int i = QUANTIDADEALUNOS; i >= 1; i--)
    {
        int matricula = i;
        char *nome = nomes[rand() % 5];
        int codigo_curso = i;

        cadastrar_aluno(aluno, matricula, nome, codigo_curso);
    }
}
void povoar_alunos_aleatorio(Alunos **aluno, Arv_Cursos *curso)
{
    char *nomes[5] = {"Eleven", "Mike", "Dustin", "Lucas", "Max"};
    int i = 0;

    while (i < QUANTIDADEALUNOS)
    {
        int matricula = (rand() % QUANTIDADEALUNOS) + 1;
        char *nome = nomes[rand() % 5];
        int codigo_curso = i+1;

        if (cadastrar_aluno(aluno, matricula, nome, codigo_curso) == 1)
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


void exibir_todos_alunos_relatorio(Alunos *aluno)
{
    if (aluno != NULL)
    {
        if (aluno->codigo_curso == 1 || aluno->codigo_curso == 200 || aluno->codigo_curso == 500 || aluno->codigo_curso == 1000)
        {
            printf("Matrícula: %d\n", aluno->matricula);
            printf("Nome: %s\n", aluno->nome);
            printf("Código do Curso: %d\n", aluno->codigo_curso);
            printf("----------------------------\n");
        }


        exibir_todos_alunos_relatorio(aluno->prox);
    }
}



// ------------------------------------  POVOAMENTO DE NOTAS ----------------------------------

void povoamentocrescente_notas(Alunos *aluno)
{
    if (aluno != NULL)
    {
        for (int i = 1; i <= QUANTIDADECURSOS; i++)
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
        printf("Disciplina: %d\n", raiz->info->codigo_disciplina);
        printf("Nota final = %.2f\n", raiz->info->nota_final);
        exibir_notas(raiz->esq);
        exibir_notas(raiz->dir);
    }
}

void povoamentodecrescente_notas(Alunos *aluno)
{
    if (aluno != NULL)
    {
        for (int i = QUANTIDADECURSOS; i >= 1; i--)
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
        while (i <= QUANTIDADECURSOS)
        {
            int codigo_disciplina = (rand() % QUANTIDADEDISCIPLINAS) + 1;
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

// -----------------------------------  POVOAMENTO COMPLETO --------------------------------

void povoamentocrescente_completo(Arv_Cursos **raiz, Alunos **alunos)
{
    free_arvore_cursos(*raiz);
    free_lista_alunos(*alunos);

    *raiz = NULL;
    *alunos = NULL;

    povoamentocrescente_cursos(raiz);
    povoamentocrescente_alunos(alunos, *raiz);
    povoamentocrescente_disciplinas(raiz);
    povoamentocrescente_notas(*alunos);
}

void povoamentodecrescente_completo(Arv_Cursos **raiz, Alunos **alunos)
{
    free_arvore_cursos(*raiz);
    free_lista_alunos(*alunos);

    *raiz = NULL;
    *alunos = NULL;

    povoamentodecrescente_cursos(raiz);
    povoamentodecrescente_alunos(alunos, *raiz);
    povoamentodecrescente_disciplinas(raiz);
    povoamentodecrescente_notas(*alunos);
}

void povoamentoaleatorio_completo(Arv_Cursos **raiz, Alunos **alunos)
{

    free_arvore_cursos(*raiz);
    free_lista_alunos(*alunos);

    *raiz = NULL;
    *alunos = NULL;

    povoamentoaleatorio_cursos(raiz);
    povoar_alunos_aleatorio(alunos, *raiz);
    povoar_disciplinas_aleatorio(raiz);
    povoar_notas_aleatorio(*alunos);
}

// -----------------------------------  MENU E RELATÓRIOS ---------------------------------

void menu_inicial()
{
    printf("|------------------- Área de Testes ------------------|\n");
    printf("| 1. Inserções Crescentes                             |\n");
    printf("| 2. Inserções Decrescentes                           |\n");
    printf("| 3. Inserções Aleatórias                             |\n");
    printf("| 4. Buscar Nota                                      |\n");
    printf("| 5. Relatório                                        |\n");
    printf("| 6. Conferir Inserções                               |\n");
    printf("| 7. Sair                                             |\n");
    printf("|_____________________________________________________|\n");
}

double calcular_media_tempo(double *tempos)
{
    double soma = 0;
    for (int i = 0; i < QUANTIDADECURSOS; i++)
    {
        soma += tempos[i];
    }
    return soma / QUANTIDADECURSOS;
}

void relatorio_tempos_insercao()
{
    printf("|------------------ Relatório de Tempos ------------------|\n");
    printf("| Tipo de Inserção       | Tempo Médio (s)                |\n");
    printf("|---------------------------------------------------------|\n");
    printf("| Crescente              | %.6f                       |\n", calcular_media_tempo(tempos_insercao_crescente));
    printf("| Decrescente            | %.6f                       |\n", calcular_media_tempo(tempos_insercao_decrescente));
    printf("| Aleatória              | %.6f                       |\n", calcular_media_tempo(tempos_insercao_aleatorio));
    printf("|---------------------------------------------------------|\n");
}

// -----------------------------------  FUNÇÃO PRINCIPAL -----------------------------------

int main()
{
    Arv_Cursos *raiz = NULL;
    Alunos *alunos = NULL;
    int opcao_principal, matricula, codigo_disciplina, menu_testes, codigo_curso;

    srand(time(NULL));

    do
    {
        menu_inicial();
        printf("Escolha uma opção: ");
        scanf("%d", &opcao_principal);

        switch (opcao_principal)
        {
        case 1:
            povoamentocrescente_completo(&raiz, &alunos);
            printf("Inserção crescente concluída!\n");
            break;
        case 2:
            povoamentodecrescente_completo(&raiz, &alunos);
            printf("Inserção decrescente concluída!\n");
            break;
        case 3:
            povoamentoaleatorio_completo(&raiz, &alunos);
            printf("Inserção aleatória concluída!\n");
            break;
        case 4:
            printf("Informe a matrícula do aluno: ");
            scanf("%d", &matricula);
            printf("Informe o código da disciplina: ");
            scanf("%d", &codigo_disciplina);
            clock_t inicio = clock();
            exibir_nota_aluno_disciplina(alunos, raiz, matricula, codigo_disciplina);
            clock_t fim = clock();
            printf("O tempo para a busca foi de: %fs\n", ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000);
            break;
        case 5:
            relatorio_tempos_insercao();
            break;

        case 6:
            printf("1. Cursos\n");
            printf("2. Alunos\n");
            printf("3. Disciplinas\n");
            printf("4. Notas\n");
            printf("Opcao desejada: ");
            scanf("%d", &menu_testes);
            switch (menu_testes)
            {
            case 1:
                exibir_curso(raiz);
                break;
            case 2:
                exibir_todos_alunos_relatorio(alunos);
                break;
            case 3:
                printf("Escolha o curso: ");
                scanf("%d", &codigo_curso);
                exibir_disciplinasporcurso(raiz, codigo_curso);
                break;
            case 4:
                exibir_notas(alunos->notas);
                break;

            default:
                break;
            }
        case 7:
            printf("Saindo...\n");
            break;
        default:
            printf("Opção inválida!\n");
        }

    } while (opcao_principal != 7);

    free_arvore_cursos(raiz);
    free_lista_alunos(alunos);

    return 0;
}
