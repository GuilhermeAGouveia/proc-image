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
typedef struct
{
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
    {1, 0}};

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
    {1, -1}};

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
    {0, 1}};


image erosao(image In, pto *B, int N)
{
    int i, j;

    int nl = In->nr;
    int nc = In->nc;
    int mn = In->ml;
    int tp = In->tp;
    int *px = In->px;

    image Out = img_clone(In);

    for (i = 1; i < nl - 1; i++)
        for (j = 1; j < nc - 1; j++)
        {
            int min = 1000;
            for (int x = -1; x <= 1; x++)
            {
                for (int y = -1; y <= 1; y++)
                {
                    int lin = i + x;
                    int col = j + y;
                    int px1 = In->px[lin * nc + col];
                    if (px1 < min)
                        min = px1;
                }
            }
            Out->px[i * nc + j] = min;
        }

    return Out;
}

image dilatacao(image In, pto *B, int N)
{
    int i, j;

    int nl = In->nr;
    int nc = In->nc;
    int mn = In->ml;
    int tp = In->tp;
    int *px = In->px;

    image Out = img_clone(In);

    for (i = 1; i < nl - 1; i++)
        for (j = 1; j < nc - 1; j++)
        {
            int max = -1;
            for (int x = -1; x <= 1; x++)
            {
                for (int y = -1; y <= 1; y++)
                {
                    int lin = i + x;
                    int col = j + y;
                    int px1 = In->px[lin * nc + col];
                    if (px1 > max)
                        max = px1;
                }
            }
            Out->px[i * nc + j] = max;
        }

    return Out;
}

image diffImage(image In1, image In2)
{
    int nl = In1->nr;
    int nc = In1->nc;
    int mn = In1->ml;
    int tp = In1->tp;
    int *px1 = In1->px;
    int *px2 = In2->px;

    for (int i = 0; i < nc * nl; i++)
    {
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

    img_name(argv[1], nameIn, nameOut, GRAY, GRAY);

    //-- read image
    In = img_get(nameIn, GRAY);
    //-- transformation
    Out = dilatacao(In, B2, N2);
    //-- save image
    img_put(Out, nameOut, GRAY);

    sprintf(cmd, "%s %s &", VIEW, nameOut);
    system(cmd);
    img_free(In);
    return 0;
}
