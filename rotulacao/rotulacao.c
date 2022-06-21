/*-------------------------------------------------------------------------
 * Tranformação de intensidade
 * Por Luiz Eduardo da Silva.
 *-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "imagelib.h"

// Naive implementation of find
int find(int parent[], int i)
{
    while (parent[i] != i)
    {
        i = parent[i];
    }
    return i;
}

// Naive implementation of union()
void Union(int parent[], int x, int y)
{
    int xset = find(parent, x);
    int yset = find(parent, y);
    parent[xset] = yset;
}

void rotulacao(image In)
{
    int nl = In->nr;
    int nc = In->nc;
    int mn = In->ml;
    int tp = In->tp;
    int *px = In->px;

    int parent[1000];

    int rotulo = 0;

    for (int i = 0; i < 1000; i++)
    {
        parent[i] = i;
    }

    for (int i = 1; i < nl; i++)
    {
        for (int j = 1; j < nc; j++)
        {
            int p = px[i * nc + j];       // pixel
            int r = px[(i - 1) * nc + j]; // vizinho de cima
            int t = px[i * nc + j - 1];   // vizinho da esquerda

            if (p != 0)
            {
                if (r == 0 && t == 0)
                    px[i * nc + j] = ++rotulo;

                if (r != 0 && t == 0)
                    px[i * nc + j] = r;

                if (r == 0 && t != 0)
                    px[i * nc + j] = t;

                if (r != 0 && t != 0 && t == r)
                    px[i * nc + j] = t;

                if (r != 0 && t != 0 && t != r)
                {
                    px[i * nc + j] = t;
                    //printf("%d eq %d \n", r, t);
                    Union(parent, r, t);
                }
            }
        }
    }
    In->ml = rotulo;

    for (int i = 0; i < nl * nc; i++)
    {
        px[i] = find(parent, px[i]);
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
    char nameIn[100], nameOut[100], cmd[110];
    image In;
    if (argc < 2)
        msg(argv[0]);

    img_name(argv[1], nameIn, nameOut, BW, GRAY);

    //-- read image
    In = img_get(nameIn, BW);
    //-- transformation
    rotulacao(In);
    //-- save image
    img_put(In, nameOut, GRAY);

    sprintf(cmd, "%s %s &", VIEW, nameOut);
    system(cmd);
    img_free(In);
    return 0;
}
