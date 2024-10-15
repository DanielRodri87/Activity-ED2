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

Arv_Cursos *alocar_no_curso()
{
    Arv_Cursos *novo = (Arv_Cursos *)malloc(sizeof(Arv_Cursos));
    if (novo == NULL)
        printf("Erro ao alocar memória para o curso.\n");

    novo->info = (Cursos_Info *)malloc(sizeof(Cursos_Info));
    if (novo->info == NULL)
        printf("Erro ao alocar memória para as informações do curso.\n");

    novo->esq = NULL;
    novo->dir = NULL;
    novo->altura = 1;

    return (novo);
}

int altura_curso(Arv_Cursos *curso)
{
    int h = 0;
    if (curso != NULL)
        h = curso->altura;

    return (h);
}

void atualizar_altura_curso(Arv_Cursos *curso)
{
    if (curso != NULL)
    {
        int altura_esq = altura_curso(curso->esq);
        int altura_dir = altura_curso(curso->dir);

        if (altura_esq > altura_dir)
            curso->altura = 1 + altura_esq;
        else
            curso->altura = 1 + altura_dir;
    }
}

int fator_balanceamento_curso(Arv_Cursos *curso)
{
    int fb = 0;
    if (curso != NULL)
        fb = altura_curso(curso->esq) - altura_curso(curso->dir);
    return fb;
}

void rotacaoDir_curso(Arv_Cursos **raiz)
{
    Arv_Cursos *nova_raiz = (*raiz)->esq;
    (*raiz)->esq = nova_raiz->dir;
    nova_raiz->dir = *raiz;
    atualizar_altura_curso(*raiz);
    atualizar_altura_curso(nova_raiz);
    *raiz = nova_raiz;
}

void rotacaoEsq_curso(Arv_Cursos **raiz)
{
    Arv_Cursos *nova_raiz = (*raiz)->dir;
    (*raiz)->dir = nova_raiz->esq;
    nova_raiz->esq = *raiz;
    atualizar_altura_curso(*raiz);
    atualizar_altura_curso(nova_raiz);
    *raiz = nova_raiz;
}

void balanceamento_curso(Arv_Cursos **curso)
{
    int fb = fator_balanceamento_curso(*curso);

    if (fb == 2)
    {
        if (fator_balanceamento_curso((*curso)->esq) < 0)
            rotacaoEsq_curso(&((*curso)->esq));
        rotacaoDir_curso(curso);
    }
    else if (fb == -2)
    {
        if (fator_balanceamento_curso((*curso)->dir) > 0)
            rotacaoDir_curso(&((*curso)->dir));
        rotacaoEsq_curso(curso);
    }
}

void cadastrar_curso(Arv_Cursos **curso, int codigo_curso, const char *nome_curso, int quantidade_periodos)
{
    if (*curso == NULL)
    {
        *curso = alocar_no_curso();
        (*curso)->info->codigo_curso = codigo_curso;
        strcpy((*curso)->info->nome_curso, nome_curso);
        (*curso)->info->quantidade_periodos = quantidade_periodos;
        (*curso)->info->alunos = NULL;
        (*curso)->esq = NULL;
        (*curso)->dir = NULL;
    }
    else if (codigo_curso < (*curso)->info->codigo_curso)
        cadastrar_curso(&((*curso)->esq), codigo_curso, nome_curso, quantidade_periodos);
    else if (codigo_curso > (*curso)->info->codigo_curso)
        cadastrar_curso(&((*curso)->dir), codigo_curso, nome_curso, quantidade_periodos);

    atualizar_altura_curso(*curso);
    balanceamento_curso(curso);
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// III -  Cadastrar disciplinas a qualquer momento em uma árvore de disciplinas de um determinado curso, ou
// seja, um disciplina só pode ser cadastrada se o curso já estiver sido cadastrado, além disso, o período da
// disciplina deve ser válido, ou seja, estar entre 1 e a quantidade máxima de períodos do curso. A carga
// horária da disciplina deve ser múltiplo de 15, variando entre 30 e 90.
// ---------------------------------------------------- XXXXXX -------------------------------------------------

// Função para alocar um novo nó de disciplina
Arv_Disciplina *alocar_no_disciplina()
{
    Arv_Disciplina *novo = (Arv_Disciplina *)malloc(sizeof(Arv_Disciplina));
    Disciplinas_Info *info = NULL;

    if (novo != NULL)
    {
        info = (Disciplinas_Info *)malloc(sizeof(Disciplinas_Info));
        if (info == NULL)
        {
            printf("Erro ao alocar memória para as informações da disciplina.\n");
            free(novo);
            novo = NULL;
        }
        else
        {
            novo->info = info;
            novo->esq = NULL;
            novo->dir = NULL;
            novo->altura = 1;
        }
    }
    else
    {
        printf("Erro ao alocar memória para a disciplina.\n");
    }

    return novo;
}

// Função para obter a altura de uma árvore de disciplinas
int altura_disciplina(Arv_Disciplina *disciplina)
{
    int h = 0;
    if (disciplina != NULL)
        h = disciplina->altura;
    return h;
}

// Função para atualizar a altura de uma árvore de disciplinas
void atualizar_altura_disciplina(Arv_Disciplina *disciplina)
{
    if (disciplina != NULL)
    {
        int altura_esq = altura_disciplina(disciplina->esq);
        int altura_dir = altura_disciplina(disciplina->dir);

        if (altura_esq > altura_dir)
            disciplina->altura = 1 + altura_esq;
        else
            disciplina->altura = 1 + altura_dir;
    }
}

// Função para calcular o fator de balanceamento de uma árvore de disciplinas
int fator_balanceamento_disciplina(Arv_Disciplina *disciplina)
{
    int fb = 0;
    if (disciplina != NULL)
        fb = altura_disciplina(disciplina->esq) - altura_disciplina(disciplina->dir);
    return fb;
}

// Rotação à direita
void rotacaoDir_disciplina(Arv_Disciplina **raiz)
{
    Arv_Disciplina *nova_raiz = (*raiz)->esq;
    (*raiz)->esq = nova_raiz->dir;
    nova_raiz->dir = *raiz;

    atualizar_altura_disciplina(*raiz);
    atualizar_altura_disciplina(nova_raiz);

    *raiz = nova_raiz;
}

// Rotação à esquerda
void rotacaoEsq_disciplina(Arv_Disciplina **raiz)
{
    Arv_Disciplina *nova_raiz = (*raiz)->dir;
    (*raiz)->dir = nova_raiz->esq;
    nova_raiz->esq = *raiz;

    atualizar_altura_disciplina(*raiz);
    atualizar_altura_disciplina(nova_raiz);

    *raiz = nova_raiz;
}

// Função de balanceamento da árvore de disciplinas
void balanceamento_disciplina(Arv_Disciplina **disciplina)
{
    int fb = fator_balanceamento_disciplina(*disciplina);

    if (fb == 2)
    {
        if (fator_balanceamento_disciplina((*disciplina)->esq) < 0)
            rotacaoEsq_disciplina(&((*disciplina)->esq));
        rotacaoDir_disciplina(disciplina);
    }
    else if (fb == -2)
    {
        if (fator_balanceamento_disciplina((*disciplina)->dir) > 0)
            rotacaoDir_disciplina(&((*disciplina)->dir));
        rotacaoEsq_disciplina(disciplina);
    }
}

// Função para cadastrar disciplina no curso
void cadastrar_disciplina(Arv_Disciplina **disciplina, int codigo_disciplina, const char *nome_disciplina, int periodo, int carga_horaria, int max_periodos)
{
    if (periodo < 1 || periodo > max_periodos || carga_horaria % 15 != 0 || carga_horaria < 30 || carga_horaria > 90)
        printf("Erro: dados inválidos para disciplina.\n");

    if (*disciplina == NULL)
    {
        *disciplina = alocar_no_disciplina();
        if (*disciplina != NULL)
        {
            (*disciplina)->info->codigo_disciplina = codigo_disciplina;
            strcpy((*disciplina)->info->nome_disciplina, nome_disciplina);
            (*disciplina)->info->periodo = periodo;
            (*disciplina)->info->carga_horaria = carga_horaria;
        }
    }
    else
    {
        if (codigo_disciplina < (*disciplina)->info->codigo_disciplina)
            cadastrar_disciplina(&((*disciplina)->esq), codigo_disciplina, nome_disciplina, periodo, carga_horaria, max_periodos);
        else if (codigo_disciplina > (*disciplina)->info->codigo_disciplina)
            cadastrar_disciplina(&((*disciplina)->dir), codigo_disciplina, nome_disciplina, periodo, carga_horaria, max_periodos);
    }

    atualizar_altura_disciplina(*disciplina);
    balanceamento_disciplina(disciplina);
}

// Função para buscar um curso e cadastrar a disciplina nele
void cadastrar_disciplina_no_curso(Arv_Cursos *curso, int codigo_curso, int codigo_disciplina, const char *nome_disciplina, int periodo, int carga_horaria)
{
    while (curso != NULL && curso->info->codigo_curso != codigo_curso)
    {
        if (codigo_curso < curso->info->codigo_curso)
            curso = curso->esq;
        else
            curso = curso->dir;
    }

    if (curso == NULL)
        printf("Erro: Curso não encontrado.\n");

    cadastrar_disciplina(&(curso->info->disciplina), codigo_disciplina, nome_disciplina, periodo, carga_horaria, curso->info->quantidade_periodos);
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// IV - Cadastrar uma matrícula, onde a mesma é uma árvore organizada e contendo somente um código de
// uma disciplina do curso do aluno.
// ---------------------------------------------------- XXXXXX -------------------------------------------------

int altura_matricula(Arv_Matricula *matricula)
{
    int h = 0;
    if (matricula != NULL)
        h = matricula->altura;

    return (h);
}

void atualizar_altura_matricula(Arv_Matricula *matricula)
{
    if (matricula != NULL)
    {
        int altura_esq = altura_matricula(matricula->esq);
        int altura_dir = altura_matricula(matricula->dir);

        if (altura_esq > altura_dir)
            matricula->altura = 1 + altura_esq;
        else
            matricula->altura = 1 + altura_dir;
    }
}

int fator_balanceamento_matricula(Arv_Matricula *matricula)
{
    int fb = 0;
    if (matricula != NULL)
        fb = altura_matricula(matricula->esq) - altura_matricula(matricula->dir);

    return fb;
}


void rotacaoDir_matricula(Arv_Matricula **raiz)
{
    Arv_Matricula *nova_raiz = (*raiz)->esq;
    (*raiz)->esq = nova_raiz->dir;
    nova_raiz->dir = *raiz;
    atualizar_altura_matricula(*raiz);
    atualizar_altura_matricula(nova_raiz);
    *raiz = nova_raiz;
}

void rotacaoEsq_matricula(Arv_Matricula **raiz)
{
    Arv_Matricula *nova_raiz = (*raiz)->dir;
    (*raiz)->dir = nova_raiz->esq;
    nova_raiz->esq = *raiz;
    atualizar_altura_matricula(*raiz);
    atualizar_altura_matricula(nova_raiz);
    *raiz = nova_raiz;
}

void balanceamento_matricula(Arv_Matricula **matricula)
{
    int fb = fator_balanceamento_matricula(*matricula);
    
    if (fb == 2)
    {
        if (fator_balanceamento_matricula((*matricula)->esq) < 0)
            rotacaoEsq_matricula(&((*matricula)->esq));
        rotacaoDir_matricula(matricula);
    }
    else if (fb == -2)
    {
        if (fator_balanceamento_matricula((*matricula)->dir) > 0)
            rotacaoDir_matricula(&((*matricula)->dir));
        rotacaoEsq_matricula(matricula);
    }
}

void inserirMatricula_AVL(Arv_Matricula **matricula, int codigo, int *igual)
{
    if (*matricula == NULL)
    {
        Arv_Matricula *novo = (Arv_Matricula *)malloc(sizeof(Arv_Matricula));
        novo->info = (Matriculas_Info *)malloc(sizeof(Matriculas_Info));
        novo->info->codigo_disciplina = codigo;
        novo->altura = 1;
        novo->esq = NULL;
        novo->dir = NULL;
        *matricula = novo;
    }
    else if (codigo < (*matricula)->info->codigo_disciplina)
        inserirMatricula_AVL(&(*matricula)->esq, codigo, igual);
    else if (codigo > (*matricula)->info->codigo_disciplina)
        inserirMatricula_AVL(&(*matricula)->dir, codigo, igual);
    else
    {
        *igual = -1;
        return;
    }

    atualizar_altura_matricula(*matricula);
    balanceamento_matricula(matricula);
}

void cadastrar_matricula(Alunos **a, int codigo, int mat)
{
    if (*a != NULL)
    {
        if ((*a)->matricula == mat)
        {
            int igual = 0;
            inserirMatricula_AVL(&(*a)->mat, codigo, &igual);
        }
        else
            cadastrar_matricula(&(*a)->prox, codigo, mat);
    }
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// V - Cadastrar Notas, permitir o cadastro de notas somente de disciplinas que estejam na árvore de
// matricula, e quando a nota for cadastrada a disciplina deve ser removida da árvore de matricula para
// árvore de notas.
// ---------------------------------------------------- XXXXXX -------------------------------------------------
int altura_nota(Arv_Notas *nota)
{
    int h = 0;
    if (nota != NULL)
        h = nota->altura;

    return (h);
}

void atualizar_altura_nota(Arv_Notas *nota)
{
    if (nota != NULL)
    {
        int altura_esq = altura_nota(nota->esq);
        int altura_dir = altura_nota(nota->dir);

        if (altura_esq > altura_dir)
            nota->altura = 1 + altura_esq;
        else
            nota->altura = 1 + altura_dir;
    }
}

int fator_balanceamento_nota(Arv_Notas *nota)
{
    int fb = 0;
    if (nota != NULL)
        fb = altura_nota(nota->esq) - altura_nota(nota->dir);
    return fb;
}

void rotacaoDir_nota(Arv_Notas **raiz)
{
    Arv_Notas *nova_raiz = (*raiz)->esq;
    (*raiz)->esq = nova_raiz->dir;
    nova_raiz->dir = *raiz;
    atualizar_altura_nota(*raiz);
    atualizar_altura_nota(nova_raiz);
    *raiz = nova_raiz;
}

void rotacaoEsq_nota(Arv_Notas **raiz)
{
    Arv_Notas *nova_raiz = (*raiz)->dir;
    (*raiz)->dir = nova_raiz->esq;
    nova_raiz->esq = *raiz;
    atualizar_altura_nota(*raiz);
    atualizar_altura_nota(nova_raiz);
    *raiz = nova_raiz;
}

void balanceamento_nota(Arv_Notas **nota)
{
    int fb = fator_balanceamento_nota(*nota);

    if (fb == 2)
    {
        if (fator_balanceamento_nota((*nota)->esq) < 0)
            rotacaoEsq_nota(&((*nota)->esq));
        rotacaoDir_nota(nota);
    }
    else if (fb == -2)
    {
        if (fator_balanceamento_nota((*nota)->dir) > 0)
            rotacaoDir_nota(&((*nota)->dir));
        rotacaoEsq_nota(nota);
    }
}

void buscar_disciplina(Arv_Matricula *matricula, int cod, int *enc)
{
    if (matricula != NULL)
    {
        if (matricula->info->codigo_disciplina == cod)
            *enc = 1;
        else if (cod < matricula->info->codigo_disciplina)
            buscar_disciplina(matricula->esq, cod, enc);
        else
            buscar_disciplina(matricula->dir, cod, enc);
    }
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// XIV - Permita remover uma disciplina da árvore de matrícula de um determinado aluno.
// ---------------------------------------------------- XXXXXX -------------------------------------------------
int e_folha_matricula(Arv_Matricula *matricula)
{
    return (matricula->esq == NULL && matricula->dir == NULL);
}

Arv_Matricula *soumfilhomat(Arv_Matricula *matricula)
{
    Arv_Matricula *aux;
    aux = NULL;
    if (matricula->esq == NULL)
        aux = matricula->dir;
    else if (matricula->dir == NULL)
        aux = matricula->esq;

    return aux;
}

Arv_Matricula *menorfilhoesqmat(Arv_Matricula *matricula)
{
    Arv_Matricula *aux;
    aux = NULL;
    if (matricula)
    {
        aux = menorfilhoesqmat(matricula->esq);
        if (!aux)
            aux = matricula;
    }
    return aux;
}

void remover_matricula(Arv_Matricula **matricula, int codigo)
{
    if (*matricula != NULL)
    {
        if ((*matricula)->info->codigo_disciplina == codigo)
        {
            Arv_Matricula *aux;

            if (e_folha_matricula(*matricula))
            {
                aux = *matricula;
                free(aux);
                *matricula = NULL;
            }
            else if ((aux = soumfilhomat(*matricula)) != NULL)
            {
                Arv_Matricula *temp;
                temp = *matricula;
                free(temp);
                *matricula = aux;
            }
            else
            {
                Arv_Matricula *menorfilho = menorfilhoesqmat((*matricula)->dir);
                (*matricula)->info->codigo_disciplina = menorfilho->info->codigo_disciplina;
                remover_matricula(&(*matricula)->dir, menorfilho->info->codigo_disciplina);
            }
        }
        else if (codigo < (*matricula)->info->codigo_disciplina)
            remover_matricula(&(*matricula)->esq, codigo);
        else
            remover_matricula(&(*matricula)->dir, codigo);
    }
    if (*matricula != NULL)
    {
        balanceamento_matricula(matricula);
        atualizar_altura_matricula(*matricula);
    }
}

int cadastrar_nota_aux(Arv_Notas **nota, Notas_Info *n)
{
    int sucesso = 1;
    if (*nota == NULL)
    {
        Arv_Notas *novo = (Arv_Notas *)malloc(sizeof(Arv_Notas));
        novo->info = n;
        novo->esq = NULL;
        novo->dir = NULL;
        *nota = novo;
    }
    else
    {
        if (n->codigo_disciplina == (*nota)->info->codigo_disciplina)
            sucesso = 0;
        else
        {
            if (n->codigo_disciplina < (*nota)->info->codigo_disciplina)
                sucesso = cadastrar_nota_aux(&((*nota)->esq), n);
            else
                sucesso = cadastrar_nota_aux(&((*nota)->dir), n);
        }

        balanceamento_nota(nota);
        atualizar_altura_nota(*nota);
    }
    return sucesso;
}

int cadastrar_notas(Alunos **aluno, int matricula, Notas_Info *n)
{
    int(enc) = 0;
    if (*aluno != NULL)
    {
        if ((*aluno)->matricula == matricula)
        {
            int enc_disc = 0;
            buscar_disciplina((*aluno)->mat, n->codigo_disciplina, &enc_disc);
            if (enc_disc == 1)
            {
                if (cadastrar_nota_aux(&(*aluno)->notas, n) == 1)
                {
                    remover_matricula(&(*aluno)->mat, n->codigo_disciplina);
                    enc = 1;
                }
            }
        }
        else
            enc = cadastrar_notas(&(*aluno)->prox, matricula, n);
    }

    return (enc);
}

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
        if (raiz->info->disciplina->info->periodo == periodo)
        {
            printf("Curso: %s | Disciplina: %s | Período: %d\n", raiz->info->nome_curso, raiz->info->disciplina->info->nome_disciplina, raiz->info->disciplina->info->periodo);
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

int e_folha(Arv_Disciplina *disciplina)
{
    return (disciplina->esq == NULL && disciplina->dir == NULL);
}

Arv_Disciplina *so_um_filho(Arv_Disciplina *disciplina)
{
    Arv_Disciplina *aux = NULL;

    if (disciplina->dir == NULL)
        aux = disciplina->esq;
    else if (disciplina->esq == NULL)
        aux = disciplina->dir;

    return (aux);
}

Arv_Disciplina *menor_filho_esquerda(Arv_Disciplina *disciplina)
{
    Arv_Disciplina *aux = NULL;

    if (disciplina != NULL)
        aux = menor_filho_esquerda(disciplina->esq);
    if (!aux)
        aux = disciplina;

    return (aux);
}

void remover_disciplina(Arv_Disciplina **disciplina, int codigo_disciplina, int *remove)
{
    if ((*disciplina != NULL))
    {
        Arv_Disciplina *aux;
        Arv_Disciplina *endereco_filho;
        Arv_Disciplina *endereco_menor_filho;
        if ((*disciplina)->codigo_disciplina == codigo_disciplina)
        {
            if (e_folha(*disciplina))
            {
                aux = *disciplina;
                free(aux);
                *disciplina = NULL;
            }
            else if ((endereco_filho = so_um_filho(*disciplina)) != NULL)
            {
                aux = *disciplina;
                free(aux);
                *disciplina = endereco_filho;
            }
            else
            {
                endereco_menor_filho = menor_filho_esquerda((*disciplina)->dir);
                (*disciplina)->codigo_disciplina = endereco_menor_filho->codigo_disciplina;
                (*disciplina)->carga_horaria = endereco_menor_filho->carga_horaria;
                (*disciplina)->periodo = endereco_menor_filho->periodo;
                strcpy((*disciplina)->nome_disciplina, endereco_menor_filho->nome_disciplina);
                remover_disciplina(&(*disciplina)->dir, endereco_menor_filho->codigo_disciplina, remove);
            }
            *remove = 1;
        }
        else if (codigo_disciplina < (*disciplina)->codigo_disciplina)
            remover_disciplina(&(*disciplina)->esq, codigo_disciplina, remove);
        else
            remover_disciplina(&(*disciplina)->dir, codigo_disciplina, remove);
    }
    if (*disciplina != NULL)
    {
        balanceamento_disciplina(disciplina);
        atualizar_altura_disciplina(*disciplina);
    }
}

void confirmar_remocao_disciplina(Alunos *alunos, int codigo_disciplina, int *confirmar)
{
    int enc = 0;

    if (alunos != NULL)
    {
        buscar_matricula(alunos->mat, codigo_disciplina, &enc);

        if (enc != 0)
            *confirmar = 1;

        confirmar_remocao_disciplina(alunos->prox, codigo_disciplina, confirmar);
    }
}


int remover_disciplina_curso(Arv_Cursos **cursos, Alunos *alunos, int idcurso, int codigo_disciplina)
{
    int remove = 0, confirmar_disciplina = 0;
    if ((*cursos))
    {
        confirmar_remocao_disciplina(alunos, codigo_disciplina, &confirmar_disciplina);
        if (confirmar_disciplina != 0)
        {
            if ((*cursos)->codigo_curso == idcurso)
                remover_disciplina(&(*cursos)->disciplina, codigo_disciplina, &remove);
            else if (idcurso < (*cursos)->codigo_curso)
                remove = remover_disciplina_curso(&(*cursos)->esq, alunos, idcurso, codigo_disciplina);
            else
                remove = remover_disciplina_curso(&(*cursos)->dir, alunos, idcurso, codigo_disciplina);
        }
    }
    return (remove);
}
// ---------------------------------------------------- XXXXXX -------------------------------------------------
// XV - Mostrar o histórico de um determinado aluno, contendo o nome do curso, as disciplinas e sua respectiva
// nota organizadas pelo período que a disciplina está cadastrada no curso.
// ---------------------------------------------------- XXXXXX -------------------------------------------------
void exibir_disciplina_historico(Arv_Disciplina *disciplina, int codigo_disciplina)
{
    if (disciplina != NULL)
    {
        if (disciplina->info->codigo_disciplina == codigo_disciplina)
            printf("Disciplina: %s\n", disciplina->info->nome_disciplina);
        else if (codigo_disciplina < disciplina->info->codigo_disciplina)
            exibir_disciplina_historico(disciplina->esq, codigo_disciplina);
        else
            exibir_disciplina_historico(disciplina->dir, codigo_disciplina);
    }
}

void exibir_notas_alunos(Arv_Notas *nota, Arv_Disciplina *disciplina, int periodo)
{
    if (nota != NULL)
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
    if (curso != NULL)
    {
        if (curso->info->codigo_curso == codigo_curso)
        {
            printf("Curso: %s\n", curso->info->nome_curso);
            count_periodos = curso->info->quantidade_periodos;
        }
        else if (codigo_curso < curso->info->codigo_curso)
            count_periodos = exibir_nome_do_curso(curso->esq, codigo_curso);
        else
            count_periodos = exibir_nome_do_curso(curso->dir, codigo_curso);
    }
    return (count_periodos);
}

void consultar_historico(Alunos *aluno, Arv_Cursos *curso, int matricula)
{
    if (aluno != NULL)
    {
        if (aluno->matricula == matricula)
        {
            printf("\n------------------\n");
            printf("Matricula: %d\n", aluno->matricula);
            printf("Aluno: %s\n", aluno->nome);
            printf("\n------------------\n");
            int count_periodos = 0;
            count_periodos = exibir_nome_do_curso(curso, aluno->codigo_curso);
            printf("Historico:\n");
            for (int i = 0; i < count_periodos; i++)
                exibir_notas_alunos(aluno->notas, curso->info->disciplina, i + 1);
        }
        else
            consultar_historico(aluno->prox, curso, matricula);
    }
}