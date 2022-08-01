/*-------------------------------------------------------------------------
 * Tranformação de intensidade
 * Por Luiz Eduardo da Silva.
 *-------------------------------------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "imagelib_l64.h"

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

int concat_binary(int a, u_int b){
    printf("%d", b);
    return (a << 6) | b;
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
    image_l64 Out;
    if (argc < 2)
        msg(argv[0]);

    img_name(argv[1], nameIn, nameOut);

    //-- read image
    printf("a");
    In = img_get(nameIn, code);
    In->code = code;
    //-- transformation
    printf("Codigo da imagem: %s\n", In->code);
    printf("binario: %d\n", concat_binary(0x000000, 0x100111));
    //-- save image
    img_put(In, nameOut);
    sprintf(cmd, "%s %s &", VIEW, nameOut);
    system(cmd);
    img_free(In);
    return 0;
}
