#include "src.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void test_function()
{
    printf("Ate amanha cristina!!\n");
}


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