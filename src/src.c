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
        strcpy(novo_aluno->nome, nome); // Copiando a string corretamente
        novo_aluno->codigo_curso = codigo_curso;
        novo_aluno->notas = NULL;
        novo_aluno->mat = NULL;

        novo_aluno->prox = *lista_alunos;  
        *lista_alunos = novo_aluno;
    }
}
