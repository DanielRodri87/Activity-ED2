#ifndef SRC_H
#define SRC_H

// -------------- Matriculas ----------------

typedef struct matriculas_info {
    int codigo_disciplina;
} Matriculas_Info;

typedef struct arv_matricula {
    Matriculas_Info *info;
    int altura;
    struct arv_matricula *esq, *dir;
} Arv_Matricula;

// -------------- Disciplinas ----------------
typedef struct disciplinas_info {
    int codigo_disciplina;
    char nome_disciplina[100];
    int periodo;
    int carga_horaria;
    Arv_Matricula *mat; 

} Disciplinas_Info;

typedef struct arv_disciplina {
    Disciplinas_Info *info;
    int altura;
    struct arv_disciplina *esq, *dir;
} Arv_Disciplina;

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
    Arv_Disciplina *disciplina;
    Alunos *alunos;
} Cursos_Info;

typedef struct Arv_Cursos
{
    Cursos_Info *info;
    int altura;
    struct Arv_Cursos *esq, *dir;

} Arv_Cursos;


/*

FUNÇÕES QUE PRECISAM DE BALANCEAMENTO:
 - cadastrar_curso         --> Daniel
 - cadastrar_matricula     --> Cristina feito em 13/10    
 - remover_matricula       --> Daniel
 - cadastrar_nota          --> Daniel
 - cadastrar_disciplina    --> Cristina feito em 13/10 
 - remover_disciplina      --> Cristina feito metade dela em 13/10

*/


// ---------- Funções relacionadas a Cursos ------------
void gerar_codigo_disc(int *codigo_disciplina);
Arv_Cursos* buscar_curso(Arv_Cursos *curso, int codigo_curso);
int cadastrar_curso(Arv_Cursos **curso, Cursos_Info *info);
void exibir_curso(Arv_Cursos *r);


// ---------- Funções relacionadas a Alunos ------------
int cadastrar_aluno(Alunos **aluno, int mat, char *nome, int codigo_curso);
void alunos_por_curso(Alunos *a, int codigo_curso);
Alunos* buscar_aluno_por_matricula_no_curso(Arv_Cursos *curso, int codigo_curso, int matricula);
void exibir_alunos(Alunos *lista);
void consultar_historico(Alunos *aluno, Arv_Cursos *curso, int matricula);

// ---------- Funções relacionadas a Matrículas ------------
void cadastrar_matricula(Alunos **a, int codigo, int mat);
void remover_matricula(Arv_Matricula **m, int cod);
void exibir_matriculas(Arv_Matricula *r);
int verificar_matriculas_alunos(Alunos *aluno, int codigo_disciplina);

// ---------- Funções relacionadas a Notas ------------
int cadastrar_notas(Alunos **a, int mat, Notas_Info *n);
void exibir_nota_aluno_disciplina(Alunos *a, Arv_Cursos *curso, int matricula, int codigo_disciplina);
void notas_disciplina_periodo_aluno(Alunos *aluno, int periodo, int matricula);
int notas_disciplina_periodo(Arv_Notas *n, int periodo);
void mostrar_notas_aluno(Alunos *aluno, Arv_Disciplina *disciplinas, int periodo);

// ---------- Funções Referentes a Disciplina ------------
void exibir_disciplina_periodo(Arv_Cursos *r, int periodo);
int cadastrar_disciplina(Arv_Cursos **curso, Disciplinas_Info *no, int codigo_curso);
void exibir_disciplinas_por_curso(Arv_Cursos *curso, int codigo_curso);
int remover_disciplina_curso(Arv_Cursos **cursos, Alunos *alunos, int codigo_curso, int codigo_disciplina);
void exibir_disciplinas_por_aluno(Alunos *aluno, Arv_Cursos *cursos, int matricula);


// ---------- Escopos para funcionamento do src.c ------------
void remover_no_folha(Arv_Matricula **raiz, Arv_Matricula *pai, Arv_Matricula *atual);
void remover_no_com_um_filho(Arv_Matricula **raiz, Arv_Matricula *pai, Arv_Matricula *atual);
void remover_no_com_dois_filhos(Arv_Matricula *atual);
Alunos *buscar_aluno(Alunos *aluno, int matricula);
void buscar_matricula(Arv_Matricula *r, int codigo, int *encontrado);
void buscar_disciplina(Arv_Matricula *matricula, int cod, int *enc);
void cadastrar_disciplina_no_curso(Arv_Cursos *curso, int codigo_curso, int codigo_disciplina, const char *nome_disciplina, int periodo, int carga_horaria);
#endif

