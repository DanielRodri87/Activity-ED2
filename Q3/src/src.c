#include "src.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>


int contador_disciplina = 1;

void gerar_codigo_disc(int *codigo_disciplina)
{
    *codigo_disciplina = contador_disciplina;
    contador_disciplina++;
}


// ---------------------------------------------------- XXXXXX -------------------------------------------------
// I - Cadastrar alunos a qualquer momento na lista, de forma que só possa cadastrar um código de curso que
// já tenha sido cadastrado na árvore de cursos.
// ---------------------------------------------------- XXXXXX -------------------------------------------------
void converternome(char *nome)
{
    int i = 0;
    while (nome[i] != '\0')
    {
        nome[i] = toupper(nome[i]);
        i++;
    }
}

void cadastrar_aluno(Alunos **aluno, int mat, char *nome, int codigo_curso)
{
    Alunos *novo = (Alunos *)malloc(sizeof(Alunos));
    novo->prox = NULL;
    novo->matricula = mat;
    char *aux_nome = strdup(nome);
    converternome(aux_nome);
    strcpy(novo->nome, aux_nome);
    novo->codigo_curso = codigo_curso;
    novo->notas = NULL;
    novo->mat = NULL;

    if (*aluno == NULL)
        *aluno = novo;
    else
    {
        if (strcmp(aux_nome, (*aluno)->nome) < 0)
        {
            novo->prox = *aluno;
            *aluno = novo;
        }
        else
        {
            Alunos *aux = *aluno;
            while (aux->prox != NULL && strcmp(aux_nome, aux->prox->nome) > 0)
                aux = aux->prox;
            novo->prox = aux->prox;
            aux->prox = novo;
        }
    }
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// II - Cadastrar cursos a qualquer momento na árvore de curso, de forma que o usuário não precise cadastrar
// as disciplinas para permitir o cadastro do curso.
// ---------------------------------------------------- XXXXXX -------------------------------------------------


// ---------------------------------------------------- XXXXXX -------------------------------------------------
// III -  Cadastrar disciplinas a qualquer momento em uma árvore de disciplinas de um determinado curso, ou
// seja, um disciplina só pode ser cadastrada se o curso já estiver sido cadastrado, além disso, o período da
// disciplina deve ser válido, ou seja, estar entre 1 e a quantidade máxima de períodos do curso. A carga
// horária da disciplina deve ser múltiplo de 15, variando entre 30 e 90.
// ---------------------------------------------------- XXXXXX -------------------------------------------------


// ---------------------------------------------------- XXXXXX -------------------------------------------------
// IV - Cadastrar uma matrícula, onde a mesma é uma árvore organizada e contendo somente um código de
// uma disciplina do curso do aluno.
// ---------------------------------------------------- XXXXXX -------------------------------------------------

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// V - Cadastrar Notas, permitir o cadastro de notas somente de disciplinas que estejam na árvore de
// matricula, e quando a nota for cadastrada a disciplina deve ser removida da árvore de matricula para
// árvore de notas.
// ---------------------------------------------------- XXXXXX -------------------------------------------------

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// VI -  Mostrar todos os alunos de um determinado curso.
// ---------------------------------------------------- XXXXXX -------------------------------------------------

void alunosporcurso(Alunos *aluno, int codigo_curso)
{
    if (aluno != NULL)
    {
        if (aluno->codigo_curso == codigo_curso)
        {
            printf("Nome: %s\n", aluno->nome);
            printf("Matricula: %d\n", aluno->matricula);
            printf("\n");
        }
        alunosporcurso(aluno->prox, codigo_curso);
    }
}
// ---------------------------------------------------- XXXXXX -------------------------------------------------
// VII - Mostrar todos os cursos do Campus
// ---------------------------------------------------- XXXXXX -------------------------------------------------
void exibir_curso(Arv_Cursos *raiz)
{
    if (raiz != NULL)
    {
        printf("Nome do curso: %s | ID: %d\n", raiz->info->nome_curso, raiz->info->codigo_curso);
        exibir_curso(raiz->esq);
        exibir_curso(raiz->dir);
    }
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// VIII - Mostrar todas as disciplinas de um determinado curso.
// ---------------------------------------------------- XXXXXX -------------------------------------------------
void exibir_disciplinasporcurso_recursivamente(Arv_Disciplina *disciplina)
{
    if (disciplina != NULL)
    {

        printf("Codigo: %d\n", disciplina->info->codigo_disciplina);
        printf("Nome: %s\n", disciplina->info->nome_disciplina);
        printf("Periodo: %d\n", disciplina->info->periodo);
        printf("Carga Horaria: %d\n", disciplina->info->carga_horaria);
        printf("-------------------------------\n");

        exibir_disciplinasporcurso_recursivamente(disciplina->esq);
        exibir_disciplinasporcurso_recursivamente(disciplina->dir);
    }
}

void exibir_disciplinasporcurso(Arv_Cursos *curso, int codigo_curso)
{
    if (curso != NULL)
    {
        if (curso->info->codigo_curso == codigo_curso)
        {
            Arv_Disciplina *disciplina_atual = curso->info->disciplina;
            printf("Disciplinas do curso %s:\n", curso->info->nome_curso);
            printf("-------------------------------\n");

            exibir_disciplinasporcurso_recursivamente(disciplina_atual);
        }

        exibir_disciplinasporcurso(curso->esq, codigo_curso);
        exibir_disciplinasporcurso(curso->dir, codigo_curso);
    }
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// IX - Mostrar todas as disciplinas de um determinado período de um curso.
// ---------------------------------------------------- XXXXXX -------------------------------------------------
void exibir_disciplina_periodo(Arv_Cursos *raiz, int periodo)
{
    if (raiz != NULL)
    {
        if (raiz->info->disciplina->periodo == periodo)
        {
            printf("Curso: %s | Disciplina: %s | Período: %d\n", raiz->info->nome_curso, raiz->info->disciplina->nome_disciplina, raiz->info->disciplina->periodo);
            exibir_disciplina_periodo(raiz->esq, periodo);
            exibir_disciplina_periodo(raiz->dir, periodo);
        }
    }
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// X - Mostrar todas as disciplinas que um determinado aluno está matriculado
// ---------------------------------------------------- XXXXXX -------------------------------------------------
void exibir_disciplinas(Arv_Disciplina *disciplina, int codigo_disciplina)
{
    if (disciplina != NULL)
    {
        if (disciplina->info->codigo_disciplina == codigo_disciplina)
        {
            printf("Codigo: %d\n", disciplina->info->codigo_disciplina);
            printf("Nome: %s\n", disciplina->info->nome_disciplina);
            printf("Carga horaria: %d\n", disciplina->info->carga_horaria);
            printf("Periodo: %d\n", disciplina->info->periodo);
            printf("\n");
        }
        else if (codigo_disciplina < disciplina->info->codigo_disciplina)
            exibir_disciplinas(disciplina->esq, codigo_disciplina);
        else
            exibir_disciplinas(disciplina->dir, codigo_disciplina);
    }
}

void exibir_disciplina(Arv_Cursos *curso, int codigo_disciplina, int codigo_curso)
{
    if (curso != NULL)
    {
        if (curso->info->codigo_curso == codigo_curso)
        {
            if (curso->info->disciplina != NULL)
                exibir_disciplinas(curso->info->disciplina, codigo_disciplina);
        }
        else if (codigo_curso < curso->info->codigo_curso)
            exibir_disciplina(curso->esq, codigo_disciplina, codigo_curso);
        else
            exibir_disciplina(curso->dir, codigo_disciplina, codigo_curso);
    }
}

void exibir_disciplina_aluno(Arv_Matricula *mat, Arv_Cursos *cursos, int codigo_curso)
{
    if (mat != NULL)
    {
        exibir_disciplina(cursos, mat->info->codigo_disciplina, codigo_curso);
        exibir_disciplina_aluno(mat->esq, cursos, codigo_curso);
        exibir_disciplina_aluno(mat->dir, cursos, codigo_curso);
    }
}

void exibir_disciplinasporaluno(Alunos *aluno, Arv_Cursos *cursos, int matricula)
{
    if (aluno != NULL)
    {
        if (aluno->matricula == matricula)
            exibir_disciplina_aluno(aluno->mat, cursos, aluno->codigo_curso);
        else
            exibir_disciplinasporaluno(aluno->prox, cursos, matricula);
    }
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// XI -  Mostrar todas as notas de disciplinas de um determinado período de um determinado aluno.
// ---------------------------------------------------- XXXXXX -------------------------------------------------

void aux_XI_notaperiodo(Arv_Notas *nota, int periodo)
{
    if (nota != NULL)
    {
        if (nota->info->semestre == periodo)
        {
            printf("Codigo: %d\n", nota->info->codigo_disciplina);
            printf("Nota Final: %.2f\n", nota->info->nota_final);
            printf("Semestre: %.2f\n", nota->info->semestre);
            printf("\n");
        }
        aux_XI_notaperiodo(nota->esq, periodo);
        aux_XI_notaperiodo(nota->dir, periodo);
    }
}

void notas_disciplina_periodo_aluno(Alunos *aluno, int periodo, int matricula)
{
    if (aluno != NULL)
    {
        if (aluno->matricula == matricula)
            aux_XI_notaperiodo(aluno->notas, periodo);
        else
            notas_disciplina_periodo_aluno(aluno->prox, periodo, matricula);
    }
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// xii) Mostrar a nota de uma disciplina de um determinado aluno, mostrando o período e a carga horária da disciplina
// ---------------------------------------------------- XXXXXX -------------------------------------------------
void exibir_nota_aluno_disciplina(Alunos *aluno, Arv_Cursos *curso, int matricula, int codigo_disciplina)
{
    if (aluno != NULL)
    {
        if (aluno->matricula == matricula)
        {
            Arv_Notas *nota = aluno->notas;
            while (nota != NULL)
            {
                if (nota->info->codigo_disciplina == codigo_disciplina)
                {
                    Arv_Disciplina *disciplina = curso->info->disciplina;
                    while (disciplina != NULL)
                    {
                        if (disciplina->info->codigo_disciplina == codigo_disciplina)
                            printf("Aluno: %s\nDisciplina: %d\nPeriodo: %d\nCH: %d\nNota Final: %.2f\n",
                                   aluno->nome, nota->info->codigo_disciplina, disciplina->info->periodo, disciplina->info->carga_horaria, nota->info->nota_final);
                        if (codigo_disciplina < disciplina->info->codigo_disciplina)
                            disciplina = disciplina->esq;
                        else
                            disciplina = disciplina->dir;
                    }
                }
                if (codigo_disciplina < nota->info->codigo_disciplina)
                    nota = nota->esq;
                else
                    nota = nota->dir;
            }
        }
        else
            exibir_nota_aluno_disciplina(aluno->prox, curso, matricula, codigo_disciplina);
    }
}


// ---------------------------------------------------- XXXXXX -------------------------------------------------
// XIII - Remover uma disciplina de um determinado curso desde que não tenha nenhum aluno matriculado na
// mesma.
// ---------------------------------------------------- XXXXXX -------------------------------------------------

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// XIV - Permita remover uma disciplina da árvore de matrícula de um determinado aluno.
// ---------------------------------------------------- XXXXXX -------------------------------------------------

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// XV - Mostrar o histórico de um determinado aluno, contendo o nome do curso, as disciplinas e sua respectiva
// nota organizadas pelo período que a disciplina está cadastrada no curso.
// ---------------------------------------------------- XXXXXX -------------------------------------------------
void exibir_disciplina_historico(Arv_Disciplina *disciplina, int codigo_disciplina)
{
    if(disciplina != NULL)
    {
        if(disciplina->info->codigo_disciplina == codigo_disciplina) 
            printf("Disciplina: %s\n", disciplina->info->nome_disciplina);
        else if(codigo_disciplina < disciplina->info->codigo_disciplina)
            exibir_disciplina_historico(disciplina->esq, codigo_disciplina);
        else
            exibir_disciplina_historico(disciplina->dir, codigo_disciplina);
    }
}

void exibir_notas_alunos(Arv_Notas *nota, Arv_Disciplina *disciplina, int periodo) {
    if(nota != NULL)
    {
        if (nota->info->semestre == periodo)
        {
            exibir_disciplina_historico(disciplina, nota->info->codigo_disciplina);
            printf("\n------------------\n");
            printf("Semestre: %.1f\n\n", nota->info->semestre);
            printf("Nota: %.2f\n", nota->info->nota_final);
            printf("\n------------------\n");

        }
        exibir_notas_alunos(nota->esq, disciplina, periodo);
        exibir_notas_alunos(nota->dir, disciplina, periodo);
    }
}


int exibir_nome_do_curso(Arv_Cursos *curso, int codigo_curso)
{
    int count_periodos = 0;
    if(curso != NULL)
    {
        if(curso->info->codigo_curso == codigo_curso)
        {
            printf("Curso: %s\n", curso->info->nome_curso);
            count_periodos = curso->info->quantidade_periodos;
        }
        else if(codigo_curso < curso->info->codigo_curso)
            count_periodos = exibir_nome_do_curso(curso->esq, codigo_curso);
        else
            count_periodos = exibir_nome_do_curso(curso->dir, codigo_curso);
    }
    return (count_periodos);
}

void consultar_historico(Alunos *aluno, Arv_Cursos *curso, int matricula)
{
    if(aluno != NULL)
    {
        if(aluno->matricula == matricula)
        {
            printf("\n------------------\n");
            printf("Matricula: %d\n", aluno->matricula);
            printf("Aluno: %s\n", aluno->nome);
            printf("\n------------------\n");
            int count_periodos = 0;
            count_periodos = exibir_nome_do_curso(curso, aluno->codigo_curso);
            printf("Historico:\n");
            for (int i = 0; i < count_periodos; i++)
                exibir_notas_alunos(aluno->notas, curso->info->disciplina, i+1);
        }
        else
            consultar_historico(aluno->prox, curso, matricula);
    }
}