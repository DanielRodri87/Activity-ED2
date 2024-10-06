#include "src.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
void converternome(char *nome)
{
    int i = 0;
    while (nome[i] != '\0')
    {
        nome[i] = toupper(nome[i]);
        i++;
    }
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// I - Cadastrar alunos a qualquer momento na lista, de forma que só possa cadastrar um código de curso que
// já tenha sido cadastrado na árvore de cursos.
// ---------------------------------------------------- XXXXXX -------------------------------------------------

// exibição da nota de uma disciplina de um determinado aluno, mostrando o período e a carga horária da disciplina.
Arv_Cursos *buscar_curso(Arv_Cursos *curso, int codigo_curso)
{
    Arv_Cursos *resultado = NULL;

    if (curso != NULL)
    {
        if (curso->codigo_curso == codigo_curso)
            resultado = curso;
        else if (codigo_curso < curso->codigo_curso)
            resultado = buscar_curso(curso->esq, codigo_curso);
        else
            resultado = buscar_curso(curso->dir, codigo_curso);
    }

    return (resultado);
}

void cadastrar_aluno(Arv_Cursos *arv_cursos, int matricula, char *nome, int codigo_curso)
{
    // Buscar o curso pelo código
    Arv_Cursos *curso = buscar_curso(arv_cursos, codigo_curso);
    if (curso == NULL)
        printf("Curso com código %d não encontrado.\n", codigo_curso);

    Alunos *novo = (Alunos *)malloc(sizeof(Alunos));
    if (novo == NULL)
        printf("Erro ao alocar memória para o aluno.\n");

    novo->prox = NULL;
    novo->matricula = matricula;
    char *aux_nome = strdup(nome);
    converternome(aux_nome);
    strcpy(novo->nome, aux_nome);
    novo->codigo_curso = codigo_curso;
    novo->notas = NULL;
    novo->mat = NULL;

    if (curso->alunos == NULL)
        curso->alunos = novo;
    else
    {
        Alunos *aux = curso->alunos;
        if (strcmp(aux_nome, aux->nome) < 0)
        {
            novo->prox = curso->alunos;
            curso->alunos = novo;
        }
        else
        {
            while (aux->prox != NULL && strcmp(aux_nome, aux->prox->nome) > 0)
                aux = aux->prox;

            novo->prox = aux->prox;
            aux->prox = novo;
        }
    }

    free(aux_nome);
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// II - Cadastrar cursos a qualquer momento na árvore de curso, de forma que o usuário não precise cadastrar
// as disciplinas para permitir o cadastro do curso.
// ---------------------------------------------------- XXXXXX -------------------------------------------------

void cadastrar_curso(Arv_Cursos **curso, int codigo_curso, const char *nome_curso, int quantidade_periodos)
{
    if (*curso == NULL)
    {
        Arv_Cursos *novo = (Arv_Cursos *)malloc(sizeof(Arv_Cursos));
        novo->codigo_curso = codigo_curso;
        strcpy(novo->nome_curso, nome_curso);
        novo->quantidade_periodos = quantidade_periodos;
        novo->dir = NULL;
        novo->esq = NULL;
        *curso = novo;
    }
    else
    {
        if (codigo_curso < (*curso)->codigo_curso)
            cadastrar_curso(&((*curso)->esq), codigo_curso, nome_curso, quantidade_periodos);
        else
            cadastrar_curso(&((*curso)->dir), codigo_curso, nome_curso, quantidade_periodos);
    }
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// Cadastrar uma matrícula, onde a mesma é uma árvore organizada e contendo somente um código de
// uma disciplina do curso do aluno.
// ---------------------------------------------------- XXXXXX -------------------------------------------------
void inserirMatricula(Arv_Matricula **mat, int codigo, int *igual)
{ 
    if(*mat == NULL)
    {
        Arv_Matricula *novo = (Arv_Matricula*)malloc(sizeof(Arv_Matricula));
        novo->codigo_disciplina = codigo;
        novo->esq = NULL;
        novo->dir = NULL;
        *mat = novo;
    } 
    else if(codigo < (*mat)->codigo_disciplina)
        inserirMatricula(&(*mat)->esq, codigo, igual);
    else if(codigo > (*mat)->codigo_disciplina)
        inserirMatricula(&(*mat)->dir, codigo, igual);
    else
        *igual = -1;
}



void cadastrar_matricula(Alunos **a, int codigo, int mat)
{
    if (*a != NULL)
    {
        if ((*a)->matricula == mat)
        {
            int igual = 0;
            inserirMatricula(&(*a)->mat, codigo, &igual);
        }
        else
            cadastrar_matricula(&(*a)->prox, codigo, mat);
    }
}


void exibir_matriculas(Arv_Matricula *r)
{
    if (r != NULL)
    {
        printf("Matricula: %d\n", r->codigo_disciplina);
        exibir_matriculas(r->esq);
        exibir_matriculas(r->dir);
    }
}

void buscar_matricula(Arv_Matricula *r, int codigo, int *encontrado)
{
    if (r != NULL)
    {
        if (r->codigo_disciplina == codigo)
            *encontrado = 1;
        else
        {
            if (codigo < r->codigo_disciplina)
                buscar_matricula(r->esq, codigo, encontrado);
            else
                buscar_matricula(r->dir, codigo, encontrado);
        }
    }
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// V - Cadastrar Notas, permitir o cadastro de notas somente de disciplinas que estejam na árvore de
// matricula, e quando a nota for cadastrada a disciplina deve ser removida da árvore de matricula para
// árvore de notas.
// ---------------------------------------------------- XXXXXX -------------------------------------------------
void buscar_disciplina(Arv_Matricula *r, int codigo, int *enc)
{
    if (r != NULL)
    {
        if (r->codigo_disciplina == codigo)

            *enc = 1;
        else if (codigo < r->codigo_disciplina)
            buscar_disciplina(r->esq, codigo, enc);
        else
            buscar_disciplina(r->dir, codigo, enc);
    }
}

int eh_folha_mat(Arv_Matricula *r)
{
    return (r->esq == NULL && r->dir == NULL);
}

Arv_Matricula *so_um_filho_mat(Arv_Matricula *r)
{
    Arv_Matricula *aux = NULL;
    if (r->esq == NULL)
        aux = r->dir;
    else if (r->dir == NULL)
        aux = r->esq;

    return aux;
}

Arv_Matricula *menor_filho_esq_mat(Arv_Matricula *r)
{
    Arv_Matricula *aux;
    aux = NULL;

    if (r != NULL)
    {
        aux = menor_filho_esq_mat(r->esq);
        if (aux == NULL)
            aux = r;
    }

    return aux;
}

void remover_matricula(Arv_Matricula **r, int codigo)
{
    if (*r != NULL)
    {
        if ((*r)->codigo_disciplina == codigo)
        {
            Arv_Matricula *aux;

            if (eh_folha_mat(*r))
            {
                aux = *r;
                free(aux);
                *r = NULL;
            }
            else if ((aux = so_um_filho_mat(*r)) != NULL)
            {
                Arv_Matricula *temp;
                temp = *r;
                free(temp);
                *r = aux;
            }
            else
            {
                Arv_Matricula *menor = menor_filho_esq_mat((*r)->dir);
                (*r)->codigo_disciplina = menor->codigo_disciplina;
                remover_matricula(&(*r)->dir, menor->codigo_disciplina);
            }
        }
        else if ((*r)->codigo_disciplina > codigo)
            remover_matricula(&(*r)->esq, codigo);
        else
            remover_matricula(&(*r)->dir, codigo);
    }
}

void cadastrar_notas(Arv_Notas **nota, int codigo, int semestre, float notafinal)
{
    if (*nota == NULL)
    {
        Arv_Notas *n = (Arv_Notas *)malloc(sizeof(Arv_Notas));
        n->codigo_disciplina = codigo;
        n->semestre = semestre;
        n->nota_final = notafinal;
        n->esq = NULL;
        n->dir = NULL;

        *nota = n;
    }
    else
    {
        if ((*nota)->codigo_disciplina > codigo)
            cadastrar_notas(&((*nota)->esq), codigo, semestre, notafinal);
        else
            cadastrar_notas(&((*nota)->dir), codigo, semestre, notafinal);
    }
}

int cadastrar_nota(Alunos **a, int mat, int codigo, int semestre, float notafinal)
{
    int c = 0;
    if (*a != NULL)
    {
        if ((*a)->codigo_curso == codigo)
        {
            int c_disc = 0;
            buscar_disciplina((*a)->mat, codigo, &c_disc);
            if (c_disc == 1)
            {
                remover_matricula(&(*a)->mat, codigo);
                cadastrar_notas(&(*a)->notas, codigo, semestre, notafinal);
            }
            else
                c = cadastrar_nota(&(*a)->prox, mat, codigo, semestre, notafinal);
        }
    }
}

Alunos *buscar_aluno_por_matricula_no_curso(Arv_Cursos *curso, int codigo_curso, int matricula)
{
    // Busca o curso desejado
    if (curso == NULL)
    {
        return NULL;
    }

    if (curso->codigo_curso == codigo_curso)
    {
        // Busca o aluno dentro do curso
        Alunos *aluno = curso->alunos;
        while (aluno != NULL)
        {
            if (aluno->matricula == matricula)
            {
                return aluno; // Aluno encontrado
            }
            aluno = aluno->prox;
        }
        return NULL; // Aluno não encontrado
    }

    // Caso não encontre o curso, busca nas sub-árvores
    Alunos *esquerda = buscar_aluno_por_matricula_no_curso(curso->esq, codigo_curso, matricula);
    if (esquerda != NULL)
        return esquerda;

    return buscar_aluno_por_matricula_no_curso(curso->dir, codigo_curso, matricula);
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// VII - Mostrar todos os cursos do Campus
// ---------------------------------------------------- XXXXXX -------------------------------------------------
void exibir_curso(Arv_Cursos *r)
{
    if (r != NULL)
    {
        printf("Nome do curso: %s | ID: %d\n", r->nome_curso, r->codigo_curso);
        exibir_curso(r->esq);
        exibir_curso(r->dir);
    }
}
// ---------------------------------------------------- XXXXXX -------------------------------------------------
// IX - Mostrar todas as disciplinas de um determinado período de um curso.
// ---------------------------------------------------- XXXXXX -------------------------------------------------
void exibir_disciplina_periodo(Arv_Cursos *r, int periodo)
{
    if (r != NULL)
    {
        if (r->disciplina->periodo == periodo)
        {
            printf("Curso: %s | Disciplina: %s | Período: %d\n", r->nome_curso, r->disciplina->nome_disciplina, r->disciplina->periodo);
            exibir_disciplina_periodo(r->esq, periodo);
            exibir_disciplina_periodo(r->dir, periodo);
        }
    }
}

int notas_disciplina_periodo(Arv_Notas *n, int periodo)
{
    if (n == NULL)
        printf("Nota nao encontrada\n");
    else
    {
        if (n->semestre == periodo)
            printf("Disciplina: %d, Nota Final: %.2f\n", n->codigo_disciplina, n->nota_final);

        notas_disciplina_periodo(n->esq, periodo);
        notas_disciplina_periodo(n->dir, periodo);
    }
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// XI -  Mostrar todas as notas de disciplinas de um determinado período de um determinado aluno.
// ---------------------------------------------------- XXXXXX -------------------------------------------------


void aux_XI_notaperiodo(Arv_Notas *nota, int periodo)
{
    if(nota != NULL)
    {
        if(nota->semestre == periodo)
        {
            printf("Codigo: %d\n", nota->codigo_disciplina);
            printf("Nota Final: %.2f\n", nota->nota_final);
            printf("Semestre: %.2f\n", nota->semestre);
            printf("\n");
        }
        aux_XI_notaperiodo(nota->esq, periodo);
        aux_XI_notaperiodo(nota->dir, periodo);
    }
}

void notas_disciplina_periodo_aluno(Alunos *aluno, int periodo, int matricula)
{
    if(aluno != NULL)
    {
        if(aluno->matricula == matricula)
            aux_XI_notaperiodo(aluno->notas, periodo);
        else
            notas_disciplina_periodo_aluno(aluno->prox, periodo, matricula);
    }
}


// ---------------------------------------------------- XXXXXX -------------------------------------------------
// VI -  Mostrar todos os alunos de um determinado curso.
// ---------------------------------------------------- XXXXXX -------------------------------------------------

void exibir_alunos_recursivamente(Alunos *aluno)
{
    if (aluno != NULL)
    {
        printf("Nome: %s\n", aluno->nome);
        printf("Matricula: %d\n", aluno->matricula);
        exibir_alunos_recursivamente(aluno->prox);
    }
}

void exibir_alunosporcurso(Arv_Cursos *curso, int codigo_curso)
{
    if (curso != NULL)
    {
        if (curso->codigo_curso == codigo_curso)
        {
            printf("Alunos do curso: %s (Código: %d)\n", curso->nome_curso, curso->codigo_curso);
            printf("-----------------------------------\n");

            exibir_alunos_recursivamente(curso->alunos);
        }

        exibir_alunosporcurso(curso->esq, codigo_curso);
        exibir_alunosporcurso(curso->dir, codigo_curso);
    }
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// IX -  Mostrar todas as disciplinas de um determinado período de um curso.
// ---------------------------------------------------- XXXXXX -------------------------------------------------

void exibir_disciplinasporcurso_recursivamente(Arv_Disciplina *disciplina)
{
    if (disciplina != NULL)
    {

        printf("Codigo: %d\n", disciplina->codigo_disciplina);
        printf("Nome: %s\n", disciplina->nome_disciplina);
        printf("Periodo: %d\n", disciplina->periodo);
        printf("Carga Horaria: %d\n", disciplina->carga_horaria);
        printf("-------------------------------\n");

        exibir_disciplinasporcurso_recursivamente(disciplina->esq);
        exibir_disciplinasporcurso_recursivamente(disciplina->dir);
    }
}

void exibir_disciplinasporcurso(Arv_Cursos *curso, int codigo_curso)
{
    if (curso != NULL)
    {
        if (curso->codigo_curso == codigo_curso)
        {
            Arv_Disciplina *disciplina_atual = curso->disciplina;
            printf("Disciplinas do curso %s:\n", curso->nome_curso);
            printf("-------------------------------\n");

            exibir_disciplinasporcurso_recursivamente(disciplina_atual);
        }

        exibir_disciplinasporcurso(curso->esq, codigo_curso);
        exibir_disciplinasporcurso(curso->dir, codigo_curso);
    }
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// X - Mostrar todas as disciplinas que um determinado aluno está matriculado
// ---------------------------------------------------- XXXXXX -------------------------------------------------

void exibir_disciplinas_por_aluno_recursivo(Arv_Disciplina *disciplina_atual)
{
    if (disciplina_atual != NULL)
    {
        printf("Código: %d - Período: %d - Nome: %s\n",
               disciplina_atual->codigo_disciplina,
               disciplina_atual->periodo,
               disciplina_atual->nome_disciplina);

        exibir_disciplinas_por_aluno_recursivo(disciplina_atual->esq);
        exibir_disciplinas_por_aluno_recursivo(disciplina_atual->dir);
    }
}


void exibir_disciplinasporaluno(Arv_Cursos *curso, int matricula_aluno)
{
    if (curso != NULL)
    {
        if (curso->codigo_curso != -1)
        {
            Alunos *aluno_atual = curso->alunos;
            if (aluno_atual != NULL && aluno_atual->matricula == matricula_aluno)
            {
                printf("Disciplinas do aluno %d:\n", matricula_aluno);
                exibir_disciplinas_por_aluno_recursivo(curso->disciplina);
            }
        }

        exibir_disciplinasporaluno(curso->esq, matricula_aluno);
        exibir_disciplinasporaluno(curso->dir, matricula_aluno);
    }
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// XIV - Permita remover uma disciplina da árvore de matrícula de um determinado aluno.
// ---------------------------------------------------- XXXXXX -------------------------------------------------

void remover_disciplinaaluno(Arv_Matricula **raiz, int codigo_disciplina)
{
    if (*raiz != NULL)
    {
        Arv_Matricula *atual = *raiz;
        Arv_Matricula *pai = NULL;
        int encontrado = 0;

        localizar_no(&atual, &pai, codigo_disciplina, &encontrado);

        if (encontrado)
        {
            remover_no(raiz, pai, atual);
        }
    }
}

void localizar_no(Arv_Matricula **atual, Arv_Matricula **pai, int codigo_disciplina, int *encontrado)
{
    while (*atual != NULL && *encontrado == 0)
    {
        if ((*atual)->codigo_disciplina == codigo_disciplina)
        {
            *encontrado = 1;
        }
        else
        {
            *pai = *atual;

            if (codigo_disciplina < (*atual)->codigo_disciplina)
            {
                *atual = (*atual)->esq;
            }
            else
            {
                *atual = (*atual)->dir;
            }
        }
    }
}

void remover_no(Arv_Matricula **raiz, Arv_Matricula *pai, Arv_Matricula *atual)
{
    if (atual->esq == NULL && atual->dir == NULL)
    {
        remover_no_folha(raiz, pai, atual);
    }
    else if (atual->esq == NULL || atual->dir == NULL)
    {
        remover_no_com_um_filho(raiz, pai, atual);
    }
    else
    {
        remover_no_com_dois_filhos(atual);
    }

    free(atual);
}

void remover_no_folha(Arv_Matricula **raiz, Arv_Matricula *pai, Arv_Matricula *atual)
{
    if (pai == NULL)
    {
        *raiz = NULL;
    }
    else if (pai->esq == atual)
    {
        pai->esq = NULL;
    }
    else
    {
        pai->dir = NULL;
    }
}

void remover_no_com_um_filho(Arv_Matricula **raiz, Arv_Matricula *pai, Arv_Matricula *atual)
{
    Arv_Matricula *filho = (atual->esq != NULL) ? atual->esq : atual->dir;

    if (pai == NULL)
    {
        *raiz = filho;
    }
    else if (pai->esq == atual)
    {
        pai->esq = filho;
    }
    else
    {
        pai->dir = filho;
    }
}

void remover_no_com_dois_filhos(Arv_Matricula *atual)
{
    Arv_Matricula *menor_no = atual->dir;
    Arv_Matricula *menor_nopai = atual;

    while (menor_no->esq != NULL)
    {
        menor_nopai = menor_no;
        menor_no = menor_no->esq;
    }

    atual->codigo_disciplina = menor_no->codigo_disciplina;

    if (menor_nopai->esq == menor_no)
    {
        menor_nopai->esq = menor_no->dir;
    }
    else
    {
        menor_nopai->dir = menor_no->dir;
    }
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// III -  Cadastrar disciplinas a qualquer momento em uma árvore de disciplinas de um determinado curso, ou
// seja, um disciplina só pode ser cadastrada se o curso já estiver sido cadastrado, além disso, o período da
// disciplina deve ser válido, ou seja, estar entre 1 e a quantidade máxima de períodos do curso. A carga
// horária da disciplina deve ser múltiplo de 15, variando entre 30 e 90.
// ---------------------------------------------------- XXXXXX -------------------------------------------------

void cadastrar_disciplina(Arv_Cursos *curso, int codigo_curso, int codigo_disciplina, char *nome_disciplina, int periodo, int cargahoraria)
{
    int curso_encontrado = 0;
    int periodo_valido = 0;
    int cargahoraria_valida = 0;

    while (curso != NULL && curso->codigo_curso != codigo_curso)
    {
        if (codigo_curso < curso->codigo_curso)
            curso = curso->esq;
        else
            curso = curso->dir;
    }

    if (curso != NULL)
        curso_encontrado = 1;

    if (curso_encontrado && (periodo >= 1 && periodo <= curso->quantidade_periodos))
        periodo_valido = 1;

    if (curso_encontrado && periodo_valido && (cargahoraria >= 30 && cargahoraria <= 90 && cargahoraria % 15 == 0))
        cargahoraria_valida = 1;

    if (curso_encontrado && periodo_valido && cargahoraria_valida)
    {
        Arv_Disciplina *nova_disciplina = (Arv_Disciplina *)malloc(sizeof(Arv_Disciplina));
        nova_disciplina->codigo_disciplina = codigo_disciplina;
        strcpy(nova_disciplina->nome_disciplina, nome_disciplina);
        nova_disciplina->periodo = periodo;
        nova_disciplina->carga_horaria = cargahoraria;
        nova_disciplina->esq = NULL;
        nova_disciplina->dir = NULL;

        if (curso->disciplina == NULL)
            curso->disciplina = nova_disciplina;
        else
        {
            Arv_Disciplina *atual = curso->disciplina;
            Arv_Disciplina *anterior = NULL;

            while (atual != NULL)
            {
                anterior = atual;
                if (codigo_disciplina < atual->codigo_disciplina)
                    atual = atual->esq;
                else
                    atual = atual->dir;
            }

            if (codigo_disciplina < anterior->codigo_disciplina)
                anterior->esq = nova_disciplina;
            else
                anterior->dir = nova_disciplina;
        }
    }
    else
    {
        printf("Erro ao cadastrar disciplina: ");
        if (!curso_encontrado)
            printf("Curso não encontrado.\n");
        else if (!periodo_valido)
            printf("Período inválido.\n");
        else if (!cargahoraria_valida)
            printf("Carga horária inválida.\n");
    }
}

Alunos *buscar_aluno(Alunos *alunos, int matricula)
{
    while (alunos != NULL && alunos->matricula != matricula)
        alunos = alunos->prox;
    return alunos;
}

Arv_Disciplina *buscar_disciplina_2(Arv_Disciplina *disciplina, int codigo_disciplina)
{
    while (disciplina != NULL && disciplina->codigo_disciplina != codigo_disciplina)
    {
        if (codigo_disciplina > disciplina->codigo_disciplina)
            disciplina = disciplina->dir;
        else
            disciplina = disciplina->esq;
    }
    return disciplina;
}

Arv_Notas *buscar_nota(Arv_Notas *notas, int codigo_disciplina)
{
    while (notas != NULL && notas->codigo_disciplina != codigo_disciplina)
    {
        if (codigo_disciplina > notas->codigo_disciplina)
            notas = notas->dir;
        else
            notas = notas->esq;
    }
    return notas;
}

void exibir_informacoes(Arv_Disciplina *disciplina_atual, Arv_Notas *nota_atual)
{
    printf("Disciplina: %s\n", disciplina_atual->nome_disciplina);
    printf("Periodo: %d\n", disciplina_atual->periodo);
    printf("Carga Horaria: %dh\n", disciplina_atual->carga_horaria);
    printf("Nota final: %.2f\n", nota_atual->nota_final);
}

void exibir_nota_aluno_disciplina(Arv_Cursos *curso, int codigo_curso, int matricula, int codigo_disciplina)
{
    if (curso != NULL)
    {
        if (curso->codigo_curso == codigo_curso)
        {
            Alunos *aluno = buscar_aluno_por_matricula_no_curso(curso, codigo_curso, matricula);
            if (aluno == NULL)
                printf("Aluno não encontrado no curso.\n");
            

            Arv_Notas *nota = aluno->notas;
            while (nota != NULL)
            {
                if (nota->codigo_disciplina == codigo_disciplina)
                {
                    printf("Aluno: %s\n", aluno->nome);
                    printf("Matrícula: %d\n", aluno->matricula);
                    printf("Disciplina: %d\n", codigo_disciplina);
                    printf("Nota Final: %.2f (Semestre: %.1f)\n", nota->nota_final, nota->semestre);
                }
                if (nota->esq != NULL)
                    nota = nota->esq;
                else
                    nota = nota->dir;
            }

            printf("Nota não encontrada para a disciplina especificada.\n");
        }

        exibir_nota_aluno_disciplina(curso->esq, codigo_curso, matricula, codigo_disciplina);
        exibir_nota_aluno_disciplina(curso->dir, codigo_curso, matricula, codigo_disciplina);
    }
}

// void exibir_notas_periodo(Alunos *aluno, Arv_Disciplina *disciplinas, int periodo)
// {
//     if (aluno == NULL)
//         printf("Aluno não encontrado ou não possui notas cadastradas.\n");

//     if (disciplinas == NULL)
//         printf("Nenhuma disciplina encontrada.\n");


//     if (disciplinas->periodo == periodo)
//     {
//         Arv_Notas *nota_atual = aluno->notas;
//         while (nota_atual != NULL) {
//             if (nota_atual->codigo_disciplina == disciplinas->codigo_disciplina) {
//                 printf("Disciplina: %s\n", disciplinas->nome_disciplina);
//                 printf("Nota Final: %.2f (Semestre: %.1f)\n", nota_atual->nota_final, nota_atual->semestre);
//                 printf("----------------------------------------\n");
//             }
//             if (nota_atual->esq != NULL) nota_atual = nota_atual->esq;
//             else nota_atual = nota_atual->dir;
//         }
//     }

//     exibir_notas_periodo(aluno, disciplinas->esq, periodo);
//     exibir_notas_periodo(aluno, disciplinas->dir, periodo);
// }



void exibir_alunos(Alunos *lista)
{
    Alunos *atual = lista;

    while (atual != NULL)
    {
        printf("Matricula: %d\n", atual->matricula);
        printf("Nome: %s\n", atual->nome);
        printf("Codigo do curso: %d\n", atual->codigo_curso);

        printf("\n");
        atual = atual->prox;
    }
}

Arv_Disciplina *buscar_disciplina_por_codigo(Arv_Disciplina *disciplinas, int codigo)
{
    Arv_Disciplina *aux;
    if (disciplinas != NULL)
    {
        if (codigo == disciplinas->codigo_disciplina)
            aux = disciplinas;
        else if (codigo < disciplinas->codigo_disciplina)
            aux = buscar_disciplina_por_codigo(disciplinas->esq, codigo);
        else
            aux = buscar_disciplina_por_codigo(disciplinas->dir, codigo);
    }

    return (aux);
}

// void mostrar_notas_aluno(Alunos *aluno, Arv_Disciplina *disciplinas, int periodo)
// {
//     if (aluno == NULL)
//         printf("Erro: Aluno não encontrado.\n");


//     if (disciplinas == NULL)
//         printf("Erro: Disciplinas não encontradas.\n");

//     printf("Notas do aluno %s no periodo %d:\n", aluno->nome, periodo);
    
//     exibir_notas_periodo(aluno, disciplinas, periodo);
// }


// ---------------------------------------------------- XXXXXX -------------------------------------------------
// XIII - Remover uma disciplina de um determinado curso desde que não tenha nenhum aluno matriculado na
// mesma.
// ---------------------------------------------------- XXXXXX -------------------------------------------------
int ehfolha(Arv_Disciplina *r)
{
    int retorno = 0;
    if (r != NULL)
    {
        if ((r->esq == NULL) && (r->dir == NULL))
            retorno = 1;

        ehfolha(r->esq);
        ehfolha(r->dir);
    }
}

Arv_Disciplina *so_um_filho(Arv_Disciplina *r)
{
    Arv_Disciplina *filho = NULL;

    if (r->esq != NULL && r->dir == NULL)
        filho = r->esq;
    else if (r->esq == NULL && r->dir != NULL)
        filho = r->dir;

    return filho;
}

Arv_Disciplina *menor_filho_esquerda_disciplina(Arv_Disciplina *r)
{
    Arv_Disciplina *aux;
    aux = NULL;
    if (r != NULL)
    {
        if (r->esq != NULL)
            aux = menor_filho_esquerda_disciplina(r->esq);
    }
    return (aux);
}

int remover_disciplina_xiii(Arv_Disciplina **r, int codigo_disciplina)
{
    Arv_Disciplina *aux;
    Arv_Disciplina *end_filho;
    Arv_Disciplina *end_menor_filho;
    int retorno = 0;

    if (*r != NULL)
    {
        if ((*r)->codigo_disciplina == codigo_disciplina)
        {
            if (ehfolha(*r))
            {
                aux = *r;
                *r = NULL;
                free(aux);
                retorno = 1;
            }
            else if ((end_filho = so_um_filho(*r)) != NULL)
            {
                aux = *r;
                *r = end_filho;
                free(aux);
                retorno = 1;
            }
            else
            {
                end_menor_filho = menor_filho_esquerda_disciplina((*r)->dir);
                (*r)->codigo_disciplina = end_menor_filho->codigo_disciplina;
                retorno = remover_disciplina_xiii(&(*r)->dir, end_menor_filho->codigo_disciplina);
            }
        }
        else if (codigo_disciplina < (*r)->codigo_disciplina)
            retorno = remover_disciplina_xiii(&(*r)->esq, codigo_disciplina);
        else
            retorno = remover_disciplina_xiii(&(*r)->dir, codigo_disciplina);
    }
    return (retorno);
}

void confirmar_remocao(Alunos *r, int cod_disc, int *validar_disc)
{
    int enc = 0;
    if (r != NULL)
    {
        buscar_matricula(r->mat, cod_disc, &enc);
        if (enc != 0)
            *validar_disc = 1;
        confirmar_remocao(r->prox, cod_disc, validar_disc);
    }
}

int remover_disciplina_curso(Arv_Cursos **curso, Alunos *alunos, int id_curso, int codigo_disc)
{
    int remove = 0, confirmar_disc = 0;
    if (*curso != NULL)
    {
        confirmar_remocao(alunos, codigo_disc, &confirmar_disc);
        if (confirmar_disc != 0)
        {
            if ((*curso)->codigo_curso == id_curso)
                remove = remover_disciplina_xiii(&(*curso)->disciplina, codigo_disc);
            else if (id_curso < (*curso)->codigo_curso)
                remove = remover_disciplina_curso(&(*curso)->esq, alunos, id_curso, codigo_disc);
            else
                remove = remover_disciplina_curso(&(*curso)->dir, alunos, id_curso, codigo_disc);
        }
    }
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// XV - Mostrar o histórico de um determinado aluno, contendo o nome do curso, as disciplinas e sua respectiva
// nota organizadas pelo período que a disciplina está cadastrada no curso.
// ---------------------------------------------------- XXXXXX -------------------------------------------------
