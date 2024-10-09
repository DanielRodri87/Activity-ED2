#include "src.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

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

void cadastrar_aluno(Alunos **a, int mat, char *nome, int codigo_curso)
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

    if (*a == NULL)
        *a = novo;
    else
    {
        if (strcmp(aux_nome, (*a)->nome) < 0)
        {
            novo->prox = *a;
            *a = novo;
        }
        else
        {
            Alunos *aux = *a;
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
    if (*mat == NULL)
    {
        Arv_Matricula *novo = (Arv_Matricula *)malloc(sizeof(Arv_Matricula));
        novo->codigo_disciplina = codigo;
        novo->esq = NULL;
        novo->dir = NULL;
        *mat = novo;
    }
    else if (codigo < (*mat)->codigo_disciplina)
        inserirMatricula(&(*mat)->esq, codigo, igual);
    else if (codigo > (*mat)->codigo_disciplina)
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

void buscar_disciplina(Arv_Matricula *m, int cod, int *enc)
{
    if (m != NULL)
    {
        if (m->codigo_disciplina == cod)
            *enc = 1;
        else if (cod < m->codigo_disciplina)
            buscar_disciplina(m->esq, cod, enc);
        else
            buscar_disciplina(m->dir, cod, enc);
    }
}

Alunos *buscar_aluno(Alunos *aluno, int matricula)
{
    while (aluno != NULL && aluno->matricula != matricula) 
        aluno = aluno->prox; 
    
    return aluno; 
}


int ehfolhamat(Arv_Matricula *m)
{
    return (m->esq == NULL && m->dir == NULL);
}

Arv_Matricula *soumfilhomat(Arv_Matricula *m)
{
    Arv_Matricula *aux;
    aux = NULL;
    if (m->esq == NULL)
        aux = m->dir;
    else if (m->dir == NULL)
        aux = m->esq;

    return aux;
}

Arv_Matricula *menorfilhoesqmat(Arv_Matricula *m)
{
    Arv_Matricula *aux;
    aux = NULL;
    if (m)
    {
        aux = menorfilhoesqmat(m->esq);
        if (!aux)
            aux = m;
    }
    return aux;
}

void remover_matricula(Arv_Matricula **m, int cod)
{
    if (*m != NULL)
    {
        if ((*m)->codigo_disciplina == cod)
        {
            Arv_Matricula *aux;

            if (ehfolhamat(*m))
            {
                aux = *m;
                free(aux);
                *m = NULL;
            }
            else if ((aux = soumfilhomat(*m)) != NULL)
            {
                Arv_Matricula *temp;
                temp = *m;
                free(temp);
                *m = aux;
            }
            else
            {
                Arv_Matricula *menorfilho = menorfilhoesqmat((*m)->dir);
                (*m)->codigo_disciplina = menorfilho->codigo_disciplina;
                remover_matricula(&(*m)->dir, menorfilho->codigo_disciplina);
            }
        }
        else if (cod < (*m)->codigo_disciplina)
            remover_matricula(&(*m)->esq, cod);
        else
            remover_matricula(&(*m)->dir, cod);
    }
}

void cadastrar_nota_aux(Arv_Notas **nota, int cod, int semestre, float nota_final)
{
    if (*nota == NULL)
    {
        Arv_Notas *novo = (Arv_Notas *)malloc(sizeof(Arv_Notas));
        novo->codigo_disciplina = cod;
        novo->semestre = semestre;
        novo->nota_final = nota_final;
        novo->esq = NULL;
        novo->dir = NULL;
        *nota = novo;
    }
    else
    {
        if (cod < (*nota)->codigo_disciplina)
            cadastrar_nota_aux(&((*nota)->esq), cod, semestre, nota_final);
        else
            cadastrar_nota_aux(&((*nota)->dir), cod, semestre, nota_final);
    }
}

int cadastrar_nota(Alunos **a, int mat, int cod, int semestre, float nota_final)
{
    int enc = 0;

    if (*a != NULL)
    {
        if ((*a)->matricula == mat)
        {
            int enc_disc = 0;
            buscar_disciplina((*a)->mat, cod, &enc_disc);
            if (enc_disc == 1)
            {
                remover_matricula(&(*a)->mat, cod);
                cadastrar_nota_aux(&(*a)->notas, cod, semestre, nota_final);
                enc = 1;
            }
        }
        else
            enc = cadastrar_nota(&(*a)->prox, mat, cod, semestre, nota_final);
    }

    return enc;
}

// --------------------------------------------------------------------------------------------------------------

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
    if (nota != NULL)
    {
        if (nota->semestre == periodo)
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
    if (aluno != NULL)
    {
        if (aluno->matricula == matricula)
            aux_XI_notaperiodo(aluno->notas, periodo);
        else
            notas_disciplina_periodo_aluno(aluno->prox, periodo, matricula);
    }
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// VI -  Mostrar todos os alunos de um determinado curso.
// ---------------------------------------------------- XXXXXX -------------------------------------------------

void alunosporcurso(Alunos *a, int codigo_curso)
{
    if (a != NULL)
    {
        if (a->codigo_curso == codigo_curso)
        {
            printf("Nome: %s\n", a->nome);
            printf("Matricula: %d\n", a->matricula);
            printf("\n");
        }
        alunosporcurso(a->prox, codigo_curso);
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

void exibir_disciplinas(Arv_Disciplina *disciplina, int codigo_disciplina)
{
    if (disciplina != NULL)
    {
        if (disciplina->codigo_disciplina == codigo_disciplina)
        {
            printf("Codigo: %d\n", disciplina->codigo_disciplina);
            printf("Nome: %s\n", disciplina->nome_disciplina);
            printf("Carga horaria: %d\n", disciplina->carga_horaria);
            printf("Periodo: %d\n", disciplina->periodo);
            printf("\n");
        }
        else if (codigo_disciplina < disciplina->codigo_disciplina)
            exibir_disciplinas(disciplina->esq, codigo_disciplina);
        else
            exibir_disciplinas(disciplina->dir, codigo_disciplina);
    }
}

void exibir_disciplina(Arv_Cursos *curso, int codigo_disciplina, int codigo_curso)
{
    if (curso != NULL)
    {
        if (curso->codigo_curso == codigo_curso)
        {
            if (curso->disciplina != NULL)
                exibir_disciplinas(curso->disciplina, codigo_disciplina);
        }
        else if (codigo_curso < curso->codigo_curso)
            exibir_disciplina(curso->esq, codigo_disciplina, codigo_curso);
        else
            exibir_disciplina(curso->dir, codigo_disciplina, codigo_curso);
    }
}

void exibir_disciplina_aluno(Arv_Matricula *mat, Arv_Cursos *cursos, int codigo_curso)
{
    if (mat != NULL)
    {
        exibir_disciplina(cursos, mat->codigo_disciplina, codigo_curso);
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

void validar_carga_horaria(int *validar, int choraria)
{
    if (choraria % 15 == 0 && (choraria >= 30 && choraria <= 90))
        *validar = 1;
}

void validar_periodo(Arv_Cursos *curso, int *validar, int periodo)
{
    if (periodo >= 1 && periodo <= curso->quantidade_periodos)
        *validar = 1;
}

void inserir_disciplina(Arv_Disciplina **disciplina, Arv_Disciplina *No, int *insere)
{
    if (*disciplina == NULL)
    {
        *disciplina = No;
        No->esq = NULL;
        No->dir = NULL;
        *insere = 1;
    }
    else if (No->codigo_disciplina < (*disciplina)->codigo_disciplina)
        inserir_disciplina(&((*disciplina)->esq), No, insere);
    else if (No->codigo_disciplina > (*disciplina)->codigo_disciplina)
        inserir_disciplina(&((*disciplina)->dir), No, insere);
}

int cadastrar_disciplina(Arv_Cursos **curso, Arv_Disciplina *No, int codigo_curso)
{
    int validar_h = 0, validar_p = 0, sucesso = 0;
    validar_carga_horaria(&validar_h, No->carga_horaria);
    if (validar_h == 1)
    {
        if (*curso != NULL)
        {
            if ((*curso)->codigo_curso == codigo_curso)
            {
                validar_periodo(*curso, &validar_p, No->periodo);
                if (validar_p == 1)
                {
                    inserir_disciplina(&((*curso)->disciplina), No, &sucesso);
                    if (sucesso != 0)
                        sucesso = 1;
                }
            }
            else
            {
                if (codigo_curso < (*curso)->codigo_curso)
                    sucesso = cadastrar_disciplina(&(*curso)->esq, No, codigo_curso);
                else
                    sucesso = cadastrar_disciplina(&(*curso)->dir, No, codigo_curso);
            }
        }
    }

    return sucesso;
}

void gerar_codigo_disc(int ch, int periodo, int *codigo_disciplina)
{
    // Passo 1: obtendo o ano atual
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int anoatual = tm.tm_year + 1900;

    int num5 = rand() % 100000;

    *codigo_disciplina = anoatual * 100000000 + ch * 1000000 + periodo * 100000 + num5;
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------

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
// ---------------------------------------------------- XXXXXX -------------------------------------------------
// xii) Mostrar a nota de uma disciplina de um determinado aluno, mostrando o período e a carga horária da disciplina
// ---------------------------------------------------- XXXXXX -------------------------------------------------

void exibir_nota_aluno_disciplina(Alunos *a, Arv_Cursos *curso, int matricula, int codigo_disciplina)
{
    if (a != NULL)
    {
        if (a->matricula == matricula)
        {
            Arv_Notas *nota = a->notas;
            while (nota != NULL)
            {
                if (nota->codigo_disciplina == codigo_disciplina)
                {
                    Arv_Disciplina *d = curso->disciplina;
                    while (d != NULL)
                    {
                        if (d->codigo_disciplina == codigo_disciplina)
                            printf("Aluno: %s\nDisciplina: %d\nPeriodo: %d\nCH: %d\nNota Final: %.2f\n",
                                   a->nome, nota->codigo_disciplina, d->periodo, d->carga_horaria, nota->nota_final);
                        if (codigo_disciplina < d->codigo_disciplina)
                            d = d->esq;
                        else
                            d = d->dir;
                    }
                }
                if (codigo_disciplina < nota->codigo_disciplina)
                    nota = nota->esq;
                else
                    nota = nota->dir;
            }
        }
        else
            exibir_nota_aluno_disciplina(a->prox, curso, matricula, codigo_disciplina);
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
int verificar_matriculas_alunos(Alunos *aluno, int codigo_disciplina)
{
    if (aluno != NULL)
    {
        int encontrado = 0;
        Arv_Matricula *matricula = aluno->mat;

        while (matricula != NULL && !encontrado)
        {
            if (matricula->codigo_disciplina == codigo_disciplina)
                encontrado = 1;

            else if (codigo_disciplina < matricula->codigo_disciplina)
                matricula = matricula->esq;

            else
                matricula = matricula->dir;
        }

        if (!encontrado)
            encontrado = verificar_matriculas_alunos(aluno->prox, codigo_disciplina);

        return (encontrado);
    }
}

int ehfolha(Arv_Disciplina *disc)
{
    return (disc->esq == NULL && disc->dir == NULL);
}

Arv_Disciplina *soumfilho(Arv_Disciplina *disc)
{
    Arv_Disciplina *retorno;
    if (disc->esq == NULL)
        retorno = disc->dir;
    if (disc->dir == NULL)
        retorno = disc->esq;

    return (retorno);
}

Arv_Disciplina *menorfilhoesq(Arv_Disciplina *disc)
{
    Arv_Disciplina *aux = disc->esq;
    while (aux->dir != NULL)
        aux = aux->dir;

    return aux;
}

int remover_disciplina_curso(Arv_Disciplina **raiz, int codigo_disciplina)
{
    int retorno = 0;
    if (*raiz != NULL)
    {
        if ((*raiz)->codigo_disciplina == codigo_disciplina)
        {
            Arv_Disciplina *aux;

            if (ehfolha(*raiz))
            {
                aux = *raiz;
                free(aux);
                *raiz = NULL;
            }
            else if ((aux = soumfilho(*raiz)) != NULL)
            {
                free(*raiz);
                *raiz = aux;
            }
            else
            {
                Arv_Disciplina *menor = menorfilhoesq((*raiz)->dir);
                (*raiz)->codigo_disciplina = menor->codigo_disciplina;
                strcpy((*raiz)->nome_disciplina, menor->nome_disciplina);
                (*raiz)->carga_horaria = menor->carga_horaria;
                (*raiz)->periodo = menor->periodo;
                remover_disciplina_curso(&(*raiz)->dir, menor->codigo_disciplina);
            }
            retorno = 1; 
        }
        else if (codigo_disciplina < (*raiz)->codigo_disciplina)
        {
            retorno = remover_disciplina_curso(&(*raiz)->esq, codigo_disciplina);
        }
        else
        {
            retorno = remover_disciplina_curso(&(*raiz)->dir, codigo_disciplina);
        }

        return (retorno);
    }
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// XV - Mostrar o histórico de um determinado aluno, contendo o nome do curso, as disciplinas e sua respectiva
// nota organizadas pelo período que a disciplina está cadastrada no curso.
// ---------------------------------------------------- XXXXXX -------------------------------------------------
