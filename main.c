#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/src.h"

int main() {
    Arv_Cursos *cursos = NULL; 
    Alunos *lista_alunos = NULL;

    cadastrar_curso(&cursos, 101, "Sistemas de Informação", 8);
    cadastrar_curso(&cursos, 102, "Medicina", 12);
    cadastrar_curso(&cursos, 103, "Medicina Veterinária", 10);

    cadastrar_aluno(&lista_alunos, cursos, 2024001, "Daniel Rodrigues", 101);
    cadastrar_aluno(&lista_alunos, cursos, 2024002, "Cristina Rica", 102);
    cadastrar_aluno(&lista_alunos, cursos, 2024003, "Rita Silva", 103);

    cadastrar_matricula(&(lista_alunos->mat), 3001); 
    cadastrar_matricula(&(lista_alunos->prox->mat), 3002); 

    cadastrar_nota(&(lista_alunos->mat), &(lista_alunos->notas), 3001, 8.5, 2024.1);
    cadastrar_nota(&(lista_alunos->prox->mat), &(lista_alunos->prox->notas), 3002, 9.2, 2024.1);

    Alunos *aluno_atual = lista_alunos;
    while (aluno_atual != NULL) {
        printf("Aluno: %s (Matricula: %d, Curso: %d)\n", aluno_atual->nome, aluno_atual->matricula, aluno_atual->codigo_curso);

        if (aluno_atual->mat != NULL) {
            printf("  Matriculado em disciplina: %d\n", aluno_atual->mat->codigo_disciplina);
        }

        if (aluno_atual->notas != NULL) {
            printf("  Nota final na disciplina %d: %.2f (Semestre: %.1f)\n", aluno_atual->notas->codigo_disciplina, aluno_atual->notas->nota_final, aluno_atual->notas->semestre);
        }

        aluno_atual = aluno_atual->prox;
    }

    return 0;
}
