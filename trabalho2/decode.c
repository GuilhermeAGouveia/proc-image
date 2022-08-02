/*-------------------------------------------------------------------------
 * Tranformação de intensidade
 * Por Luiz Eduardo da Silva.
 *-------------------------------------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "imagelib_l64.h"
#include "imagelib.h"
#include "lzw.h"

#define DEBUG(x)

int convert_base64_to_int(char c)
{
    if (c >= 'A' && c <= 'Z')
        return c - 'A';
    if (c >= 'a' && c <= 'z')
        return c - 'a' + 26;
    if (c >= '0' && c <= '9')
        return c - '0' + 52;
    if (c == '+')
        return 62;
    if (c == '/')
        return 63;
    return -1;
}

int concat_binary(int a, int b){
    return a * 64 + b;
}

image decode(image_l64 img_code){
    char *code = img_code->code;
    char caracter1, caracter2;
    int int_decode;

    image image_decode = img_create(img_code->nr, img_code->nc, 255, GRAY);
    int *px = image_decode->px;

    int count = 0; // conta quantos inteiro foram decodificados a partir da string base64
    while (*code){
        caracter1 = *(code++);
        caracter2 = *(code++);

        int_decode = concat_binary(convert_base64_to_int(caracter1), convert_base64_to_int(caracter2));
        *px = int_decode;
        px++;
        count++;
    }

    int * out = malloc(img_code->nc * img_code->nr * sizeof(int));

    decodifica_lzw(image_decode->px, count, out);

    image_decode->px = out;
    return image_decode;
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
    char nameIn[100], nameOut[100], cmd[110], code[MAX_CODE_LENGTH];
    image_l64 In;
    image Out;
    if (argc < 2)
        msg(argv[0]);

    imgl64_name(argv[1], nameIn, nameOut);

    //-- read image
    In = imgl64_get(nameIn, code);
    In->code = code;

    //-- transformation
    Out = decode(In);
    //-- save image
    img_put(Out, nameOut, GRAY);
    sprintf(cmd, "%s %s &", VIEW, nameOut);
    system(cmd);
    imgl64_free(In);
    img_free(Out);
    return 0;
}
