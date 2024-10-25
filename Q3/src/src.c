#include "src.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

int contador_disciplina = 1;

void gerar_codigo_disc(int *codigo_disciplina)
{
    *codigo_disciplina = contador_disciplina;
    contador_disciplina++;
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// I - Cadastrar alunos a qualquer momento na lista, de forma que só possa cadastrar um código de curso que
// já tenha sido cadastrado na árvore de cursos.
// ---------------------------------------------------- XXXXXX -------------------------------------------------
void converternome(char *nome)
{
    int i = 0;
    while (nome[i] != '\0')
    {
        nome[i] = toupper(nome[i]);
        i++;
    }
}

int cadastrar_aluno(Alunos **aluno, int matricula, char *nome, int codigo_curso)
{
    int resultado = 1;  // Variável de controle (1 = sucesso, 0 = aluno já existe, -1 = erro de alocação)
    int existe = 0;     // Variável auxiliar para controlar se o aluno já existe

    Alunos *aux = *aluno;
    while (aux != NULL)
    {
        if (aux->matricula == matricula)
        {
            existe = 1;  // Aluno com a mesma matrícula encontrado
            resultado = 0;  // Aluno já existe
        }
        aux = aux->prox;  // Continua a verificação para o próximo aluno
    }

    if (!existe)  // Apenas tenta inserir se a matrícula for única
    {
        // Alocar memória para o novo aluno
        Alunos *novo = (Alunos *)malloc(sizeof(Alunos));
        if (novo == NULL)
        {
            printf("Erro ao alocar memória para o novo aluno.\n");
            resultado = -1;  // Erro de alocação
        }
        else
        {
            novo->prox = NULL;
            novo->matricula = matricula;
            char *aux_nome = strdup(nome);
            converternome(aux_nome);  // Função para converter nome
            strcpy(novo->nome, aux_nome);
            novo->codigo_curso = codigo_curso;
            novo->notas = NULL;
            novo->mat = NULL;

            if (*aluno == NULL) 
                *aluno = novo;
            else
            {
                if (strcmp(aux_nome, (*aluno)->nome) < 0)  // Inserir no início
                {
                    novo->prox = *aluno;
                    *aluno = novo;
                }
                else  
                {
                    Alunos *aux = *aluno;
                    while (aux->prox != NULL && strcmp(aux_nome, aux->prox->nome) > 0)
                        aux = aux->prox;

                    novo->prox = aux->prox;
                    aux->prox = novo;
                }
            }

            free(aux_nome);  // Liberar memória duplicada para o nome
        }
    }

    return (resultado);  // Retornar o valor final da variável de controle
}


// ---------------------------------------------------- XXXXXX -------------------------------------------------
// II - Cadastrar cursos a qualquer momento na árvore de curso, de forma que o usuário não precise cadastrar
// as disciplinas para permitir o cadastro do curso.
// ---------------------------------------------------- XXXXXX -------------------------------------------------

int altura_curso(Arv_Cursos *curso)
{
    int resultado = -1, altura_esquerda, altura_direita;

    if (curso)
    {
        altura_esquerda = altura_curso(curso->esq);
        altura_direita = altura_curso(curso->dir);
        resultado = 1 + (altura_esquerda > altura_direita ? altura_esquerda : altura_direita);
    }

    return (resultado);
}

int fator_balanceamento_curso(Arv_Cursos *curso)
{
    int fb = 0;
    if (curso != NULL)
        fb = altura_curso(curso->esq) - altura_curso(curso->dir);
    return (fb);
}

void rotacao_esq_curso(Arv_Cursos **curso)
{
    Arv_Cursos *aux;
    if (*curso != NULL)
    {
        aux = (*curso)->dir;
        (*curso)->dir = aux->esq;
        aux->esq = (*curso);

        (*curso)->altura = altura_curso(*curso);
        aux->altura = altura_curso(aux);

        *curso = aux;
    }
}

void rotacao_dir_curso(Arv_Cursos **curso)
{
    Arv_Cursos *aux;
    if (*curso != NULL)
    {
        aux = (*curso)->esq;
        (*curso)->esq = aux->dir;
        aux->dir = (*curso);

        (*curso)->altura = altura_curso(*curso);
        aux->altura = altura_curso(aux);

        *curso = aux;
    }
}

void balanceamento_curso(Arv_Cursos **curso)
{
    int fb;
    Arv_Cursos *aux;
    fb = fator_balanceamento_curso(*curso);

    if (fb >= 2)
    {
        aux = (*curso)->esq;
        fb = fator_balanceamento_curso(aux);

        if (fator_balanceamento_curso((*curso)->esq) < 0)
            rotacao_esq_curso(&(*curso)->esq);

        rotacao_dir_curso(curso);
    }
    else if (fb <= -2)
    {
        aux = (*curso)->dir;
        fb = fator_balanceamento_curso(aux);

        if (fator_balanceamento_curso((*curso)->dir) > 0)
            rotacao_dir_curso(&(*curso)->dir);

        rotacao_esq_curso(curso);
    }
}
int cadastrar_curso(Arv_Cursos **curso, Cursos_Info *info)
{
    int sucesso = 0;
    if (*curso == NULL)
    {
        *curso = (Arv_Cursos *)malloc(sizeof(Arv_Cursos));
        (*curso)->info = info;
        (*curso)->esq = NULL;
        (*curso)->dir = NULL;
        (*curso)->altura = 0;
        sucesso = 1;
    }
    else
    {
        if (info->codigo_curso == (*curso)->info->codigo_curso)
            sucesso = 0;
        else if (info->codigo_curso < (*curso)->info->codigo_curso)
            sucesso = cadastrar_curso(&((*curso)->esq), info);
        else
            sucesso = cadastrar_curso(&((*curso)->dir), info);
        if (sucesso == 1)
        {
            balanceamento_curso(curso);
            (*curso)->altura = altura_curso(*curso);
        }
    }
    return (sucesso);
}
// ---------------------------------------------------- XXXXXX -------------------------------------------------
// III -  Cadastrar disciplinas a qualquer momento em uma árvore de disciplinas de um determinado curso, ou
// seja, um disciplina só pode ser cadastrada se o curso já estiver sido cadastrado, além disso, o período da
// disciplina deve ser válido, ou seja, estar entre 1 e a quantidade máxima de períodos do curso. A carga
// horária da disciplina deve ser múltiplo de 15, variando entre 30 e 90.
// ---------------------------------------------------- XXXXXX -------------------------------------------------
// Função para obter a altura de uma árvore de disciplinas
int altura_disciplina(Arv_Disciplina *disciplina)
{
    int altura = 0;
    if (disciplina != NULL)
        altura = disciplina->altura;
    return (altura);
}

// Função para atualizar a altura de uma árvore de disciplinas
void atualizar_altura_disciplina(Arv_Disciplina *disciplina)
{
    if (disciplina != NULL)
    {
        int altura_esq = altura_disciplina(disciplina->esq);
        int altura_dir = altura_disciplina(disciplina->dir);

        if (altura_esq > altura_dir)
            disciplina->altura = 1 + altura_esq;
        else
            disciplina->altura = 1 + altura_dir;
    }
}

// Função para calcular o fator de balanceamento de uma árvore de disciplinas
int fator_balanceamento_disciplina(Arv_Disciplina *disciplina)
{
    int fb = 0;
    if (disciplina != NULL)
        fb = altura_disciplina(disciplina->esq) - altura_disciplina(disciplina->dir);
    return (fb);
}

// Rotação à direita
void rotacao_dir_disciplina(Arv_Disciplina **disciplina)
{
    Arv_Disciplina *nova_raiz = (*disciplina)->esq;
    (*disciplina)->esq = nova_raiz->dir;
    nova_raiz->dir = *disciplina;

    atualizar_altura_disciplina(*disciplina);
    atualizar_altura_disciplina(nova_raiz);

    *disciplina = nova_raiz;
}

// Rotação à esquerda
void rotacao_esq_disciplina(Arv_Disciplina **disciplina)
{
    Arv_Disciplina *nova_raiz = (*disciplina)->dir;
    (*disciplina)->dir = nova_raiz->esq;
    nova_raiz->esq = *disciplina;

    atualizar_altura_disciplina(*disciplina);
    atualizar_altura_disciplina(nova_raiz);

    *disciplina = nova_raiz;
}

int validar_cargahoraria(int cargahoraria)
{
    int validar = 0;
    if (cargahoraria % 15 == 0 && cargahoraria >= 30 && cargahoraria <= 90)
        validar = 1;

    return (validar);
}

int validar_periodo(Arv_Cursos *curso, int periodo)
{
    int validar = 0;
    if (periodo >= 1 && periodo <= curso->info->quantidade_periodos)
        validar = 1;

    return (validar);
}

// Função de balanceamento da árvore de disciplinas
void balanceamento_disciplina(Arv_Disciplina **disciplina)
{
    int fb = fator_balanceamento_disciplina(*disciplina);

    if (fb == 2)
    {
        if (fator_balanceamento_disciplina((*disciplina)->esq) < 0)
            rotacao_esq_disciplina(&((*disciplina)->esq));
        rotacao_dir_disciplina(disciplina);
    }
    else if (fb == -2)
    {
        if (fator_balanceamento_disciplina((*disciplina)->dir) > 0)
            rotacao_dir_disciplina(&((*disciplina)->dir));
        rotacao_esq_disciplina(disciplina);
    }
}

int inserir_na_disciplina(Arv_Disciplina **disciplina, Disciplinas_Info *info)
{
    int sucesso = 0;
    if ((*disciplina) == NULL)
    {
        *disciplina = (Arv_Disciplina *)malloc(sizeof(Arv_Disciplina));
        (*disciplina)->info = info;
        (*disciplina)->esq = NULL;
        (*disciplina)->dir = NULL;
        (*disciplina)->altura = 0;
        sucesso = 1;
    }
    else
    {
        if (info->codigo_disciplina < (*disciplina)->info->codigo_disciplina)
            sucesso = inserir_na_disciplina(&(*disciplina)->esq, info);
        else if (info->codigo_disciplina > (*disciplina)->info->codigo_disciplina)
            sucesso = inserir_na_disciplina(&(*disciplina)->dir, info);
        else
            sucesso = 0;

        if (sucesso == 1)
        {
            balanceamento_disciplina(disciplina);
            (*disciplina)->altura = altura_disciplina(*disciplina);
        }
    }

    return (sucesso);
}

// Função para cadastrar disciplina no curso
int cadastrar_disciplina(Arv_Cursos **curso, Disciplinas_Info *no, int codigo_curso)
{
    int sucesso = 0;

    if (validar_cargahoraria(no->carga_horaria))
    {
        if (*curso != NULL)
        {
            if ((*curso)->info != NULL && (*curso)->info->codigo_curso == codigo_curso)
            {
                if (validar_periodo(*curso, no->periodo))
                    sucesso = inserir_na_disciplina(&(*curso)->info->disciplina, no);
            }
            else
            {
                if (codigo_curso < (*curso)->info->codigo_curso)
                    sucesso = cadastrar_disciplina(&(*curso)->esq, no, codigo_curso);
                else
                    sucesso = cadastrar_disciplina(&(*curso)->dir, no, codigo_curso);
            }
        }
    }

    return (sucesso);
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// IV - Cadastrar uma matrícula, onde a mesma é uma árvore organizada e contendo somente um código de
// uma disciplina do curso do aluno.
// ---------------------------------------------------- XXXXXX -------------------------------------------------

int altura_matricula(Arv_Matricula *matricula)
{
    int altura = 0;
    if (matricula != NULL)
        altura = matricula->altura;

    return (altura);
}

void atualizar_altura_matricula(Arv_Matricula *matricula)
{
    if (matricula != NULL)
    {
        int altura_esq = altura_matricula(matricula->esq);
        int altura_dir = altura_matricula(matricula->dir);

        if (altura_esq > altura_dir)
            matricula->altura = 1 + altura_esq;
        else
            matricula->altura = 1 + altura_dir;
    }
}

int fator_balanceamento_matricula(Arv_Matricula *matricula)
{
    int fb = 0;
    if (matricula != NULL)
        fb = altura_matricula(matricula->esq) - altura_matricula(matricula->dir);

    return (fb);
}

void rotacao_dir_matricula(Arv_Matricula **matricula)
{
    Arv_Matricula *nova_raiz = (*matricula)->esq;
    (*matricula)->esq = nova_raiz->dir;
    nova_raiz->dir = *matricula;
    atualizar_altura_matricula(*matricula);
    atualizar_altura_matricula(nova_raiz);
    *matricula = nova_raiz;
}

void rotacao_esq_matricula(Arv_Matricula **matricula)
{
    Arv_Matricula *nova_raiz = (*matricula)->dir;
    (*matricula)->dir = nova_raiz->esq;
    nova_raiz->esq = *matricula;
    atualizar_altura_matricula(*matricula);
    atualizar_altura_matricula(nova_raiz);
    *matricula = nova_raiz;
}

void balanceamento_matricula(Arv_Matricula **matricula)
{
    int fb = fator_balanceamento_matricula(*matricula);

    if (fb == 2)
    {
        if (fator_balanceamento_matricula((*matricula)->esq) < 0)
            rotacao_esq_matricula(&((*matricula)->esq));
        rotacao_dir_matricula(matricula);
    }
    else if (fb == -2)
    {
        if (fator_balanceamento_matricula((*matricula)->dir) > 0)
            rotacao_dir_matricula(&((*matricula)->dir));
        rotacao_esq_matricula(matricula);
    }
}

void inserir_matricula_avl(Arv_Matricula **matricula, int codigo, int *igual)
{
    if (*matricula == NULL)
    {
        Arv_Matricula *novo = (Arv_Matricula *)malloc(sizeof(Arv_Matricula));
        novo->info = (Matriculas_Info *)malloc(sizeof(Matriculas_Info));
        novo->info->codigo_disciplina = codigo;
        novo->altura = 1;
        novo->esq = NULL;
        novo->dir = NULL;
        *matricula = novo;
    }
    else if (codigo < (*matricula)->info->codigo_disciplina)
        inserir_matricula_avl(&(*matricula)->esq, codigo, igual);
    else if (codigo > (*matricula)->info->codigo_disciplina)
        inserir_matricula_avl(&(*matricula)->dir, codigo, igual);
    else
        *igual = -1;

    atualizar_altura_matricula(*matricula);
    balanceamento_matricula(matricula);
}

void cadastrar_matricula(Alunos **aluno, int codigo, int matricula)
{
    if (*aluno != NULL)
    {
        if ((*aluno)->matricula == matricula)
        {
            int igual = 0;
            inserir_matricula_avl(&(*aluno)->mat, codigo, &igual);
        }
        else
            cadastrar_matricula(&(*aluno)->prox, codigo, matricula);
    }
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// V - Cadastrar Notas, permitir o cadastro de notas somente de disciplinas que estejam na árvore de
// matricula, e quando a nota for cadastrada a disciplina deve ser removida da árvore de matricula para
// árvore de notas.
// ---------------------------------------------------- XXXXXX -------------------------------------------------
int altura_nota(Arv_Notas *nota)
{
    int altura = 0;
    if (nota != NULL)
        altura = nota->altura;

    return (altura);
}

void atualizar_altura_nota(Arv_Notas *nota)
{
    if (nota != NULL)
    {
        int altura_esq = altura_nota(nota->esq);
        int altura_dir = altura_nota(nota->dir);

        if (altura_esq > altura_dir)
            nota->altura = 1 + altura_esq;
        else
            nota->altura = 1 + altura_dir;
    }
}

int fator_balanceamento_nota(Arv_Notas *nota)
{
    int fb = 0;
    if (nota != NULL)
        fb = altura_nota(nota->esq) - altura_nota(nota->dir);
    return (fb);
}

void rotacao_dir_notas(Arv_Notas **notas)
{
    Arv_Notas *nova_raiz = (*notas)->esq;
    (*notas)->esq = nova_raiz->dir;
    nova_raiz->dir = *notas;
    atualizar_altura_nota(*notas);
    atualizar_altura_nota(nova_raiz);
    *notas = nova_raiz;
}

void rotacao_esq_notas(Arv_Notas **notas)
{
    Arv_Notas *nova_raiz = (*notas)->dir;
    (*notas)->dir = nova_raiz->esq;
    nova_raiz->esq = *notas;
    atualizar_altura_nota(*notas);
    atualizar_altura_nota(nova_raiz);
    *notas = nova_raiz;
}

void balanceamento_nota(Arv_Notas **nota)
{
    int fb = fator_balanceamento_nota(*nota);

    if (fb == 2)
    {
        if (fator_balanceamento_nota((*nota)->esq) < 0)
            rotacao_esq_notas(&((*nota)->esq));
        rotacao_dir_notas(nota);
    }
    else if (fb == -2)
    {
        if (fator_balanceamento_nota((*nota)->dir) > 0)
            rotacao_dir_notas(&((*nota)->dir));
        rotacao_esq_notas(nota);
    }
}

int cadastrar_nota_aux(Arv_Notas **nota, Notas_Info *info)
{
    int sucesso = 1;
    if (*nota == NULL)
    {
        Arv_Notas *novo = (Arv_Notas *)malloc(sizeof(Arv_Notas));
        novo->info = info;
        novo->esq = NULL;
        novo->dir = NULL;
        *nota = novo;
    }
    else
    {
        if (info->codigo_disciplina == (*nota)->info->codigo_disciplina)
            sucesso = 0;
        else
        {
            if (info->codigo_disciplina < (*nota)->info->codigo_disciplina)
                sucesso = cadastrar_nota_aux(&((*nota)->esq), info);
            else
                sucesso = cadastrar_nota_aux(&((*nota)->dir), info);
        }

        balanceamento_nota(nota);
        atualizar_altura_nota(*nota);
    }
    return (sucesso);
}

int cadastrar_notas(Alunos **aluno, int matricula, Notas_Info *info)
{
    int(encontrado) = 0;
    if (*aluno != NULL)
    {
        if ((*aluno)->matricula == matricula)
        {
            int enc_disc = 0;
            buscar_disciplina((*aluno)->mat, info->codigo_disciplina, &enc_disc);
            if (enc_disc == 1)
            {
                if (cadastrar_nota_aux(&(*aluno)->notas, info) == 1)
                {
                    remover_matricula(&(*aluno)->mat, info->codigo_disciplina);
                    encontrado = 1;
                }
            }
        }
        else
            encontrado = cadastrar_notas(&(*aluno)->prox, matricula, info);
    }

    return (encontrado);
}


// ---------------------------------------------------- XXXXXX -------------------------------------------------
// XIV - Permita remover uma disciplina da árvore de matrícula de um determinado aluno.
// ---------------------------------------------------- XXXXXX -------------------------------------------------
void buscar_disciplina(Arv_Matricula *matricula, int codigo, int *encontrado)
{
    if (matricula != NULL)
    {
        if (matricula->info->codigo_disciplina == codigo)
            *encontrado = 1;
        else if (codigo < matricula->info->codigo_disciplina)
            buscar_disciplina(matricula->esq, codigo, encontrado);
        else
            buscar_disciplina(matricula->dir, codigo, encontrado);
    }
}

int e_folha_matricula(Arv_Matricula *matricula)
{
    return (matricula->esq == NULL && matricula->dir == NULL);
}

Arv_Matricula *so_um_filho_matricula(Arv_Matricula *matricula)
{
    Arv_Matricula *aux;
    aux = NULL;
    if (matricula->esq == NULL)
        aux = matricula->dir;
    else if (matricula->dir == NULL)
        aux = matricula->esq;

    return (aux);
}

Arv_Matricula *menor_filho_esq_matricula(Arv_Matricula *matricula)
{
    Arv_Matricula *aux;
    aux = NULL;
    if (matricula)
    {
        aux = menor_filho_esq_matricula(matricula->esq);
        if (!aux)
            aux = matricula;
    }
    return (aux);
}

void remover_matricula(Arv_Matricula **matricula, int codigo)
{
    if (*matricula != NULL)
    {
        if ((*matricula)->info->codigo_disciplina == codigo)
        {
            Arv_Matricula *aux;

            if (e_folha_matricula(*matricula))
            {
                aux = *matricula;
                free(aux);
                *matricula = NULL;
            }
            else if ((aux = so_um_filho_matricula(*matricula)) != NULL)
            {
                Arv_Matricula *temp;
                temp = *matricula;
                free(temp);
                *matricula = aux;
            }
            else
            {
                Arv_Matricula *menorfilho = menor_filho_esq_matricula((*matricula)->dir);
                (*matricula)->info->codigo_disciplina = menorfilho->info->codigo_disciplina;
                remover_matricula(&(*matricula)->dir, menorfilho->info->codigo_disciplina);
            }
        }
        else if (codigo < (*matricula)->info->codigo_disciplina)
            remover_matricula(&(*matricula)->esq, codigo);
        else
            remover_matricula(&(*matricula)->dir, codigo);
    }
    if (*matricula != NULL)
    {
        balanceamento_matricula(matricula);
        atualizar_altura_matricula(*matricula);
    }
}


// ---------------------------------------------------- XXXXXX -------------------------------------------------
// VI -  Mostrar todos os alunos de um determinado curso.
// ---------------------------------------------------- XXXXXX -------------------------------------------------

void alunos_por_curso(Alunos *aluno, int codigo_curso)
{
    if (aluno != NULL)
    {
        if (aluno->codigo_curso == codigo_curso)
        {
            printf("Nome: %s\n", aluno->nome);
            printf("Matricula: %d\n", aluno->matricula);
            printf("\n");
        }
        alunos_por_curso(aluno->prox, codigo_curso);
    }
}
// ---------------------------------------------------- XXXXXX -------------------------------------------------
// VII - Mostrar todos os cursos do Campus
// ---------------------------------------------------- XXXXXX -------------------------------------------------
void exibir_curso(Arv_Cursos *raiz)
{
    if (raiz != NULL)
    {
        printf("Nome do curso: %s | ID: %d\n", raiz->info->nome_curso, raiz->info->codigo_curso);
        exibir_curso(raiz->esq);
        exibir_curso(raiz->dir);
    }
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// VIII - Mostrar todas as disciplinas de um determinado curso.
// ---------------------------------------------------- XXXXXX -------------------------------------------------
void exibir_disciplinas_por_curso_recursivamente(Arv_Disciplina *disciplina)
{
    if (disciplina != NULL)
    {

        printf("Codigo: %d\n", disciplina->info->codigo_disciplina);
        printf("Nome: %s\n", disciplina->info->nome_disciplina);
        printf("Periodo: %d\n", disciplina->info->periodo);
        printf("Carga Horaria: %d\n", disciplina->info->carga_horaria);
        printf("-------------------------------\n");

        exibir_disciplinas_por_curso_recursivamente(disciplina->esq);
        exibir_disciplinas_por_curso_recursivamente(disciplina->dir);
    }
}

void exibir_disciplinas_por_curso(Arv_Cursos *curso, int codigo_curso)
{
    if (curso != NULL)
    {
        if (curso->info->codigo_curso == codigo_curso)
        {
            Arv_Disciplina *disciplina_atual = curso->info->disciplina;
            printf("Disciplinas do curso %s:\n", curso->info->nome_curso);
            printf("-------------------------------\n");

            exibir_disciplinas_por_curso_recursivamente(disciplina_atual);
        }

        exibir_disciplinas_por_curso(curso->esq, codigo_curso);
        exibir_disciplinas_por_curso(curso->dir, codigo_curso);
    }
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// IX - Mostrar todas as disciplinas de um determinado período de um curso.
// ---------------------------------------------------- XXXXXX -------------------------------------------------
void exibir_disciplina_periodo(Arv_Cursos *curso, int periodo)
{
    if (curso != NULL)
    {
        if (curso->info->disciplina != NULL)
        {
            if (curso->info->disciplina->info->periodo == periodo)
            {
                printf("Curso: %s | Disciplina: %s | Período: %d\n", curso->info->nome_curso, curso->info->disciplina->info->nome_disciplina, curso->info->disciplina->info->periodo);
                exibir_disciplina_periodo(curso->esq, periodo);
                exibir_disciplina_periodo(curso->dir, periodo);
            }
        }
    }
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// X - Mostrar todas as disciplinas que um determinado aluno está matriculado
// ---------------------------------------------------- XXXXXX -------------------------------------------------
void exibir_disciplinas(Arv_Disciplina *disciplina, int codigo_disciplina)
{
    if (disciplina != NULL)
    {
        if (disciplina->info->codigo_disciplina == codigo_disciplina)
        {
            printf("Codigo: %d\n", disciplina->info->codigo_disciplina);
            printf("Nome: %s\n", disciplina->info->nome_disciplina);
            printf("Carga horaria: %d\n", disciplina->info->carga_horaria);
            printf("Periodo: %d\n", disciplina->info->periodo);
            printf("\n");
        }
        else if (codigo_disciplina < disciplina->info->codigo_disciplina)
            exibir_disciplinas(disciplina->esq, codigo_disciplina);
        else
            exibir_disciplinas(disciplina->dir, codigo_disciplina);
    }
}

void exibir_disciplina(Arv_Cursos *curso, int codigo_disciplina, int codigo_curso)
{
    if (curso != NULL)
    {
        if (curso->info->codigo_curso == codigo_curso)
        {
            if (curso->info->disciplina != NULL)
                exibir_disciplinas(curso->info->disciplina, codigo_disciplina);
        }
        else if (codigo_curso < curso->info->codigo_curso)
            exibir_disciplina(curso->esq, codigo_disciplina, codigo_curso);
        else
            exibir_disciplina(curso->dir, codigo_disciplina, codigo_curso);
    }
}

void exibir_disciplina_aluno(Arv_Matricula *matricula, Arv_Cursos *cursos, int codigo_curso)
{
    if (matricula != NULL)
    {
        exibir_disciplina(cursos, matricula->info->codigo_disciplina, codigo_curso);
        exibir_disciplina_aluno(matricula->esq, cursos, codigo_curso);
        exibir_disciplina_aluno(matricula->dir, cursos, codigo_curso);
    }
}

void exibir_disciplinas_por_aluno(Alunos *aluno, Arv_Cursos *cursos, int matricula)
{
    if (aluno != NULL)
    {
        if (aluno->matricula == matricula)
            exibir_disciplina_aluno(aluno->mat, cursos, aluno->codigo_curso);
        else
            exibir_disciplinas_por_aluno(aluno->prox, cursos, matricula);
    }
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// XI -  Mostrar todas as notas de disciplinas de um determinado período de um determinado aluno.
// ---------------------------------------------------- XXXXXX -------------------------------------------------

void auxiliar_notas_periodo(Arv_Notas *nota, int periodo)
{
    if (nota != NULL)
    {
        if (nota->info->semestre == periodo)
        {
            printf("Codigo: %d\n", nota->info->codigo_disciplina);
            printf("Nota Final: %.2f\n", nota->info->nota_final);
            printf("Semestre: %.2f\n", nota->info->semestre);
            printf("\n");
        }
        auxiliar_notas_periodo(nota->esq, periodo);
        auxiliar_notas_periodo(nota->dir, periodo);
    }
}

void notas_disciplina_periodo_aluno(Alunos *aluno, int periodo, int matricula)
{
    if (aluno != NULL)
    {
        if (aluno->matricula == matricula)
            auxiliar_notas_periodo(aluno->notas, periodo);
        else
            notas_disciplina_periodo_aluno(aluno->prox, periodo, matricula);
    }
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// XII - Mostrar a nota de uma disciplina de um determinado aluno, mostrando o período e a carga horária da disciplina
// ---------------------------------------------------- XXXXXX -------------------------------------------------
Arv_Disciplina *buscar_disciplina_xii(Arv_Disciplina *disciplina, int codigo_disciplina)
{
    Arv_Disciplina *resultado = NULL;
    if (disciplina != NULL)
    {
        if (disciplina->info->codigo_disciplina == codigo_disciplina)
            resultado = disciplina;
        else if (codigo_disciplina < disciplina->info->codigo_disciplina)
            resultado = buscar_disciplina_xii(disciplina->esq, codigo_disciplina);
        else
            resultado = buscar_disciplina_xii(disciplina->dir, codigo_disciplina);
    }
    return (resultado);
}

Arv_Notas *buscar_nota_xii(Arv_Notas *nota, int codigo_disciplina)
{
    Arv_Notas *resultado = NULL;
    if (nota != NULL)
    {
        if (nota->info->codigo_disciplina == codigo_disciplina)
            resultado = nota;
        else if (codigo_disciplina < nota->info->codigo_disciplina)
        {
            resultado = buscar_nota_xii(nota->esq, codigo_disciplina);
        }
        else
        {
            resultado = buscar_nota_xii(nota->dir, codigo_disciplina);
        }
    }
    return resultado;
}

void exibir_nota_aluno_disciplina(Alunos *aluno, Arv_Cursos *curso, int matricula, int codigo_disciplina)
{
    Arv_Cursos *var_curso = NULL;
    Arv_Disciplina *var_disc = NULL;
    Arv_Notas *var_nota = NULL;
    if (aluno != NULL)
    {
        if (aluno->matricula == matricula)
        {
            var_curso = buscar_curso(curso, aluno->codigo_curso);
            if (var_curso != NULL)
            {
                var_disc = buscar_disciplina_xii(var_curso->info->disciplina, codigo_disciplina);
                if (var_disc != NULL)
                {
                    var_nota = buscar_nota_xii(aluno->notas, codigo_disciplina);
                    if (var_nota != NULL)
                    {
                        printf("Disciplina: %s\n", var_disc->info->nome_disciplina);
                        printf("Nota: %.2f\n", var_nota->info->nota_final);
                        printf("Periodo: %d\n", var_disc->info->periodo);
                        printf("Carga horaria: %d\n", var_disc->info->carga_horaria);
                    }
                }
            }
        }
        else
            exibir_nota_aluno_disciplina(aluno->prox, curso, matricula, codigo_disciplina);
    }
}
// ---------------------------------------------------- XXXXXX -------------------------------------------------
// XIII - Remover uma disciplina de um determinado curso desde que não tenha nenhum aluno matriculado na
// mesma.
// ---------------------------------------------------- XXXXXX -------------------------------------------------

int e_folha(Arv_Disciplina *disciplina)
{
    return (disciplina->esq == NULL && disciplina->dir == NULL);
}

Arv_Disciplina *so_um_filho(Arv_Disciplina *disciplina)
{
    Arv_Disciplina *aux = NULL;

    if (disciplina->dir == NULL)
        aux = disciplina->esq;
    else if (disciplina->esq == NULL)
        aux = disciplina->dir;

    return (aux);
}

Arv_Disciplina *menor_filho_esquerda(Arv_Disciplina *disciplina)
{
    Arv_Disciplina *aux = NULL;

    if (disciplina != NULL)
        aux = menor_filho_esquerda(disciplina->esq);
    if (!aux)
        aux = disciplina;

    return (aux);
}

void remover_disciplina(Arv_Disciplina **disciplina, int codigo_disciplina, int *remove)
{
    if ((*disciplina != NULL))
    {
        Arv_Disciplina *aux;
        Arv_Disciplina *endereco_filho;
        Arv_Disciplina *endereco_menor_filho;
        if ((*disciplina)->info->codigo_disciplina == codigo_disciplina)
        {
            if (e_folha(*disciplina))
            {
                aux = *disciplina;
                free(aux);
                *disciplina = NULL;
            }
            else if ((endereco_filho = so_um_filho(*disciplina)) != NULL)
            {
                aux = *disciplina;
                free(aux);
                *disciplina = endereco_filho;
            }
            else
            {
                endereco_menor_filho = menor_filho_esquerda((*disciplina)->dir);
                (*disciplina)->info->codigo_disciplina = endereco_menor_filho->info->codigo_disciplina;
                (*disciplina)->info->carga_horaria = endereco_menor_filho->info->carga_horaria;
                (*disciplina)->info->periodo = endereco_menor_filho->info->periodo;
                strcpy((*disciplina)->info->nome_disciplina, endereco_menor_filho->info->nome_disciplina);
                remover_disciplina(&(*disciplina)->dir, endereco_menor_filho->info->codigo_disciplina, remove);
            }
            *remove = 1;
        }
        else if (codigo_disciplina < (*disciplina)->info->codigo_disciplina)
            remover_disciplina(&(*disciplina)->esq, codigo_disciplina, remove);
        else
            remover_disciplina(&(*disciplina)->dir, codigo_disciplina, remove);
    }
    if (*disciplina != NULL)
    {
        balanceamento_disciplina(disciplina);
        atualizar_altura_disciplina(*disciplina);
    }
}

void confirmar_remocao_disciplina(Alunos *alunos, int codigo_disciplina, int *confirmar)
{
    int encontrado = 0;

    if (alunos != NULL)
    {
        buscar_matricula(alunos->mat, codigo_disciplina, &encontrado);

        if (encontrado != 0)
            *confirmar = 1;

        confirmar_remocao_disciplina(alunos->prox, codigo_disciplina, confirmar);
    }
}

int remover_disciplina_curso(Arv_Cursos **cursos, Alunos *alunos, int codigo_curso, int codigo_disciplina)
{
    int remove = 0, confirmar_disciplina = 0;
    if ((*cursos))
    {
        confirmar_remocao_disciplina(alunos, codigo_disciplina, &confirmar_disciplina);
        if (confirmar_disciplina == 0)
        {
            if ((*cursos)->info->codigo_curso == codigo_curso)
                remover_disciplina(&(*cursos)->info->disciplina, codigo_disciplina, &remove);
            else if (codigo_curso < (*cursos)->info->codigo_curso)
                remove = remover_disciplina_curso(&(*cursos)->esq, alunos, codigo_curso, codigo_disciplina);
            else
                remove = remover_disciplina_curso(&(*cursos)->dir, alunos, codigo_curso, codigo_disciplina);
        }
    }
    return (remove);
}
// ---------------------------------------------------- XXXXXX -------------------------------------------------
// XV - Mostrar o histórico de um determinado aluno, contendo o nome do curso, as disciplinas e sua respectiva
// nota organizadas pelo período que a disciplina está cadastrada no curso.
// ---------------------------------------------------- XXXXXX -------------------------------------------------
void exibir_disciplina_historico(Arv_Disciplina *disciplina, int codigo_disciplina)
{
    if (disciplina != NULL)
    {
        if (disciplina->info->codigo_disciplina == codigo_disciplina)
            printf("Disciplina: %s\n", disciplina->info->nome_disciplina);
        else if (codigo_disciplina < disciplina->info->codigo_disciplina)
            exibir_disciplina_historico(disciplina->esq, codigo_disciplina);
        else
            exibir_disciplina_historico(disciplina->dir, codigo_disciplina);
    }
}

void exibir_notas_alunos(Arv_Notas *nota, Arv_Disciplina *disciplina, int periodo)
{
    if (nota != NULL)
    {
        if (nota->info->semestre == periodo)
        {
            exibir_disciplina_historico(disciplina, nota->info->codigo_disciplina);
            printf("\n------------------\n");
            printf("Semestre: %.1f\n\n", nota->info->semestre);
            printf("Nota: %.2f\n", nota->info->nota_final);
            printf("\n------------------\n");
        }
        exibir_notas_alunos(nota->esq, disciplina, periodo);
        exibir_notas_alunos(nota->dir, disciplina, periodo);
    }
}

int exibir_nome_do_curso(Arv_Cursos *curso, int codigo_curso)
{
    int count_periodos = 0;
    if (curso != NULL)
    {
        if (curso->info->codigo_curso == codigo_curso)
        {
            printf("Curso: %s\n", curso->info->nome_curso);
            count_periodos = curso->info->quantidade_periodos;
        }
        else if (codigo_curso < curso->info->codigo_curso)
            count_periodos = exibir_nome_do_curso(curso->esq, codigo_curso);
        else
            count_periodos = exibir_nome_do_curso(curso->dir, codigo_curso);
    }
    return (count_periodos);
}

void consultar_historico(Alunos *aluno, Arv_Cursos *curso, int matricula)
{
    if (aluno != NULL)
    {
        if (aluno->matricula == matricula)
        {
            printf("\n------------------\n");
            printf("Matricula: %d\n", aluno->matricula);
            printf("Aluno: %s\n", aluno->nome);
            printf("\n------------------\n");
            int count_periodos = 0;
            count_periodos = exibir_nome_do_curso(curso, aluno->codigo_curso);
            printf("Historico:\n");
            for (int i = 0; i < count_periodos; i++)
                exibir_notas_alunos(aluno->notas, curso->info->disciplina, i + 1);
        }
        else
            consultar_historico(aluno->prox, curso, matricula);
    }
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// Funções auxiliares
// ---------------------------------------------------- XXXXXX -------------------------------------------------
Arv_Cursos *buscar_curso(Arv_Cursos *curso, int codigo_curso)
{
    Arv_Cursos *resultado = NULL;

    if (curso != NULL)
    {
        if (curso->info->codigo_curso == codigo_curso)
            resultado = curso;
        else if (codigo_curso < curso->info->codigo_curso)
            resultado = buscar_curso(curso->esq, codigo_curso);
        else
            resultado = buscar_curso(curso->dir, codigo_curso);
    }

    return (resultado);
}

void buscar_matricula(Arv_Matricula *r, int codigo, int *encontrado)
{
    if (r != NULL)
    {
        if (r->info->codigo_disciplina == codigo)
            *encontrado = 1;
        else
        {
            if (codigo < r->info->codigo_disciplina)
                buscar_matricula(r->esq, codigo, encontrado);
            else
                buscar_matricula(r->dir, codigo, encontrado);
        }
    }
}

Alunos *buscar_aluno(Alunos *aluno, int matricula)
{
    while (aluno != NULL && aluno->matricula != matricula)
        aluno = aluno->prox;

    return aluno;
}