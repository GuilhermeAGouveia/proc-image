/*-------------------------------------------------------------------------
 * Tranformação de intensidade
 * Por Luiz Eduardo da Silva.
 *-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "imagelib.h"

image fourier(image In)
{
    int M = In->nr;
    int N = In->nc;
    int mn = In->ml;
    int tp = In->tp;
    int *px = In->px;

    image Out = img_clone(In);

    image_double ReImage = img_create_double(M, N, mn, GRAY);
    image_double ImImage = img_create_double(M, N, mn, GRAY);
    image FImage = img_create(M, N, mn, GRAY);

    double *Re = ReImage->px;
    double *Im = ImImage->px;
    int *F = FImage->px;
    double theta;

    long double count = 0;
    for (int u = 0; u < M; u++)
    {
        for (int v = 0; v < N; v++)
        {
            Re[u * N + v] = Im[u * N + v] = 0.0;
            for (int x = 0; x < M; x++)
            {
                for (int y = 0; y < N; y++)
                {
                    theta = 2 * M_PI * ((double) u * x / M + (double) v * y / N);
                    Re[u * N + v] += 1.0 / (M * N) * (double) px[x * N + y] * cos(theta);
                    Im[u * N + v] += 1.0 / (M * N) * (double) px[x * N + y] * sin(theta);
                    count++;
                }
            }
            F[u * N + v] = sqrt(pow(Re[u * N + v], 2) + pow(Im[u * N + v], 2));
        }
        printf("iteração: %Lf\n", count);
    }

    for (int i = 0; i < M * N; i++) {
        Out->px[i] = mn * (log(F[i] + 1) / log(mn + 1));
    }

    return Out;
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
 * main fuNtion
 *-------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    char nameIn[100], nameOut[100], cmd[110];
    image In, Out;
    if (argc < 2)
        msg(argv[0]);

    img_name(argv[1], nameIn, nameOut, GRAY, GRAY);

    //-- read image
    In = img_get(nameIn, GRAY);
    //-- transformation
    Out = fourier(In);
    //-- save image
    img_put(Out, nameOut, GRAY);

    sprintf(cmd, "%s %s &", VIEW, nameOut);
    system(cmd);
    img_free(In);
    img_free(Out);
    return 0;
}
