#define CREATOR "# CREATOR: Image Processing using C-Ansi - ByDu\n"
#define MAX_CODE_LENGTH 1000000

typedef struct
{
    char *code;        // codigo da imagem
    int nr, nc; // nr = n.rows, nc = n.columns, ml = max level
} *image_l64;

#define ERROR(x, y) \
    if (x)          \
    y


#if defined(_WIN32) || defined(__WIN64__) || defined(__CYGWIN__)
#define VIEW "./i_view32"
#elif defined(__linux__)
#define VIEW "eog"
#endif

void errormsg_l64(char *, ...);
image_l64 imgl64_create(int, int);
image_l64 imgl64_clone(image_l64);
int imgl64_free(image_l64);
void imgl64_name(char *, char *, char *);
image_l64 imgl64_get(char *, char *);
void imgl64_put(image_l64, char *);
