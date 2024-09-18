#ifndef SRC_H
#define SRC_H
/*

lista alunos: mat, nome, codigo_curso, arv notas, arv matriculas, 

Arvore curso: código_curso, nome_curso, quantidade_periodos, arv_disciplinas

Arvore disciplinas: codigo_disciplina, nome_disciplina, periodo, ch

Arvore de notas: codigo_disciplina, semestre, nota_final

Arvore de Matricula: codigo_disciplina


AFAZERES:
DANIEL: II, IV, VI, VIII, X, XII, XIV
CRISTINA: I, III, V, VII, IX, XI, XIII, XV

FEITOS:
CRISTINA:
DANIEL

*/

typedef struct arv_matricula
{
    int codigo_disciplina;

    struct arv_matricula *esq;
    struct arv_matricula *dir;

    
} Arv_Matricula;

typedef struct arv_notas
{
    int codigo_disciplina;
    float semestre;
    float nota_final;

    struct arv_notas *esq;
    struct arv_notas*dir;
} Arv_Notas;

typedef struct arv_disciplina
{
    int codigo_disciplina;
    char nome_disciplina;
    int periodo;

    struct arv_disciplina *esq;
    struct arv_disciplina *dir;

} Arv_Disciplina;

typedef struct arv_cursos
{
    int codigo_curso;
    char nome_curso;
    int quantidade_periodos;

    struct arv_curso *esq;
    struct arv_curso *dir;
    Arv_Disciplina *disciplina;
} Arv_Cursos;

typedef struct alunos
{
    int matricula;
    char nome;
    int codigo_curso;
    Arv_Notas *notas;
    Arv_Matricula *mat;
    struct alunos *prox;
} Alunos;


void test_function();

#endif 