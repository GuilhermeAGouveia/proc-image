/*-------------------------------------------------------------------------
 * Image Processing using C-Ansi
 *   Program: Negative from grayscale image
 * By Luiz Eduardo da Silva.
 *-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "imagelib.h"

int normalize(int valor, int range)
{
    int vet[range], i = 1;
    for (; valor > i * 255 / (range); i++)
        ;

    // printf("%d -> %d\n", valor, i);

    return i > range - 1 ? 6 : i;
}

void dado(image In, image Out, int nl, int nc, int mn, float scaleFator)
{
    int valor;
    int tmp;
    int newC = nc * scaleFator;
    int newL = nl * scaleFator;
    char namefile[30];

    image listDados[7] = {
        img_get("./dados/preto-0.pgm", &tmp, &tmp, &tmp, GRAY, 0),
        img_get("./dados/preto-1.pgm", &tmp, &tmp, &tmp, GRAY, 0),
        img_get("./dados/preto-2.pgm", &tmp, &tmp, &tmp, GRAY, 0),
        img_get("./dados/preto-3.pgm", &tmp, &tmp, &tmp, GRAY, 0),
        img_get("./dados/preto-4.pgm", &tmp, &tmp, &tmp, GRAY, 0),
        img_get("./dados/preto-5.pgm", &tmp, &tmp, &tmp, GRAY, 0),
        img_get("./dados/preto-6.pgm", &tmp, &tmp, &tmp, GRAY, 0),

    };

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
    //-- create output image
    Out = img_alloc(nr * fator * 40, nc * fator * 40);
    //-- transformation
    dado(In, Out, nr, nc, ml, fator);

    //-- save image
    img_put(Out, nameOut, nr * fator * 40, nc * fator * 40, ml, GRAY);
    //-- show image
    sprintf(cmd, "%s %s &", VIEW, nameOut);
    system(cmd);
    img_free(In);
    img_free(Out);
    return 0;
}
