#ifndef SRC_H
#define SRC_H
/*

lista alunos: mat, nome, codigo_curso, arv notas, arv matriculas, 

Arvore curso: código_curso, nome_curso, quantidade_periodos, arv_disciplinas

Arvore disciplinas: codigo_disciplina, nome_disciplina, periodo, ch

Arvore de notas: codigo_disciplina, semestre, nota_final

Arvore de Matricula: codigo_disciplina


AFAZERES:
Cristina: II, III, IV, VI, VIII, X, XII, XIV
Daniel: I, V, VII, IX, XI, XIII, XV

FEITOS:
CRISTINA: I
DANIEL: II, III, V, VII, IX e XI

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
    struct arv_notas *dir;
} Arv_Notas;

typedef struct arv_disciplina
{
    int codigo_disciplina;
    char nome_disciplina[100];
    int periodo;

    struct arv_disciplina *esq;
    struct arv_disciplina *dir;

} Arv_Disciplina;

typedef struct arv_cursos
{
    int codigo_curso;
    char nome_curso[100]; 
    int quantidade_periodos;

    struct arv_cursos *esq;  
    struct arv_cursos *dir; 
    Arv_Disciplina *disciplina;
} Arv_Cursos;

typedef struct alunos
{
    int matricula;
    char nome[100];
    int codigo_curso;
    Arv_Notas *notas;
    Arv_Matricula *mat;
    struct alunos *prox;
} Alunos;


void cadastrar_aluno(Alunos **lista_alunos, Arv_Cursos *raiz, int matricula, char nome[], int codigo_curso);
void cadastrar_curso(Arv_Cursos **curso, int codigo_curso, const char *nome_curso, int quantidade_periodos);
int cadastrar_matricula(Arv_Matricula **r, int matricula);

int cadastrar_nota(Arv_Matricula **matricula, Arv_Notas **notas, int codigo_disciplina, float nota_final, float semestre);
Arv_Matricula* remover_matricula(Arv_Matricula* raiz, int codigo_disciplina);
Arv_Matricula* encontrar_minimo(Arv_Matricula* raiz);
Arv_Notas* inserir_nota(Arv_Notas* raiz, Arv_Notas* nova_nota);

void exibir_curso(Arv_Cursos *r);
void exibir_disciplina_periodo(Arv_Cursos *r, int periodo);
int notas_disciplina_periodo(Arv_Notas *n, int periodo);

void notas_discplina_periodo_aluno(Alunos *r, int periodo, int mat);

//by crysmorys 23/09 pela manhã
void mostrar_alunoscurso(Arv_Matricula *raiz, int codigo_curso);
void mostrar_disciplinascurso(Arv_Disciplina *raiz, int codigo_curso);
void mostrar_disciplinasaluno(Arv_Matricula *raiz, int codigo_aluno);

#endif 