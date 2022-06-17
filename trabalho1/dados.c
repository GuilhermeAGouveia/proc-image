/*-------------------------------------------------
  |         Unifal - Universidade Federal de Alfenas.
  |             BACHARELADO EM CIENCIA DA COMPUTACAO.
  | Trabalho..: Imagem de dados
  | Disciplina: Processamento de Imagens
  | Professor.: Luiz Eduardo da Silva
  | Aluno.....: Guilherme Augusto Gouveia
  | Data......: 09/03/2022
  -------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "imagelib.h"

/**
 * @brief Essa funcao faz com que cada valor de entrada seja normalizado para um valor entre 0 e 6
 * 
 * @param valor Valor de entrada
 * @param range valor maximo da normalizacao
 * @return int 
 */
int normalize(int valor, int range)
{
    int vet[range], i = 1;
    for (; valor > i * 255 / (range); i++)
        ;

    return i - 1;
}

/**
 * @brief Essa funcao faz com que uma imagem de entrada tenha suas dimensões reduzidas
 * 
 * @param In Imagem de entrada
 * @param Out Imagem de saída
 * @param nl Numero de linhas da imagem de entrada
 * @param nc Numero de colunas da imagem de entrada
 * @param nlIcone Numero de linhas da imagem de saida
 * @param ncIcone Numero de colunas da imagem de saida
 */

void iconizar(image In, image Out, int nl, int nc, int nlIcone, int ncIcone)
{
    for (int i = 0; i < nlIcone; i++)
    {

        for (int j = 0; j < ncIcone; j++)
        {
            Out[i * ncIcone + j] = In[i * (nl / nlIcone) * nc + j * (nc / ncIcone)];
        }
    }
}

/**
 * @brief Processa cada valor da imagem utilizando uma funcao
 * 
 * @param In Imagem de entrada
 * @param nl Numero de linhas da imagem de entrada
 * @param nc Numero de colunas da imagem de entrada
 * @param maxValue Valor usado por normalize
 */

void normalizeImage(image In, int nl, int nc, int maxValue)
{
    for (int i = 0; i < nl * nc; i++)
    {
        In[i] = normalize(In[i], maxValue);
    }
}

/**
 * @brief Carrega os dados usados na subtituição dos pixels
 * 
 * @param nDados Numero de dados
 * @return image* 
 */
image *loadDados(int nDados)
{
    int tmp;
    char filename[30];
    image *loadDados = malloc(nDados * sizeof(image));
    for (int i = 0; i < 7; i++)
    {
        sprintf(filename, "./dados/preto-%d.pgm", i);
        loadDados[i] = img_get(filename, &tmp, &tmp, &tmp, GRAY, 0);
    }
    return loadDados;
}

/**
 * @brief Libera a memoria dos dados carregados
 * 
 * @param listDados Lista que contem os dados carregados
 * @param nDados Numero de dados
 */
void freeDados(image *listDados, int nDados)
{
    for (int i = 0; i < nDados; i++)
    {
        img_free(listDados[i]);
    }
}

/**
 * @brief Converte cada pixel (Int) da imagem de entrada em um dado (image) na imagem de saída
 * 
 * @param In Imagem de entrada
 * @param Out Imagem de saída
 * @param nl Numero de linhas da imagem de entrada
 * @param nc Numero de colunas da imagem de entrada
 */
void convertIntToDado(image In, image Out, int nl, int nc)
{
    int valorNormalizado, tmp;
    image *listDados = loadDados(7);

    for (int i = 0; i < nl; i++)
    {
        for (int j = 0; j < nc; j++)
        {
            valorNormalizado = In[i * nc + j];
            image Dado = listDados[valorNormalizado];

            for (int a = 0; a < 40; a++)
            {
                for (int b = 0; b < 40; b++)
                {

                    Out[(i * 40 + a) * nc * 40 + j * 40 + b] = Dado[a * 40 + b];
                }
            }
        }
    }
    freeDados(listDados, 7);
}

/**
 * @brief Procedimento que executa as operações
 * 
 * @param In Imagem de entrada
 * @param Out Imagem de saída
 * @param nl Numero de linhas da imagem de entrada
 * @param nc Numero de colunas da imagem de entrada
 * @param mn Maximo valor de cinza
 * @param nlIcone Numero de linhas da imagem de saida
 * @param ncIcone Numero de colunas da imagem de saida
 */
void dado(image In, image Out, int nl, int nc, int mn, int nlIcone, int ncIcone)
{

    // Transformando imagem em outra com 100 de largura e um tamanho proporcial na altura
    image Icone = img_alloc(nlIcone, ncIcone);
    iconizar(In, Icone, nl, nc, nlIcone, ncIcone);
    normalizeImage(Icone, nlIcone, ncIcone, 7);
    convertIntToDado(Icone, Out, nlIcone, ncIcone);
}

void msg(char *s)
{
    printf("\nNegative image");
    printf("\n-------------------------------");
    printf("\nUsage:  %s  image-name[.pnm] tp\n\n", s);
    printf("    image-name[.pnm] is image file file \n");
    printf("    tp = image type (1 = BW, 2 = Gray, 3 = Color)\n\n");
    exit(1);
}

/*-------------------------------------------------------------------------
 * main function
 *-------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    int nc, nr, ml, tp;
    char *p, nameIn[30], nameOut[30], cmd[110];
    image In, Out;
    if (argc < 2)
        msg(argv[0]);
    //-- define input/output file name
    img_name(argv[1], nameIn, nameOut, GRAY);
    //-- read image
    In = img_get(nameIn, &nr, &nc, &ml, GRAY, 1);
    //-- define image factor
    float fator = (float)100 / nc;
    // Obtendo dimensoes para uma imagem com 100 de largura e um tamanho proporcional na altura
    int ncIcone = nc * fator;
    int nlIcone = nr * fator;
    //-- dimensoes da imagem resultante
    int ncResult = ncIcone * 40;
    int nlResult = nlIcone * 40;
    //-- create output image
    Out = img_alloc(nlResult, ncResult);
    //-- transformation
    dado(In, Out, nr, nc, ml, nlIcone, ncIcone);

    //-- save image
    img_put(Out, nameOut, nlResult, ncResult, ml, GRAY);
    //-- show image
    sprintf(cmd, "%s %s &", VIEW, nameOut);
    system(cmd);
    img_free(In);
    img_free(Out);
    return 0;
}

/*

// A funcao abaixo foi a solucao inicial, que fazia todos os processos (iconizar, normalizar, trocar Inteiros por Dados) em um único arranjo de for's
void dado(image In, image Out, int nl, int nc, int mn, float scaleFator)
{
    int valor;
    int tmp;

    //Obtendo dimensoes para uma imagem com 100 de largura e um tamanho proporcional na altura
    int newC = nc * scaleFator;
    int newL = nl * scaleFator;

    //carregando dados
    image listDados[7] = {
        img_get("./dados/preto-0.pgm", &tmp, &tmp, &tmp, GRAY, 0),
        img_get("./dados/preto-1.pgm", &tmp, &tmp, &tmp, GRAY, 0),
        img_get("./dados/preto-2.pgm", &tmp, &tmp, &tmp, GRAY, 0),
        img_get("./dados/preto-3.pgm", &tmp, &tmp, &tmp, GRAY, 0),
        img_get("./dados/preto-4.pgm", &tmp, &tmp, &tmp, GRAY, 0),
        img_get("./dados/preto-5.pgm", &tmp, &tmp, &tmp, GRAY, 0),
        img_get("./dados/preto-6.pgm", &tmp, &tmp, &tmp, GRAY, 0),

    };

    //Transformando imagem em outra com 100 de largura e um tamanho proporcial na altura
    image Dado;
    for (int i = 0; i < newL; i++)
    {
        Dado = NULL;
        for (int j = 0; j < newC && i * (nl / 100) <= nl; j++)
        {
            valor = In[i * (nl / 100) * nc + j * (nc / 100)];

            valor = normalize(valor, 7);

            Dado = listDados[valor];

            for (int a = 0; a < 40; a++)
            {
                for (int b = 0; b < 40; b++)
                {
                    Out[(i * 40 + a) * newC * 40 + j * 40 + b] = Dado[a * 40 + b];
                }
            }
        }
    }
}
 */