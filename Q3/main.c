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
    printf("Cursos cadastrados com sucesso.\n");
    
    cadastrar_aluno(&aluno, 2023, "Daniel", 101);
    printf("Aluno Daniel cadastrado com sucesso.\n");

    int codigo_disciplina;
    gerar_codigo_disc(&codigo_disciplina);
    cadastrar_disciplina_no_curso(arv_curso, 101, codigo_disciplina, "Algoritmos", 3, 45);
    gerar_codigo_disc(&codigo_disciplina);
    cadastrar_disciplina_no_curso(arv_curso, 101, codigo_disciplina, "Estruturas de Dados", 5, 60);
    printf("Disciplinas cadastradas no curso de Sistemas.\n");

    
    cadastrar_matricula(&aluno, 1, 2023);



    printf("\n-----------\nExibir curso:\n-----------\n");
    exibir_curso(arv_curso);

    printf("\n-----------\nExibir Disciplinas por curso:\n-----------\n");
    exibir_disciplinasporcurso(arv_curso, 101);

    printf("\n-----------\nExibir Alunos por curso:\n-----------\n");
    alunosporcurso(aluno, 101);

    printf("\n-----------\nExibir Alunos Matriculado:\n-----------\n");
    exibir_disciplinasporaluno(aluno, arv_curso, 2023);

    return 0;
}
