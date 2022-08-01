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

enum
{
    BW = 1,
    GRAY,
    COLOR
};

#if defined(_WIN32) || defined(__WIN64__) || defined(__CYGWIN__)
#define VIEW "./i_view32"
#elif defined(__linux__)
#define VIEW "eog"
#endif

void errormsg(char *, ...);
image_l64 img_create(int, int);
image_l64 img_clone(image_l64);
int img_free(image_l64);
void img_name(char *, char *, char *);
image_l64 img_get(char *, char *);
void img_put(image_l64, char *);
