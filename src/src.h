#ifndef SRC_H
#define SRC_H

typedef struct arv_disciplina
{
    int codigo_disciplina;
    char nome_disciplina[100];
    int periodo;
    int carga_horaria;

    struct arv_disciplina *esq;
    struct arv_disciplina *dir;

} Arv_Disciplina;

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

typedef struct alunos
{
    int matricula;
    char nome[100];
    int codigo_curso;
    Arv_Notas *notas;
    Arv_Matricula *mat;
    struct alunos *prox;
} Alunos;

typedef struct arv_cursos
{
    int codigo_curso;
    char nome_curso[100]; 
    int quantidade_periodos;

    struct arv_cursos *esq;  
    struct arv_cursos *dir; 
    Arv_Disciplina *disciplina;
    Alunos *alunos; 
} Arv_Cursos;

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
void exibir_alunosporcurso(Arv_Cursos *curso, int codigo_curso);
void exibir_disciplinasporcurso(Arv_Cursos *curso, int codigo_curso);

void exibir_disciplinasporcurso_recursivamente(Arv_Disciplina *disciplina);
void exibir_disciplinasporaluno(Arv_Cursos *curso, int matricula_aluno);
void exibir_disciplinasporalunos_recursivamente(Arv_Disciplina *disciplina);

void remover_disciplinaaluno(Arv_Matricula **raiz, int codigo_disciplina);
void cadastrar_disciplina(Arv_Cursos *curso, int codigo_curso, int codigo_disciplina, char *nome_disciplina, int periodo, int cargahoraria);
void exibir_notadisciplina(Alunos *alunos, Arv_Cursos *raiz_cursos, int matricula, int codigo_disciplina);

void exibir_notas_periodo(Arv_Notas *notas, Arv_Disciplina *disciplinas, int periodo);
Arv_Disciplina *buscar_disciplina_por_codigo(Arv_Disciplina *disciplinas, int codigo);
void mostrar_notas_aluno(Alunos *aluno, Arv_Disciplina *disciplinas, int periodo);

int remover_disciplina_curso(Arv_Cursos **curso, Alunos *alunos, int id_curso, int codigo_disc);
void confirmar_remocao(Alunos *r, int cod_disc, int *validar_disc);
int remover_disciplina_xiii(Arv_Disciplina **r, int codigo_disciplina);

void exibir_historico(Alunos *aluno, Arv_Cursos *r);
void buscar_curso_aluno(Alunos *aluno, Arv_Cursos *r);
void buscar_aluno_xv(Alunos *aluno, int matricula, Arv_Cursos *r);


#endif 