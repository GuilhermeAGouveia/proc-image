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

void histo(image In, image Out, int nl, int nc, int mn)
{
    int histo[mn + 1];
    int sk[mn + 1];
    float pk[mn + 1], ak[mn + 1];

    for (int i = 0; i < mn + 1; i++)
    {
        histo[i] = 0;
    }

    for (int i = 0; i < nl * nc; i++)
    {
        histo[In[i]]++;
    }

    for (int i = 0; i < mn + 1; i++)
    {
        pk[i] = (float)histo[i] / (nl * nc);
    }

    ak[0] = pk[0];
    for (int i = 1; i < mn + 1; i++)
    {
        ak[i] = ak[i - 1] + pk[i];
    }

    for (int i = 0; i < mn + 1; i++)
    {
        sk[i] = ak[i] * mn;
    }

    for (int i = 0; i < nc * nl; i++)
    {
        Out[i] = sk[In[i]];
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
    histo(In, Out, nr, nc, ml);

    //-- save image
    img_put(Out, nameOut, nr, nc, ml, GRAY);
    //-- show image
    sprintf(cmd, "%s %s &", VIEW, nameOut);
    system(cmd);
    img_free(In);
    img_free(Out);
    return 0;
}
