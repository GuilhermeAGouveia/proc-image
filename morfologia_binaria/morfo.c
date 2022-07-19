/*-------------------------------------------------------------------------
 * Tranformação de intensidade
 * Por Luiz Eduardo da Silva.
 *-------------------------------------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "imagelib.h"

#define DEBUG(x)
typedef struct {
    int di, dj;
} pto;

/*
 * Elemento Estruturante:
 * . x .
 * x x x
 * . x .
 */
#define N1 5
pto B1[N1] = {
    {-1, 0},
    {0, -1},
    {0, 0},
    {0, 1},
    {1, 0}
};

/*
 * Elemento Estruturante:
 * x x x
 * x . .
 * x . .
 */
#define N2 5
pto B2[N2] = {
    {-1, -1},
    {-1, 0},
    {-1, 1},
    {0, -1},
    {1, -1}
};

/*
 * Elemento Estruturante:
 * . . .
 * x x x
 * . . .
 */
#define N3 3
pto B3[N3] = {
    {0, -1},
    {0, 0},
    {0, 1}
};

int estaContido(image In, int i, int j, pto *B, int N) {
    int k;

    
    int nl = In->nr;
    int nc = In->nc;
    int mn = In->ml;
    int tp = In->tp;
    int *pxPointer = In->px;

    for (k = 0; k < N; k++) {
        int lin = i + B[k].di;
        int col = j + B[k].dj;
        if (lin >= 0 && lin < nl && col >= 0 && col < nc) {
            int px = pxPointer[lin * nc + col];
            DEBUG(printf("px(%d,%d) = %d\n", lin, col, px));
            if (!px)
                return 0;
        } else return 0;
    }
    return 1;
}

int intercepta(image In, int i, int j, pto *B, int N) {
    int k;

    int nl = In->nr;
    int nc = In->nc;
    int mn = In->ml;
    int tp = In->tp;
    int *pxPointer = In->px;

    for (k = 0; k < N; k++) {
        int lin = i - B[k].di;
        int col = j - B[k].dj;
        if (lin >= 0 && lin < nl && col >= 0 && col < nc) {
            int px = pxPointer[lin * nc + col];
            DEBUG(printf("px(%d,%d) = %d\n", lin, col, px));
            if (px)
                return 1;
        } else return 0;
    }
    return 0;
}

image erosao(image In, pto *B, int N) {
    int i, j;

    int nl = In->nr;
    int nc = In->nc;
    int mn = In->ml;
    int tp = In->tp;
    int *px = In->px;

    image Out = img_clone(In);

    for (i = 1; i < nl - 1; i++)
        for (j = 1; j < nc - 1; j++)
            if (estaContido(In, i, j, B, N))
                Out->px[i * nc + j] = 0;
            else
                Out->px[i * nc + j] = 1;
    return Out;
}

image dilatacao(image In, pto *B, int N) {
    int i, j;

    int nl = In->nr;
    int nc = In->nc;
    int mn = In->ml;
    int tp = In->tp;
    int *px = In->px;

    image Out = img_clone(In);

    for (i = 1; i < nl - 1; i++)
        for (j = 1; j < nc - 1; j++)
            if (intercepta(In, i, j, B, N))
                Out->px[i * nc + j] = 0;
            else
                Out->px[i * nc + j] = 1;
    return Out;
}

image diffImage(image In1, image In2) {
    int nl = In1->nr;
    int nc = In1->nc;
    int mn = In1->ml;
    int tp = In1->tp;
    int *px1 = In1->px;
    int *px2 = In2->px;

    for (int i = 0; i < nc * nl; i++) {
        px2[i] = abs(px1[i] - px2[i]);
    } 

    return In2;
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
    char nameIn[100], nameOut[100], cmd[110];
    image In;
    image Out;
    if (argc < 2)
        msg(argv[0]);

    img_name(argv[1], nameIn, nameOut, BW, GRAY);

    //-- read image
    In = img_get(nameIn, BW);
    //-- transformation
    Out = dilatacao(In, B2, N2);
    Out = diffImage(In, Out); // com a função diff obtemos o contorno
    //-- save image
    img_put(Out, nameOut, GRAY);

    sprintf(cmd, "%s %s &", VIEW, nameOut);
    system(cmd);
    img_free(In);
    return 0;
}
