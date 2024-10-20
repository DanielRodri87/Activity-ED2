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
        clock_t inicio = clock();
        cadastrar_curso(raiz, i, "MEDICINA", 12);
        clock_t fim = clock();

        tempos_insercao_crescente[i - 1] = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000;
    }
}

void povoamentodescrecente_cursos(Arv_Cursos **raiz)
{
    *raiz = NULL;

    for (int i = QUANTIDADECURSOS; i >= 1; i--)
    {
        clock_t inicio = clock();
        cadastrar_curso(raiz, i, "MEDICINA", 12);
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
        int codigo_curso = rand() % QUANTIDADECURSOS + 1;
        clock_t inicio = clock();
        if (cadastrar_curso(raiz, codigo_curso, "MEDICINA", 12) == 1)
            i++;
        clock_t fim = clock();

        tempos_insercao_aleatorio[i - 1] = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000;
    }
}

// -----------------------------------  POVOAMENTO DISCIPLINAS -------------------------------
int codigo_disciplina_global_crescente = 1;
void povoamentocrescente_disciplinas(Arv_Cursos **raiz)
{
    if (*raiz != NULL)
    {
        Arv_Disciplina *d = (Arv_Disciplina *)malloc(sizeof(Arv_Disciplina));
        strcpy(d->nome_disciplina, "Algoritmos e Estruturas de Pacientes");
        d->carga_horaria = 60;
        d->periodo = 3;
        d->codigo_disciplina = codigo_disciplina_global_crescente++;

        cadastrar_disciplina(raiz, d, (*raiz)->codigo_curso);

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
        Arv_Disciplina *d = (Arv_Disciplina *)malloc(sizeof(Arv_Disciplina));
        strcpy(d->nome_disciplina, "Algoritmos e Estruturas de Pacientes");
        d->carga_horaria = 60;
        d->periodo = 3;
        d->codigo_disciplina = codigo_disciplina_global_decrescente--;

        cadastrar_disciplina(raiz, d, (*raiz)->codigo_curso);

        povoamentodecrescente_disciplinas(&(*raiz)->esq);
        povoamentodecrescente_disciplinas(&(*raiz)->dir);
    }
}

void povoar_disciplinas_aleatorio(Arv_Cursos **raiz)
{
    if (*raiz != NULL)
    {
        srand(time(NULL));

        Arv_Disciplina *d = (Arv_Disciplina *)malloc(sizeof(Arv_Disciplina));
        strcpy(d->nome_disciplina, "Algoritmos e Estruturas de Pacientes");
        d->carga_horaria = (rand() % 5 + 2) * 15;
        d->periodo = rand() % (*raiz)->quantidade_periodos + 1;
        d->codigo_disciplina = (rand() % QUANTIDADEDISCIPLINAS) + 1;

        if (cadastrar_disciplina(raiz, d, (*raiz)->codigo_curso) == 1)
            cadastrar_disciplina(raiz, d, (*raiz)->codigo_curso);

        povoar_disciplinas_aleatorio(&(*raiz)->esq);
        povoar_disciplinas_aleatorio(&(*raiz)->dir);
    }
}

// -----------------------------------  POVOAMENTO ALUNOS --------------------------------

void povoamentocrescente_alunos(Alunos **aluno, Arv_Cursos *curso)
{
    char *nomes[5] = {"Daniel", "Rodrigues", "Cristina", "Sousa", "Juliana"};

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
    char *nomes[5] = {"Daniel", "Rodrigues", "Cristina", "Sousa", "Juliana"};

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
    char *nomes[5] = {"Daniel", "Rodrigues", "Cristina", "Sousa", "Juliana"};
    int i = 0;

    while (i < QUANTIDADEALUNOS)
    {
        int matricula = (rand() % QUANTIDADEALUNOS) + 1;
        char *nome = nomes[rand() % 5];
        int codigo_curso = i + 1;

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

// -------------------------------------  POVOAMENTO NOTAS -----------------------------------
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

            cadastrar_nota(&aluno, matricula, codigo_disciplina, semestre, nota_final);
        }

        povoamentocrescente_notas(aluno->prox);
    }
}

void exibir_notas(Arv_Notas *raiz)
{
    if (raiz != NULL)
    {
        printf("Disciplina: %d\n", raiz->codigo_disciplina);
        printf("Nota final = %.2f\n", raiz->nota_final);
        exibir_notas(raiz->esq);
        exibir_notas(raiz->dir);
    }
}

void exibir_todos_alunos_para_relatorio(Alunos *aluno)
{
    if (aluno != NULL)
    {
        if(aluno->codigo_curso == 1 || aluno->codigo_curso == 200 || aluno->codigo_curso == 500 || aluno->codigo_curso == 1000)
        {
            printf("Matrícula: %d\n", aluno->matricula);
            printf("Nome: %s\n", aluno->nome);
            printf("Código do Curso: %d\n", aluno->codigo_curso);
            printf("----------------------------\n");
        }

        exibir_todos_alunos_para_relatorio(aluno->prox);
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

            cadastrar_nota(&aluno, matricula, codigo_disciplina, semestre, nota_final);
        }

        povoamentodecrescente_notas(aluno->prox);
    }
}

void povoar_notas_aleatorio(Alunos *aluno)
{
    if (aluno != NULL)
    {
        int i = 0; // Contador de notas cadastradas
        while (i < QUANTIDADECURSOS)
        {
            int codigo_disciplina = (rand() % QUANTIDADEDISCIPLINAS) + 1;
            int semestre = 3;
            float nota_final = (rand() % 10) + 1;
            int matricula = aluno->matricula;

            int enc_disc = 0;
            auxiliar_validacao(aluno->notas, codigo_disciplina, &enc_disc);

            if (enc_disc == 0)
            {
                cadastrar_matricula(&aluno, codigo_disciplina, matricula);

                if (cadastrar_nota(&aluno, matricula, codigo_disciplina, semestre, nota_final) == 1)
                    i++;
            }
        }

        povoar_notas_aleatorio(aluno->prox);
    }
}

// -----------------------------------  POVOAMENTO COMPLETO --------------------------------

void povoamentocrescente_completo(Arv_Cursos **raiz, Alunos **alunos)
{
    free_arvore_cursos(*raiz);
    *raiz = NULL;
    free_lista_alunos(*alunos);
    *alunos = NULL;

    povoamentocrescente_cursos(raiz);
    povoamentocrescente_alunos(alunos, *raiz);
    povoamentocrescente_disciplinas(raiz);
    povoamentocrescente_notas(*alunos);
}

void povoamentodecrescente_completo(Arv_Cursos **raiz, Alunos **alunos)
{

    free_arvore_cursos(*raiz);
    *raiz = NULL; 
    free_lista_alunos(*alunos);
    *alunos = NULL;

    povoamentodescrecente_cursos(raiz);
    povoamentodecrescente_alunos(alunos, *raiz);
    povoamentodecrescente_disciplinas(raiz);
    povoamentodecrescente_notas(*alunos);
}

void povoamentoaleatorio_completo(Arv_Cursos **raiz, Alunos **alunos)
{
    free_arvore_cursos(*raiz);
    *raiz = NULL; 
    free_lista_alunos(*alunos);
    *alunos = NULL;

    povoamentoaleatorio_cursos(raiz);
    povoar_alunos_aleatorio(alunos, *raiz);
    povoar_disciplinas_aleatorio(raiz);
    povoar_notas_aleatorio(*alunos);
}

// -----------------------------------  MENU E RELATÓRIOS -----------------------------------

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
            raiz = NULL;
            alunos = NULL;
            povoamentocrescente_completo(&raiz, &alunos);
            printf("Inserção crescente concluída!\n");
            break;
        case 2:
            raiz = NULL;
            alunos = NULL;
            povoamentodecrescente_completo(&raiz, &alunos);
            printf("Inserção decrescente concluída!\n");
            break;
        case 3:
            raiz = NULL;
            alunos = NULL;
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
                exibir_disciplinas_por_curso(raiz, codigo_curso);
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
