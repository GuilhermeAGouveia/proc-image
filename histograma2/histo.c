/*-------------------------------------------------------------------------
 * Tranformação de intensidade
 * Por Luiz Eduardo da Silva.
 *-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "imagelib.h"

image equaliza(image In)
{
    int nl = In->nr;
    int nc = In->nc;
    int mn = In->ml;
    int tp = In->tp;
    int *px = In->px;

    image Out = img_clone(In);

    int histo[mn + 1], sk[mn + 1];
    float pk[mn + 1], ak[mn + 1];

    for (int i = 0; i < mn + 1; i++)
    {
        histo[i] = 0;
    }
    
    for (int i = 0; i < nl * nc; i++)
    {
        histo[px[i]]++;
    }

    pk[0] = (float)histo[0] / (nl * nc);
    ak[0] = pk[0];
    sk[0] = ak[0] * mn;
    for (int i = 1; i < mn + 1; i++)
    {
        pk[i] = (float)histo[i] / (nl * nc);
        ak[i] = ak[i - 1] + pk[i];
        sk[i] = ak[i] * mn;
    }
    for (int i = 0; i < nc * nl; i++)
    {
        Out->px[i] = sk[px[i]];
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
 * main function
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
    Out = equaliza(In);
    //-- save image
    img_put(Out, nameOut, GRAY);

    sprintf(cmd, "%s %s &", VIEW, nameOut);
    system(cmd);
    img_free(In);
    img_free(Out);
    return 0;
}
