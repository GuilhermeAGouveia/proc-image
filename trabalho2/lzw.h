typedef unsigned short int ui16;
#define TAM_DICT 4096
#define NUM_SIMB 256

int * decodifica_lzw (int *in, int n, int *out);
void codifica_lzw(int *in, int n);