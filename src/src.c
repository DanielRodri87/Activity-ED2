#include "src.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

int cadastrar_matricula(Arv_Matricula **r, int matricula)
{
    int insere = 1;
    if (*r == NULL)
    {
        *r = (Arv_Matricula*) malloc(sizeof(Arv_Matricula));
        if (*r == NULL) 
            insere = 0;
        (*r)->codigo_disciplina = matricula;
        (*r)->esq = NULL;
        (*r)->dir = NULL;
        insere = 1; 
    }
    if (matricula < (*r)->codigo_disciplina)
        insere = cadastrar_matricula(&((*r)->esq), matricula);
    else if (matricula > (*r)->codigo_disciplina)
        insere = cadastrar_matricula(&((*r)->dir), matricula);
    return (insere); 
}


int cadastrar_nota(Arv_Matricula **matricula, Arv_Notas **notas, int codigo_disciplina, float nota_final, float semestre)
{
    int nota_cadastrada = 0;
    
    Arv_Matricula *mat_atual = *matricula;
    Arv_Matricula *mat_pai = NULL;
    
    while (mat_atual != NULL && nota_cadastrada == 0)
    {
        if (codigo_disciplina == mat_atual->codigo_disciplina)
        {
            if (mat_pai == NULL)
                *matricula = remover_matricula(*matricula, codigo_disciplina);
            else if (codigo_disciplina < mat_pai->codigo_disciplina)
                mat_pai->esq = remover_matricula(mat_pai->esq, codigo_disciplina);
            else 
                mat_pai->dir = remover_matricula(mat_pai->dir, codigo_disciplina);

            Arv_Notas *nova_nota = (Arv_Notas *)malloc(sizeof(Arv_Notas));
            nova_nota->codigo_disciplina = codigo_disciplina;
            nova_nota->nota_final = nota_final;
            nova_nota->semestre = semestre;
            nova_nota->esq = NULL;
            nova_nota->dir = NULL;

            *notas = inserir_nota(*notas, nova_nota);
            nota_cadastrada = 1;
        } else if (codigo_disciplina < mat_atual->codigo_disciplina)
        {
            mat_pai = mat_atual;
            mat_atual = mat_atual->esq;
        } else
        {
            mat_pai = mat_atual;
            mat_atual = mat_atual->dir;
        }
    }

    return (nota_cadastrada);
}

Arv_Matricula* remover_matricula(Arv_Matricula* raiz, int codigo_disciplina)
{
    Arv_Matricula* resultado = raiz;
    if (raiz != NULL)
    {
        if (codigo_disciplina < raiz->codigo_disciplina)
            raiz->esq = remover_matricula(raiz->esq, codigo_disciplina);
        else if (codigo_disciplina > raiz->codigo_disciplina)
            raiz->dir = remover_matricula(raiz->dir, codigo_disciplina);
        else
        {
            if (raiz->esq == NULL)
            {
                resultado = raiz->dir;
                free(raiz);
            } else if (raiz->dir == NULL)
            {
                resultado = raiz->esq;
                free(raiz);
            } else
            {
                Arv_Matricula* temp = encontrar_minimo(raiz->dir);
                raiz->codigo_disciplina = temp->codigo_disciplina;
                raiz->dir = remover_matricula(raiz->dir, temp->codigo_disciplina);
            }
        }
    }
    return (resultado);
}

Arv_Matricula* encontrar_minimo(Arv_Matricula* raiz)
{
    Arv_Matricula* resultado = raiz;
    while (resultado != NULL && resultado->esq != NULL)
        resultado = resultado->esq;

    return (resultado);
}

Arv_Notas* inserir_nota(Arv_Notas* raiz, Arv_Notas* nova_nota)
{
    Arv_Notas* resultado = raiz;
    if (raiz == NULL)
        resultado = nova_nota;
    else
    {
        if (nova_nota->codigo_disciplina < raiz->codigo_disciplina)
            raiz->esq = inserir_nota(raiz->esq, nova_nota);
        else
            raiz->dir = inserir_nota(raiz->dir, nova_nota);
    }
    return (resultado);
}

void exibir_curso(Arv_Cursos *r)
{
    if (r != NULL)
    {
        printf("Nome do curso: %s | ID: %d\n", r->nome_curso, r->codigo_curso);
        exibir_curso(r->esq);
        exibir_curso(r->dir);
    }

}

void exibir_disciplina_periodo(Arv_Cursos *r, int periodo)
{
    if (r != NULL)
    {
        if (r->disciplina->periodo == periodo)
        {
            printf("Curso: %s | Disciplina: %s | Período: %d\n", r->nome_curso, r->disciplina->nome_disciplina, r->disciplina->periodo);
            exibir_disciplina_periodo(r->esq, periodo);
            exibir_disciplina_periodo(r->dir, periodo);
        }
    }
}

int notas_disciplina_periodo(Arv_Notas *n, int periodo)
{
    if (n == NULL) 
        printf("Nota nao encontrada\n");
    else
    {
        if (n->semestre == periodo) 
            printf("Disciplina: %d, Nota Final: %.2f\n", n->codigo_disciplina, n->nota_final);

        notas_disciplina_periodo(n->esq, periodo);
        notas_disciplina_periodo(n->dir, periodo);
    }
}

void notas_discplina_periodo_aluno(Alunos *r, int periodo, int mat)
{
    if (r == NULL)
        printf("Nao existe alunos cadastrados\n");
    else
    {
        if (r->matricula == mat)
        {
            if (r->notas != NULL) 
                notas_disciplina_periodo(r->notas, periodo);
            else
                printf("Este aluno nao possui notas cadastradas\n");

        } else 
            printf("Aluno nao encontrado\n");
    }
}

// função para exibição de todos os alunos de um determinado curso
void exibir_alunosporcurso(Arv_Cursos *curso, int codigo_curso){
    if(curso != NULL){
        if(curso->codigo_curso == codigo_curso){
            Alunos *aluno_atual = curso->alunos;
            while(aluno_atual != NULL){
                printf("Nome: %s\nMatricula: %d\n", aluno_atual->nome, aluno_atual->matricula);
                aluno_atual = aluno_atual->prox;
            }
        }

        exibir_alunosporcurso(curso->esq, codigo_curso);
        exibir_alunosporcurso(curso->dir, codigo_curso);
    }
}

// função para exibição de todas as disciplinas de um determinado curso (as duas funções se complementam)
void exibir_disciplinasporcurso(Arv_Cursos *curso, int codigo_curso){
    if (curso != NULL){
        if(curso->codigo_curso == codigo_curso){
            Arv_Disciplina *disciplina_atual = curso->disciplina;

            exibir_disciplinasporcurso_recursivamente(disciplina_atual);
        }

        exibir_disciplinasporcurso(curso->esq, codigo_curso);
        exibir_disciplinasporcurso(curso->dir, codigo_curso);
    }

}

void exibir_disciplinasporcurso_recursivamente(Arv_Disciplina *disciplina){
    if(disciplina != NULL){
        printf("Codigo: %d - Periodo: %d - Nome: %s", disciplina->codigo_disciplina, disciplina->periodo, disciplina->nome_disciplina);

        exibir_disciplinasporcurso_recursivamente(disciplina->esq);
        exibir_disciplinasporcurso_recursivamente(disciplina->dir);
    }
}

// função para exibição de todas as disciplinas que um determinado aluno está matriculado (as duas funções se complementam)

void exibir_disciplinasporaluno(Arv_Cursos *curso, int matricula_aluno){
    if(curso != NULL){
        if(curso->codigo_curso != -1){
            Alunos *aluno_atual = curso->alunos;
            while(aluno_atual != NULL){
                if(aluno_atual->matricula == matricula_aluno){
                    Arv_Disciplina *disciplina_atual = curso->disciplina;
                    while(disciplina_atual != NULL){
                        printf("Codigo: %d - Periodo: %d - Nome: %s", disciplina_atual->codigo_disciplina, disciplina_atual->nome_disciplina, disciplina_atual->periodo);

                        exibir_disciplinasporalunos_recursivamente(disciplina_atual);
                        break;
                    }
                }
                aluno_atual = aluno_atual->prox;
            }
        }

        exibir_disciplinasporaluno(curso->esq, matricula_aluno);
        exibir_disciplinasporaluno(curso->dir, matricula_aluno);
    }
}

void exibir_disciplinasporalunos_recursivamente(Arv_Disciplina *disciplina){
    if(disciplina != NULL){
        printf("Codigo: %d - Periodo: %d - Nome: %s", disciplina->codigo_disciplina, disciplina->nome_disciplina, disciplina->periodo);

        exibir_disciplinasporalunos_recursivamente(disciplina->esq);
        exibir_disciplinasporalunos_recursivamente(disciplina->dir);
    }
}

// função para permitir a remoção de uma disciplina da árvore de matrícula de um determinado aluno.
void remover_disciplinaaluno(Arv_Matricula **raiz, int codigo_disciplina){
    if(*raiz != NULL){
        Arv_Matricula *atual = *raiz;
        Arv_Matricula *pai = NULL;
        int encontrado = 0;

        while (atual != NULL){
            if(atual->codigo_disciplina == codigo_disciplina){
                encontrado = 1;
                break;
            }

            pai = atual;
            if(codigo_disciplina < atual->codigo_disciplina){
                atual = atual->esq;
            }else{
                atual = atual->dir;
            }
        }

        if(encontrado){
            if(atual->esq == NULL && atual->dir == NULL){
                if(pai == NULL){
                    *raiz = NULL;
                }else if(pai->esq == atual){
                    pai->esq = NULL;
                }else{
                    pai->dir = NULL;
                }
            }

            else if(atual->esq == NULL){
                if(pai == NULL){
                    *raiz = atual->dir;
                }else if(pai->esq == atual){
                    pai->esq = atual->dir;
                }else{
                    pai->dir = atual->dir;
                }
            }else if(atual->dir == NULL){
                if(pai == NULL){
                    *raiz = atual->esq;
                }else if(pai->esq == atual){
                    pai->esq = atual->esq;
                }else{
                    pai->dir = atual->esq;
                }
            }

            else{

                Arv_Matricula *menor_no = atual->dir;
                Arv_Matricula *menor_nopai = atual;

                while(menor_no->esq != NULL){
                    menor_nopai = menor_no;
                    menor_no = menor_no->esq;
                }

                atual->codigo_disciplina = menor_no->codigo_disciplina;

                if(menor_nopai->esq == menor_no){
                    menor_nopai->esq = menor_no->dir;
                }else{
                    menor_nopai->dir = menor_no->dir;
                }
            }

            free(atual);
        }

    }

}

void cadastrar_disciplina(Arv_Cursos *curso, int codigo_curso, int codigo_disciplina, char *nome_disciplina, int periodo, int cargahoraria){

    int curso_encontrado = 0;
    int periodo_valido = 0;
    int cargahoraria_valida = 0;

    while(curso != NULL && curso->codigo_curso != codigo_curso){
        if(codigo_curso < curso->codigo_curso){
            curso = curso->esq;
        }else{
            curso = curso->dir;
        }
    }

    if(curso != NULL){
        curso_encontrado = 1;
    }

    if(curso_encontrado != 0 && periodo >= 1 && periodo <= curso->quantidade_periodos){
        periodo_valido = 1;
    }

    if(curso_encontrado != 0 && periodo_valido != 0 && cargahoraria >= 30 && cargahoraria <= 90 && cargahoraria % 15 == 0){
        cargahoraria_valida = 1;
    }

    if(curso_encontrado != 0 && periodo_valido != 0 && cargahoraria_valida != 0){

        Arv_Disciplina *nova_disciplina = (Arv_Disciplina*)malloc(sizeof(Arv_Disciplina));
        nova_disciplina->codigo_disciplina = codigo_disciplina;
        strcpy(nova_disciplina->nome_disciplina, nome_disciplina);
        nova_disciplina->esq = NULL;
        nova_disciplina->dir = NULL;

        if(curso->disciplina == NULL){
            curso->disciplina = nova_disciplina;
        }else{
            Arv_Disciplina *atual = curso->disciplina;
            Arv_Disciplina *anterior = NULL;

            while(atual != NULL){
                anterior = atual;
                if(codigo_disciplina < atual->codigo_disciplina){
                    atual = atual->esq;
                }else{
                    atual = atual->dir;
                }
            }

            if(codigo_disciplina < anterior->codigo_disciplina){
                anterior->esq = nova_disciplina;
            }else{
                anterior->dir = nova_disciplina;
            }
        }
    } 
}

//exibição da nota de uma disciplina de um determinado aluno, mostrando o período e a carga horária da disciplina. 
Arv_Cursos* buscar_curso(Arv_Cursos *raiz, int codigo_curso)
{
    Arv_Cursos *retorno;
    if (raiz != NULL)
    {
        if (raiz->codigo_curso == codigo_curso)
            retorno = raiz;
        else if (codigo_curso < raiz->codigo_curso)
            retorno = buscar_curso(raiz->esq, codigo_curso);
        else 
            retorno = buscar_curso(raiz->dir, codigo_curso);

        return (retorno);
    }
}

Alunos* buscar_aluno(Alunos *alunos, int matricula)
{
    while (alunos != NULL && alunos->matricula != matricula)
        alunos = alunos->prox;
    return alunos;
}

Arv_Disciplina* buscar_disciplina(Arv_Disciplina *disciplina, int codigo_disciplina)
{
    while (disciplina != NULL && disciplina->codigo_disciplina != codigo_disciplina)
    {
        if (codigo_disciplina > disciplina->codigo_disciplina)
            disciplina = disciplina->dir;
        else
            disciplina = disciplina->esq;
    }
    return disciplina;
}

Arv_Notas* buscar_nota(Arv_Notas *notas, int codigo_disciplina)
{
    while (notas != NULL && notas->codigo_disciplina != codigo_disciplina)
    {
        if (codigo_disciplina > notas->codigo_disciplina)
            notas = notas->dir;
        else
            notas = notas->esq;
    }
    return notas;
}

void exibir_informacoes(Arv_Disciplina *disciplina_atual, Arv_Notas *nota_atual)
{
    printf("Disciplina: %s\n", disciplina_atual->nome_disciplina);
    printf("Periodo: %d\n", disciplina_atual->periodo);
    printf("Carga Horaria: %dh\n", disciplina_atual->carga_horaria);
    printf("Nota final: %.2f\n", nota_atual->nota_final);
}

void exibir_notadisciplina(Alunos *alunos, Arv_Cursos *raiz_cursos, int matricula, int codigo_disciplina)
{
    Alunos *aluno_atual = buscar_aluno(alunos, matricula);
    if (!aluno_atual)
        printf("Aluno nao encontrado.\n");

    Arv_Cursos *curso_atual = buscar_curso(raiz_cursos, aluno_atual->codigo_curso);
    if (!curso_atual)
        printf("Curso nao encontrado.\n");

    Arv_Disciplina *disciplina_atual = buscar_disciplina(curso_atual->disciplina, codigo_disciplina);
    Arv_Notas *nota_atual = buscar_nota(aluno_atual->notas, codigo_disciplina);

    if (!disciplina_atual)
        printf("Disciplina nao encontrada.\n");
    else if (!nota_atual)
        printf("Nota não encontrada.\n");
    else
        exibir_informacoes(disciplina_atual, nota_atual);
}

// Mostrar todas as notas de disciplinas de um determinado período de um determinado aluno. 
void exibir_notas_periodo(Arv_Notas *notas, Arv_Disciplina *disciplinas, int periodo)
{
    if (notas != NULL)
    {   
        exibir_notas_periodo(notas->esq, disciplinas, periodo);

        Arv_Disciplina *disciplina = buscar_disciplina_por_codigo(disciplinas, notas->codigo_disciplina);
        if (disciplina != NULL && disciplina->periodo == periodo)
            printf("Disciplina: %s | Semestre: %.2f | Nota final: %.2f\n", 
                disciplina->nome_disciplina, notas->semestre, notas->nota_final);

        exibir_notas_periodo(notas->dir, disciplinas, periodo);
    }
}

Arv_Disciplina *buscar_disciplina_por_codigo(Arv_Disciplina *disciplinas, int codigo)
{
    Arv_Disciplina *aux;
    if (disciplinas != NULL)
    {
        if (codigo == disciplinas->codigo_disciplina)
            aux = disciplinas;
        else if (codigo < disciplinas->codigo_disciplina)
            aux = buscar_disciplina_por_codigo(disciplinas->esq, codigo);
        else 
            aux = buscar_disciplina_por_codigo(disciplinas->dir, codigo);
    }

    return (aux);

}

void mostrar_notas_aluno(Alunos *aluno, Arv_Disciplina *disciplinas, int periodo)
{
    if (aluno != NULL)
    {
        printf("Notas do aluno %s no periodo %d:\n", aluno->nome, periodo);
        exibir_notas_periodo(aluno->notas, disciplinas, periodo);
    }
}
