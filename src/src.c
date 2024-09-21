#include "src.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void cadastrar_aluno(Alunos **lista_alunos, Arv_Cursos *raiz, int matricula, char nome[], int codigo_curso){
    Arv_Cursos *curso_encontrado = NULL;

    while(raiz != NULL && curso_encontrado == NULL){
        if(raiz->codigo_curso == codigo_curso){
            curso_encontrado = raiz;  
        }else if(codigo_curso < raiz->codigo_curso){
            raiz = raiz->esq; 
        }else{
            raiz = raiz->dir; 
        }
    }
    
    if(curso_encontrado != NULL){
        Alunos *novo_aluno = (Alunos*) malloc(sizeof(Alunos));
        novo_aluno->matricula = matricula;
        strcpy(novo_aluno->nome, nome); 
        novo_aluno->codigo_curso = codigo_curso;
        novo_aluno->notas = NULL;
        novo_aluno->mat = NULL;

        novo_aluno->prox = *lista_alunos;  
        *lista_alunos = novo_aluno;
    }
}

void cadastrar_curso(Arv_Cursos **curso, int codigo_curso, const char *nome_curso, int quantidade_periodos)
{
    if(*curso == NULL){
        Arv_Cursos *novo = (Arv_Cursos*)malloc(sizeof(Arv_Cursos));
        novo->codigo_curso = codigo_curso;
        strcpy(novo->nome_curso, nome_curso);
        novo->quantidade_periodos = quantidade_periodos;
        novo->dir = NULL;
        novo->esq = NULL;
        *curso = novo;
    }
    else{
        if(codigo_curso < (*curso)->codigo_curso)
            cadastrar_curso(&((*curso)->esq), codigo_curso, nome_curso, quantidade_periodos);
        else
            cadastrar_curso(&((*curso)->dir), codigo_curso, nome_curso, quantidade_periodos);
    }
}

int cadastrar_matricula(Arv_Matricula **r, int matricula)
{
    int insere = 1;
    if (*r == NULL)
    {
        *r = (Arv_Matricula*) malloc(sizeof(Arv_Matricula));
        if (*r == NULL) 
            insere = 0;
        (*r)->codigo_disciplina = matricula;
        (*r)->esq = NULL;
        (*r)->dir = NULL;
        insere = 1; 
    }
    if (matricula < (*r)->codigo_disciplina)
        insere = cadastrar_matricula(&((*r)->esq), matricula);
    else if (matricula > (*r)->codigo_disciplina)
        insere = cadastrar_matricula(&((*r)->dir), matricula);
    return (insere); 
}


int cadastrar_nota(Arv_Matricula **matricula, Arv_Notas **notas, int codigo_disciplina, float nota_final, float semestre)
{
    int nota_cadastrada = 0;
    
    Arv_Matricula *mat_atual = *matricula;
    Arv_Matricula *mat_pai = NULL;
    
    while (mat_atual != NULL && nota_cadastrada == 0)
    {
        if (codigo_disciplina == mat_atual->codigo_disciplina)
        {
            if (mat_pai == NULL)
                *matricula = remover_matricula(*matricula, codigo_disciplina);
            else if (codigo_disciplina < mat_pai->codigo_disciplina)
                mat_pai->esq = remover_matricula(mat_pai->esq, codigo_disciplina);
            else 
                mat_pai->dir = remover_matricula(mat_pai->dir, codigo_disciplina);

            Arv_Notas *nova_nota = (Arv_Notas *)malloc(sizeof(Arv_Notas));
            nova_nota->codigo_disciplina = codigo_disciplina;
            nova_nota->nota_final = nota_final;
            nova_nota->semestre = semestre;
            nova_nota->esq = NULL;
            nova_nota->dir = NULL;

            *notas = inserir_nota(*notas, nova_nota);
            nota_cadastrada = 1;
        } else if (codigo_disciplina < mat_atual->codigo_disciplina)
        {
            mat_pai = mat_atual;
            mat_atual = mat_atual->esq;
        } else
        {
            mat_pai = mat_atual;
            mat_atual = mat_atual->dir;
        }
    }

    return (nota_cadastrada);
}

Arv_Matricula* remover_matricula(Arv_Matricula* raiz, int codigo_disciplina)
{
    Arv_Matricula* resultado = raiz;
    if (raiz != NULL) {
        if (codigo_disciplina < raiz->codigo_disciplina)
            raiz->esq = remover_matricula(raiz->esq, codigo_disciplina);
        else if (codigo_disciplina > raiz->codigo_disciplina)
            raiz->dir = remover_matricula(raiz->dir, codigo_disciplina);
        else
        {
            if (raiz->esq == NULL)
            {
                resultado = raiz->dir;
                free(raiz);
            } else if (raiz->dir == NULL)
            {
                resultado = raiz->esq;
                free(raiz);
            } else
            {
                Arv_Matricula* temp = encontrar_minimo(raiz->dir);
                raiz->codigo_disciplina = temp->codigo_disciplina;
                raiz->dir = remover_matricula(raiz->dir, temp->codigo_disciplina);
            }
        }
    }
    return (resultado);
}

Arv_Matricula* encontrar_minimo(Arv_Matricula* raiz)
{
    Arv_Matricula* resultado = raiz;
    while (resultado != NULL && resultado->esq != NULL)
        resultado = resultado->esq;

    return (resultado);
}

Arv_Notas* inserir_nota(Arv_Notas* raiz, Arv_Notas* nova_nota)
{
    Arv_Notas* resultado = raiz;
    if (raiz == NULL)
        resultado = nova_nota;
    else
    {
        if (nova_nota->codigo_disciplina < raiz->codigo_disciplina)
            raiz->esq = inserir_nota(raiz->esq, nova_nota);
        else
            raiz->dir = inserir_nota(raiz->dir, nova_nota);
    }
    return (resultado);
}
