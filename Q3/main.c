#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/src.h"

int main()
{
    Alunos *aluno = NULL;
    Arv_Cursos *arv_curso = NULL;
    Arv_Notas *arv_notas = NULL;
    Arv_Disciplina *arv_disciplina = NULL;
    Arv_Matricula *arv_matricula = NULL;

    cadastrar_curso(&arv_curso, 101, "Sistemas", 8);
    cadastrar_curso(&arv_curso, 102, "Medicina", 12);
    cadastrar_curso(&arv_curso, 103, "ADS", 5);

    exibir_curso(arv_curso);

    return 0;
}