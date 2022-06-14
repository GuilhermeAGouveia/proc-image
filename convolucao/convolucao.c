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

void operacao(image In, image Out, int nl, int nc, int mn)
{
    int w[3][3] = {
        {-1, -1, -1},
        {0, 0, 0},
        {1, 1, 1}};

    for (int i = 1; i < nl - 1; i++)
        for (int j = 1; j < nc - 1; j++)
        {
            int sum = 0;

            for (int a = -1; a <= 1; a++)
            {
                for (int b = -1; b <= 1; b++)
                {
                    sum += In[(i + a) * nc + (j + b)] * w[a + 1][b + 1];
                }
            }

            Out[i * nc + j] = (sum > 0 ? (sum > 255 ? 255 : sum) : 0);
        }
}

void sobel(image In, image Out, int nl, int nc, int mn)
{
    int w1[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}};

    int w2[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}};

    int max = -1, min = 255;

    for (int i = 1; i < nl - 1; i++)
        for (int j = 1; j < nc - 1; j++)
        {
            int Z[9];
            int sum = 0;

            for (int a = -1; a <= 1; a++)
            {
                for (int b = -1; b <= 1; b++)
                {
                    // Z[(a + 1) * 3 + b + 1] = In[(i + a) * nc + (j + b)];
                    sum = abs((In[(i + a) * nc + (j + b)]) * w1[a + 1][b + 1]) + abs((In[(i + a) * nc + (j + b)]) * w2[a + 1][b + 1]);
                }
            }

            // sum = abs(Z[6] + 2 * Z[7] + Z[8] - (Z[0] + 2 * Z[1] + Z[2])) + abs(Z[2] + 2 * Z[5] + Z[8] - (Z[0] + 2 * Z[3] + Z[6]));

            if (sum > max)

                max = sum;
            Out[i * nc + j] = sum;
        }

    for (int i = 0; i < nc * nl; i++)
    {
        Out[i] = ((float)Out[i] / max * mn);
    }
}

void msg(char *s)
{
    printf("\nConvolucao image");
    printf("\n-------------------------------");
    printf("\nUsage:  %s  image-name[.pgm]\n\n", s);
    printf("    image-name[.pnm] is image file file \n");
    exit(1);
}

/*-------------------------------------------------------------------------
 * main function
 *-------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    int nc, nr, ml, tp;
    char *p, nameIn[100], nameOut[100], cmd[110];
    image In, Out;
    if (argc < 2)
        msg(argv[0]);
    //-- define input/output file name
    img_name(argv[1], nameIn, nameOut, GRAY);
    //-- read image
    In = img_get(nameIn, &nr, &nc, &ml, GRAY);
    //-- create output image
    Out = img_alloc(nr, nc);
    //-- transformation
    sobel(In, Out, nr, nc, ml);

    //-- save image
    img_put(Out, nameOut, nr, nc, ml, GRAY);
    //-- show image
    sprintf(cmd, "%s %s &", VIEW, nameOut);
    system(cmd);
    img_free(In);
    img_free(Out);
    return 0;
}
