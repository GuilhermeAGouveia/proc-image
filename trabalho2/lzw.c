/*-------------------------------------------------
  |         Unifal - Universidade Federal de Alfenas.
  |             BACHARELADO EM CIENCIA DA COMPUTACAO.
  | Trabalho..: Descompactador do formato LZW-Base64
  | Disciplina: Processamento de Imagens
  | Professor.: Luiz Eduardo da Silva
  | Aluno.....: Guilherme Augusto Gouveia
  | Data......: 02/08/2022
  -------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lzw.h"

typedef struct alocaInfo
{
    int *base;
    int tam;
    int *proxAloca;
} alocaInfo;

void iniciaAloca(alocaInfo *aloc, int tam)
{
    aloc->base = malloc(tam);
    aloc->tam = tam;
    aloc->proxAloca = aloc->base;
}

int *aloca(alocaInfo *aloc, int len)
{
    int *ret = aloc->proxAloca;
    aloc->proxAloca += len;
    return ret;
}

void codifica_lzw(int *in, int n)
{
    int dicionario[TAM_DICT][NUM_SIMB];
    int posDict;
    int simbolo;
    int corrent = in[0];

    memset(dicionario, 0, TAM_DICT * NUM_SIMB * sizeof(int));
    posDict = NUM_SIMB;
    int i = 1;

    while (i < n)
    {
        simbolo = in[i++];
        int prox = dicionario[corrent][simbolo];
        if (prox != 0)
        {
            corrent = prox;
        }
        else
        {
            printf("[%d]", corrent);
            if (posDict < TAM_DICT)
                dicionario[corrent][simbolo] = posDict++;
            else
            {
                memset(dicionario, 0, TAM_DICT * NUM_SIMB * sizeof(int));
                posDict = NUM_SIMB;
            }

            corrent = simbolo;
        }
    };
    printf("[%d]", corrent);
}

int *decodifica_lzw(int *in, int n, int *out)
{

    struct
    {
        int *seq;
        int tam;
    } dicionario[TAM_DICT];

    alocaInfo aInfo;
    int *marca;
    int posDict;
    int anterior;
    int i;

    iniciaAloca(&aInfo, TAM_DICT * TAM_DICT * sizeof(int));
    marca = aInfo.proxAloca;
    for (int i = 0; i < NUM_SIMB; i++)
    {
        dicionario[i].seq = aloca(&aInfo, 1);
        dicionario[i].seq[0] = i;
        dicionario[i].tam = 1;
    }

    posDict = NUM_SIMB;
    anterior = in[0];

    out[0] = anterior;

    i = 1;
    int j = 1;
    while (i < n)
    {
        int simbolo = in[i++];
        if (posDict == TAM_DICT)
        {
            aInfo.proxAloca = marca;
            for (int i = 0; i < NUM_SIMB; i++)
            {
                dicionario[i].seq = aloca(&aInfo, 1);
                dicionario[i].seq[0] = i;
                dicionario[i].tam = 1;
            }
            posDict = NUM_SIMB;
        }
        else
        {
            int tam = dicionario[anterior].tam;
            dicionario[posDict].tam = tam + 1;
            dicionario[posDict].seq = aloca(&aInfo, tam + 1);
            for (int k = 0; k < tam; k++)
            {
                dicionario[posDict].seq[k] = dicionario[anterior].seq[k];
            }
            if (simbolo == posDict)
                dicionario[posDict++].seq[tam] = dicionario[anterior].seq[0];
            else
                dicionario[posDict++].seq[tam] = dicionario[simbolo].seq[0];
        }

        for (int k = 0; k < dicionario[simbolo].tam; k++)
        {
            out[j++] = dicionario[simbolo].seq[k];
        }

        anterior = simbolo;
    }

    free(aInfo.base);
    puts("");
}