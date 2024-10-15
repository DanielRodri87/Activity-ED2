#include "src.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

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

int contador_disciplina = 1;

void gerar_codigo_disc(int *codigo_disciplina)
{
    *codigo_disciplina = contador_disciplina;
    contador_disciplina++;
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// IV - Cadastrar uma matrícula, onde a mesma é uma árvore organizada e contendo somente um código de
// uma disciplina do curso do aluno.
// ---------------------------------------------------- XXXXXX -------------------------------------------------
void inserirMatricula(Arv_Matricula **matricula, int codigo, int *igual)
{
    if (*matricula == NULL)
    {
        Arv_Matricula *novo = (Arv_Matricula *)malloc(sizeof(Arv_Matricula));
        novo->codigo_disciplina = codigo;
        novo->esq = NULL;
        novo->dir = NULL;
        *matricula = novo;
    }
    else if (codigo < (*matricula)->codigo_disciplina)
        inserirMatricula(&(*matricula)->esq, codigo, igual);
    else if (codigo > (*matricula)->codigo_disciplina)
        inserirMatricula(&(*matricula)->dir, codigo, igual);
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

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// V - Cadastrar Notas, permitir o cadastro de notas somente de disciplinas que estejam na árvore de
// matricula, e quando a nota for cadastrada a disciplina deve ser removida da árvore de matricula para
// árvore de notas.
// ---------------------------------------------------- XXXXXX -------------------------------------------------

void buscar_disciplina(Arv_Matricula *matricula, int cod, int *enc)
{
    if (matricula != NULL)
    {
        if (matricula->codigo_disciplina == cod)
            *enc = 1;
        else if (cod < matricula->codigo_disciplina)
            buscar_disciplina(matricula->esq, cod, enc);
        else
            buscar_disciplina(matricula->dir, cod, enc);
    }
}

Alunos *buscar_aluno(Alunos *aluno, int matricula)
{
    while (aluno != NULL && aluno->matricula != matricula)
        aluno = aluno->prox;

    return aluno;
}

void cadastrar_nota_aux(Arv_Notas **nota, int codigo, int semestre, float nota_final)
{
    if (*nota == NULL)
    {
        Arv_Notas *novo = (Arv_Notas *)malloc(sizeof(Arv_Notas));
        novo->codigo_disciplina = codigo;
        novo->semestre = semestre;
        novo->nota_final = nota_final;
        novo->esq = NULL;
        novo->dir = NULL;
        *nota = novo;
    }
    else
    {
        if (codigo < (*nota)->codigo_disciplina)
            cadastrar_nota_aux(&((*nota)->esq), codigo, semestre, nota_final);
        else
            cadastrar_nota_aux(&((*nota)->dir), codigo, semestre, nota_final);
    }
}

int cadastrar_nota(Alunos **aluno, int matricula, int codigo, int semestre, float nota_final)
{
    int enc = 0;

    if (*aluno != NULL)
    {
        if ((*aluno)->matricula == matricula)
        {
            int enc_disc = 0;
            buscar_disciplina((*aluno)->mat, codigo, &enc_disc);
            if (enc_disc == 1)
            {
                remover_matricula(&(*aluno)->mat, codigo);
                cadastrar_nota_aux(&(*aluno)->notas, codigo, semestre, nota_final);
                enc = 1;
            }
        }
        else
            enc = cadastrar_nota(&(*aluno)->prox, matricula, codigo, semestre, nota_final);
    }

    return enc;
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
// VIII - Mostrar todas as disciplinas de um determinado curso.
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
                if (nota->codigo_disciplina == codigo_disciplina)
                {
                    Arv_Disciplina *disciplina = curso->disciplina;
                    while (disciplina != NULL)
                    {
                        if (disciplina->codigo_disciplina == codigo_disciplina)
                            printf("Aluno: %s\nDisciplina: %d\nPeriodo: %d\nCH: %d\nNota Final: %.2f\n",
                                   aluno->nome, nota->codigo_disciplina, disciplina->periodo, disciplina->carga_horaria, nota->nota_final);
                        if (codigo_disciplina < disciplina->codigo_disciplina)
                            disciplina = disciplina->esq;
                        else
                            disciplina = disciplina->dir;
                    }
                }
                if (codigo_disciplina < nota->codigo_disciplina)
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
        if ((*matricula)->codigo_disciplina == codigo)
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
                (*matricula)->codigo_disciplina = menorfilho->codigo_disciplina;
                remover_matricula(&(*matricula)->dir, menorfilho->codigo_disciplina);
            }
        }
        else if (codigo < (*matricula)->codigo_disciplina)
            remover_matricula(&(*matricula)->esq, codigo);
        else
            remover_matricula(&(*matricula)->dir, codigo);
    }
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// XV - Mostrar o histórico de um determinado aluno, contendo o nome do curso, as disciplinas e sua respectiva
// nota organizadas pelo período que a disciplina está cadastrada no curso.
// ---------------------------------------------------- XXXXXX -------------------------------------------------

void exibir_disciplina_historico(Arv_Disciplina *disciplina, int codigo_disciplina)
{
    if(disciplina != NULL)
    {
        if(disciplina->codigo_disciplina == codigo_disciplina) 
            printf("Disciplina: %s\n", disciplina->nome_disciplina);
        else if(codigo_disciplina < disciplina->codigo_disciplina)
            exibir_disciplina_historico(disciplina->esq, codigo_disciplina);
        else
            exibir_disciplina_historico(disciplina->dir, codigo_disciplina);
    }
}

void exibir_notas_alunos(Arv_Notas *nota, Arv_Disciplina *disciplina, int periodo) {
    if(nota != NULL)
    {
        if (nota->semestre == periodo)
        {
            exibir_disciplina_historico(disciplina, nota->codigo_disciplina);
            printf("\n------------------\n");
            printf("Semestre: %.1f\n\n", nota->semestre);
            printf("Nota: %.2f\n", nota->nota_final);
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
        if(curso->codigo_curso == codigo_curso)
        {
            printf("Curso: %s\n", curso->nome_curso);
            count_periodos = curso->quantidade_periodos;
        }
        else if(codigo_curso < curso->codigo_curso)
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
                exibir_notas_alunos(aluno->notas, curso->disciplina, i+1);
        }
        else
            consultar_historico(aluno->prox, curso, matricula);
    }
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// FUNÇÕES AUXILIARES
// ---------------------------------------------------- XXXXXX -------------------------------------------------

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

Alunos *buscar_aluno_por_matricula_no_curso(Arv_Cursos *curso, int codigo_curso, int matricula)
{
    Alunos *aluno;
    if (curso != NULL)
    {
        if (curso->codigo_curso == codigo_curso)
        {
            aluno = curso->alunos;
            while (aluno != NULL && aluno->matricula != matricula)
                aluno = aluno->prox;
        }
        else
        {
            aluno = buscar_aluno_por_matricula_no_curso(curso->esq, codigo_curso, matricula);
            if (aluno == NULL)
                aluno = buscar_aluno_por_matricula_no_curso(curso->dir, codigo_curso, matricula);
        }

        return aluno;
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

void localizar_no(Arv_Matricula **atual, Arv_Matricula **pai, int codigo_disciplina, int *encontrado)
{
    while (*atual != NULL && *encontrado == 0)
    {
        if ((*atual)->codigo_disciplina == codigo_disciplina)
            *encontrado = 1;
        else
        {
            *pai = *atual;

            if (codigo_disciplina < (*atual)->codigo_disciplina)
                *atual = (*atual)->esq;
            else
                *atual = (*atual)->dir;
        }
    }
}

void remover_no(Arv_Matricula **raiz, Arv_Matricula *pai, Arv_Matricula *atual)
{
    if (atual->esq == NULL && atual->dir == NULL)
        remover_no_folha(raiz, pai, atual);

    else if (atual->esq == NULL || atual->dir == NULL)
        remover_no_com_um_filho(raiz, pai, atual);
    else
        remover_no_com_dois_filhos(atual);

    free(atual);
}

void remover_no_folha(Arv_Matricula **raiz, Arv_Matricula *pai, Arv_Matricula *atual)
{
    if (pai == NULL)
        *raiz = NULL;

    else if (pai->esq == atual)
        pai->esq = NULL;
    else
        pai->dir = NULL;
}

void remover_no_com_um_filho(Arv_Matricula **raiz, Arv_Matricula *pai, Arv_Matricula *atual)
{
    Arv_Matricula *filho = (atual->esq != NULL) ? atual->esq : atual->dir;

    if (pai == NULL)
        *raiz = filho;

    else if (pai->esq == atual)
        pai->esq = filho;

    else
        pai->dir = filho;
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
        menor_nopai->esq = menor_no->dir;
    else
        menor_nopai->dir = menor_no->dir;
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


//inicio da Q2

#define QUANTIDADECURSOS 2000
#define CODIGOCURSO 2000
#define LOOP 15000
#define QUANTIDADEDISCIPLINAS 2000
#define QUANTIDADEALUNOS 2000



//1. inserção de cada elemento na árvore de cursos

//forma crescente
void povoamentocrescente_cursos(Arv_Cursos **raiz) {
    for (int i = 1; i <= QUANTIDADECURSOS; i++) {
        cadastrar_curso(raiz, i, "MEDICINA", 12); 
    }
}

//forma descrecente
void povoamentodescrecente_cursos(Arv_Cursos **raiz) {
    for (int i = QUANTIDADECURSOS; i > 0; i--) {
        cadastrar_curso(raiz, i, "MEDICINA", 12); 
    }
}

//forma aleatória
void povoamentoaleatorio_cursos(Arv_Cursos **raiz) {
    int i = 1;
    while (i <= QUANTIDADECURSOS) {
        int codigo_curso = rand() % QUANTIDADECURSOS + 1; 
        cadastrar_curso(raiz, codigo_curso, "MEDICINA", 12); 
        i++;
    }
}


//2. busca de uma nota de uma disciplina de uma aluno

//forma crescente
void povoamentocrescente_disciplinas(Arv_Cursos **raiz) {
    if (*raiz != NULL) {
        int codigo_curso_atual = (*raiz)->codigo_curso;

        for (int i = 1; i < QUANTIDADEDISCIPLINAS; i++) {
            Arv_Disciplina *disciplina = (Arv_Disciplina *)malloc(sizeof(Arv_Disciplina));
            strcpy(disciplina->nome_disciplina, "Anatomia");
            disciplina->carga_horaria = 90;
            disciplina->periodo = 1;
            disciplina->codigo_disciplina = i;
            disciplina->esq = NULL;
            disciplina->dir = NULL;

            cadastrar_disciplina(&(*raiz)->disciplina, disciplina, codigo_curso_atual);
        }

        povoar_disciplinas_crescente(&(*raiz)->esq);
        povoar_disciplinas_crescente(&(*raiz)->dir);
    }
}

void povoamentocrescente_matriculas(Alunos *aluno) {
    if (aluno != NULL) {
        for (int i = 1; i < QUANTIDADEDISCIPLINAS; i++) {
            int codigo_disciplina = i;
            int numero_matricula = aluno->matricula; 

            cadastrar_matricula(&aluno, codigo_disciplina, numero_matricula);
        }

        povoar_matriculas_crescente(aluno->prox);
    }
}

void povoamentocrescente_notas(Alunos *aluno) {
    if (aluno != NULL) {
        for (int i = 1; i < QUANTIDADECURSOS; i++) {
            int codigo_disciplina = i;
            int semestre = 3;  
            float nota_final = (rand() % 10) + 1;  
            int matricula = aluno->matricula;  

            cadastrar_nota(&aluno, matricula, codigo_disciplina, semestre, nota_final);
        }

        povoar_notas_crescente(aluno->prox);
    }
}

void povoamentocrescente_alunos(Alunos **aluno, Arv_Cursos *curso) {
    char *nomes[5] = {"Eleven", "Mike", "Dustin", "Lucas", "Max"};
    
    for (int i = 1; i < QUANTIDADEALUNOS; i++) {
        int matricula = i;
        char *nome = nomes[rand() % 5];  
        int codigo_curso = rand() % QUANTIDADECURSOS;  

        cadastrar_aluno(aluno, matricula, nome, codigo_curso);
    }
}

//forma descrecente
void povoamentodecrescente_disciplinas(Arv_Cursos **raiz) {
    if (*raiz != NULL) {
        int codigo_curso_atual = (*raiz)->codigo_curso;

        for (int i = QUANTIDADEDISCIPLINAS; i > 1; i--) {
            Arv_Disciplina *disciplina = (Arv_Disciplina *)malloc(sizeof(Arv_Disciplina));
            strcpy(disciplina->nome_disciplina, "Anatomia");
            disciplina->carga_horaria = 90;
            disciplina->periodo = 1;
            disciplina->codigo_disciplina = i;
            disciplina->esq = NULL;
            disciplina->dir = NULL;

            cadastrar_disciplina(&(*raiz)->disciplina, disciplina, codigo_curso_atual);
        }

        povoamentodecrescente_disciplinas(&(*raiz)->esq);
        povoamentodecrescente_disciplinas(&(*raiz)->dir);
    }
}

void povoamentodecrescente_matriculas(Alunos *aluno) {
    if (aluno != NULL) {
        for (int i = QUANTIDADEDISCIPLINAS; i > 1; i--) {
            int codigo_disciplina = i;
            int numero_matricula = aluno->matricula;

            cadastrar_matricula(&aluno, codigo_disciplina, numero_matricula);
        }

        povoamentodecrescente_matriculas(aluno->prox);
    }
}

void povoamentodecrescente_notas(Alunos *aluno) {
    if (aluno != NULL) {
        for (int i = QUANTIDADECURSOS; i > 1; i--) {
            int codigo_disciplina = i;
            int semestre = 3;
            float nota_final = (rand() % 10) + 1;
            int matricula = aluno->matricula;

            cadastrar_nota(&aluno, matricula, codigo_disciplina, semestre, nota_final);
        }

        povoamentodecrescente_notas(aluno->prox);
    }
}

void povoamentodecrescente_alunos(Alunos **aluno, Arv_Cursos *curso) {
    char *nomes[5] = {"Eleven", "Mike", "Dustin", "Lucas", "Max"};

    for (int i = QUANTIDADEALUNOS; i > 1; i--) {
        int matricula = i;
        char *nome = nomes[rand() % 5];
        int codigo_curso = rand() % QUANTIDADECURSOS;

        cadastrar_aluno(aluno, matricula, nome, codigo_curso);
    }
}


//forma aleatória
void povoar_disciplinas_aleatorio(Arv_Cursos **raiz) {
    if (*raiz != NULL) {
        int i = 1, sucesso = 0;
        while (i < QUANTIDADEDISCIPLINAS) {
            Arv_Disciplina *disciplina = (Arv_Disciplina *)malloc(sizeof(Arv_Disciplina));
            strcpy(disciplina->nome_disciplina, "Anatomia");
            disciplina->carga_horaria = 90;
            disciplina->periodo = 1;
            disciplina->codigo_disciplina = rand() % QUANTIDADEDISCIPLINAS;
            disciplina->esq = NULL;
            disciplina->dir = NULL;

            if (cadastrar_disciplina(&(*raiz)->disciplina, disciplina, (*raiz)->codigo_curso) == 1)
                i++;
        }

        povoar_disciplinas_aleatorio(&(*raiz)->esq);
        povoar_disciplinas_aleatorio(&(*raiz)->dir);
    }
}

void povoar_matriculas_aleatorio(Alunos *aluno) {
    if (aluno != NULL) {
        int i = 1, sucesso = 0;
        while (i < QUANTIDADEDISCIPLINAS) {
            int codigo_disciplina = rand() % QUANTIDADEDISCIPLINAS;
            int numero_matricula = aluno->matricula;

            cadastrar_matricula(&aluno, codigo_disciplina, numero_matricula);
            i++; 
        }

        povoar_matriculas_aleatorio(aluno->prox);
    }
}


void povoar_notas_aleatorio(Alunos *aluno) {
    if (aluno != NULL) {
        int i = 1, sucesso = 0;
        while (i < QUANTIDADECURSOS) {
            int codigo_disciplina = rand() % QUANTIDADEDISCIPLINAS;
            int semestre = 3;
            float nota_final = (rand() % 10) + 1;
            int matricula = aluno->matricula;

            if (cadastrar_nota(&aluno, matricula, codigo_disciplina, semestre, nota_final) == 1)
                i++;
        }

        povoar_notas_aleatorio(aluno->prox);
    }
}

void povoar_alunos_aleatorio(Alunos **aluno, Arv_Cursos *curso) {
    char *nomes[5] = {"Eleven", "Mike", "Dustin", "Lucas", "Max"};
    int i = 0;

    while (i < QUANTIDADEALUNOS) {
        int matricula = rand() % QUANTIDADEALUNOS; 
        char *nome = nomes[rand() % 5]; 
        int codigo_curso = rand() % QUANTIDADECURSOS; 

        cadastrar_aluno(aluno, matricula, nome, codigo_curso);
        i++; 
    }
}

//3. métricas de tempo

//por inserção na árvore de cursos
double tempomedio_insercao_cursos(Arv_Cursos **curso) {
    clock_t inicio, fim;
    double tempo_total = 0.0, tempomedio_insercao = 0.0;

    int i;

    for (i = 0; i < LOOP; i++) {
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

//por busca na árvore de notas
double tempomedio_busca(Alunos *alunos, Arv_Cursos *raiz, int matricula, int codigo_disciplina) {
    clock_t inicio, fim;
    double tempo_total = 0.0, tempomedio_busca = 0.0;  

    for (int i = 0; i < LOOP; i++) {
        inicio = clock();
    
        notadiscporaluno(alunos, raiz, matricula, codigo_disciplina);
        
        fim = clock();
        
        tempo_total += ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    }

    tempomedio_busca = tempo_total / LOOP;

    return tempomedio_busca;
}

// funções free
void free_arvore_cursos(Arv_Cursos *raiz) {
    if (raiz != NULL) {
        free_arvore_cursos(raiz->esq);
        free_arvore_cursos(raiz->dir);
        free(raiz); 
    }
}

void free_lista_alunos(Alunos *alunos) {
    Alunos *temp;
    while (alunos != NULL) {
        temp = alunos;
        alunos = alunos->prox; 
        free(temp); 
    }
}

//4. int main
int main() {
    Arv_Cursos *raiz = NULL; 
    Alunos *alunos = NULL; 
    int opcao;

    srand(time(NULL));

    do {
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
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
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
            case 10: {
                int matricula, codigo_disciplina;
                printf("Informe a matrícula do aluno: ");
                scanf("%d", &matricula);
                printf("Informe o código da disciplina: ");
                scanf("%d", &codigo_disciplina);
                double tempo = tempomedio_busca(alunos, raiz, matricula, codigo_disciplina);
                printf("Tempo médio de busca: %.6f segundos\n", tempo);
                break;
            }
            case 11: {
                double tempo = tempomedio_insercao_cursos(&raiz);
                printf("Tempo médio de inserção de cursos: %.6f segundos\n", tempo);
                break;
            }
            case 12: {
                int matricula, codigo_disciplina;
                printf("Informe a matrícula do aluno: ");
                scanf("%d", &matricula);
                printf("Informe o código da disciplina: ");
                scanf("%d", &codigo_disciplina);
                double tempo = tempomedio_busca(alunos, raiz, matricula, codigo_disciplina);
                printf("Tempo médio de busca de notas: %.6f segundos\n", tempo);
                break;
            }
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


