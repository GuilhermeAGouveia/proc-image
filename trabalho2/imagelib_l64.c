/*-------------------------------------------------
  |         Unifal - Universidade Federal de Alfenas.
  |             BACHARELADO EM CIENCIA DA COMPUTACAO.
  | Trabalho..: Descompactador do formato LZW-Base64
  | Disciplina: Processamento de Imagens
  | Professor.: Luiz Eduardo da Silva
  | Aluno.....: Guilherme Augusto Gouveia
  | Data......: 02/08/2022
  -------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "imagelib_l64.h"
#define PER_LINE 82

/*-------------------------------------------------------------------------
 * image_l64 allocation and free routines
 *   nr = number of rows
 *   nc = number of columns
 *   ml = max gray level
 *   tp = type of image_l64
 *-------------------------------------------------------------------------*/
image_l64 imgl64_create(int nr, int nc)
{
    image_l64 img = malloc(sizeof(image_l64 *));
    // img->code = malloc(MAX_CODE_LENGTH * sizeof(char));
    img->nr = nr;
    img->nc = nc;
    return img;
}

image_l64 imgl64_clone(image_l64 In)
{
    return imgl64_create(In->nr, In->nc);
}

int imgl64_free(image_l64 Im)
{
    free(Im);
}

/*-------------------------------------------------------------------------
 * Define input and output image_l64 name
 * Params:
 *   name = image_l64 name file
 *   in = input image_l64 name
 *   out = output image_l64 name
 *   tpIn, tpOut = image_l64 type (BW, GRAY, COLOR)
 *-------------------------------------------------------------------------*/
void imgl64_name(char *name, char *in, char *out)
{
    char *p = strstr(name, ".l64");
    if (p)
        *p = 0;
    sprintf(in, "%s%s", name, ".l64");
    sprintf(out, "%s-result%s", name, ".pgm");
}

/*-------------------------------------------------------------------------
 * Display image_l64 information
 *   name = file name image_l64
 *   nr = number of rows
 *   nc = number os columns
 *   ml = max grayscale level
 *   tp = image_l64 type (1, 2 ou 3)
 *-------------------------------------------------------------------------*/
void imgl64_info(char *name, image_l64 img)
{
    printf("\nimage_l64 Informations:");
    printf("\n--------------------------\n");
    printf("image_l64 file name.............: %s \n", name);
    printf("Number of rows..............: %d \n", img->nr);
    printf("Number of columns...........: %d \n", img->nc);
}

/*-------------------------------------------------------------------------
 * Error message
 *   str - string message control
 *   ... - parameters
 *-------------------------------------------------------------------------*/
void errormsg_l64(char *str, ...)
{
    char format[255];
    va_list arg;
    va_start(arg, str);
    sprintf(format, "ERROR: %s\n\n", str);
    vprintf(format, arg);
    va_end(arg);
    exit(1);
}

void concat_string(char *str1, char *str2)
{
    int i, j;
    for (; *str1; str1++)
        ;
    for (; *str2; str2++)
    {
        *str1 = *str2;
        str1++;
    }
    *str1 = 0;
    puts("aqui");
}

void remove_newline(char *str)
{
    for (; *str; str++)
    {
        if (*str == '\n')
            *str = '\0';
    }
}

/*-------------------------------------------------------------------------
 * Read pnm ascii image_l64
 * Params (in):
 *   name = image_l64 file name
 *   tp = image_l64 type (BW, GRAY or COLOR)
 * Returns:
 *   image_l64 structure
 *-------------------------------------------------------------------------+*/
image_l64 imgl64_get(char *name)
{
    ("aqui1");
    char lines[100];
    int nr, nc;
    image_l64 img;
    FILE *fimg;
    ERROR((fimg = fopen(name, "r")) == NULL, errormsg_l64("File open error: <%s>", name));
    /*--- PNM = "P1" or "P2" or "P3" ---*/
    fgets(lines, 9, fimg);
    ERROR(strcmp("lzw-b64\n", lines), errormsg_l64("File type image_l64 error: <%s>", name));
    /*--- Comment lines ---*/
    fgets(lines, 80, fimg);
    while (strchr(lines, '#'))
        fgets(lines, 80, fimg);
    sscanf(lines, "%d %d", &nc, &nr);
    ERROR(nc == 0 || nr == 0, errormsg_l64("image_l64 dimensions error: <%s>", name));
    img = imgl64_create(nr, nc);
    img->code = malloc(MAX_CODE_LENGTH * sizeof(char));

    ERROR(!img, errormsg_l64("image_l64 allocation error: %s\n\n imgl64_get routine", name));
    int count = 0;

    while (1)
    {
        if (!fgets(lines, 100, fimg) || ferror(fimg) || feof(fimg))
        {
            break;
        }
        remove_newline(lines);
        strcat(img->code, lines);
    }
    
    fclose(fimg);

    imgl64_info(name, img);
    return img;
}

/*-------------------------------------------------------------------------
 * Write pnm image_l64
 * Params:
 *   img = image_l64 structure
 *   name = image_l64 file name
 *   tp = image_l64 type (BW, GRAY or COLOR)
 *-------------------------------------------------------------------------*/
void imgl64_put(image_l64 img, char *name)
{
    int count;
    FILE *fimg;
    ERROR((fimg = fopen(name, "wt")) == NULL, errormsg_l64("image_l64 creation error: <%s>", name));
    fprintf(fimg, "lzw-b64\n");
    fputs(CREATOR, fimg);
    fprintf(fimg, "%d  %d\n", img->nc, img->nr);
    count = 0;
    for (char *p = img->code; *p; p++)
    {
        if (count == PER_LINE)
        {
            fprintf(fimg, "\n");
            count = 0;
        }
        fprintf(fimg, "%c", *p);
        count++;
    }
    fprintf(fimg, "\n\n");
    fclose(fimg);
}
