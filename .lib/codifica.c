#include <stdio.h>
#include <string.h>

typedef unsigned short int ui16;

#define TAM_DICT 4096
#define NUM_SIMB 256

void codifica(int *in, int n){
    ui16 dicionario[TAM_DICT][NUM_SIMB];
    int posDict;
    int simbolo;
    int corrent = in[0];

    memset(dicionario, 0, TAM_DICT * NUM_SIMB * sizeof(ui16));
    posDict = NUM_SIMB;
    int i = 1;

    while (i < n)
    {
        simbolo = in[i++];
        ui16 prox = dicionario[corrent][simbolo];
        if (prox != 0)
        {
           corrent = prox;
        } else {
            printf("[%d]", corrent);
            if (posDict < TAM_DICT)
                dicionario[corrent][simbolo] = posDict++;
            else {
                memset(dicionario, 0, TAM_DICT * NUM_SIMB * sizeof(ui16));
                posDict = NUM_SIMB;
            }

            corrent = simbolo;
        }
    };
    printf("[%d]", corrent);
}

int main(int argc, char const *argv[])
{
    int in[] = {39, 39, 126, 126, 39, 39, 126, 126, 39, 39, 126, 126, 39, 39, 126, 126};;
    codifica(in, 16);
    return 0;
}
