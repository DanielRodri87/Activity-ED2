#ifndef SRC_H
#define SRC_H


// -------------- Disciplinas ----------------
typedef struct disciplinas_info {
    int codigo_disciplina;
    char nome_disciplina[100];
    int periodo;
    int carga_horaria;

} Disciplinas_Info;

typedef struct arv_disciplina {
    Disciplinas_Info *info;
    int altura;
    struct arv_disciplina *esq, *dir;
} Arv_Disciplina;

// -------------- Matriculas ----------------

typedef struct matriculas_info {
    int codigo_disciplina;
} Matriculas_Info;

typedef struct arv_matricula {
    Matriculas_Info *info;
    int altura;
    struct arv_matricula *esq, *dir;
} Arv_Matricula;

// -------------- Notas ----------------
typedef struct notas_info {
    int codigo_disciplina;
    float semestre;
    float nota_final;

} Notas_Info;

typedef struct arv_notas {
    Notas_Info *info;
    int altura;
    struct arv_notas *esq, *dir;
} Arv_Notas;

// -------------- Alunos ----------------
typedef struct alunos {
    int matricula;
    char nome[100];
    int codigo_curso;
    Arv_Notas *notas;
    Arv_Matricula *mat;
    struct alunos *prox;
} Alunos;

// -------------- Cursos ----------------

typedef struct cursos_info
{
    int codigo_curso;
    char nome_curso[100];
    int quantidade_periodos;
    Disciplinas_Info *disciplina;
    Alunos *alunos;
} Cursos_Info;

typedef struct Arv_Curso
{
    Cursos_Info *info;
    int altura;
    struct Arv_Curso *esq, *dir;

} Arv_Curso;



#endif

