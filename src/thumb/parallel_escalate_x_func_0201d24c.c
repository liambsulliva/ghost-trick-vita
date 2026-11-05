
typedef struct Two0201d24c {
    unsigned char a : 2;
    unsigned char b : 2;
    unsigned char c : 2;
    unsigned char d : 2;
} Two0201d24c;

typedef struct Four0201d24c {
    unsigned char lo : 4;
    unsigned char hi : 4;
} Four0201d24c;

typedef struct Grid0201d24c {
    signed char base;
    char pad1[3];
    int mode;
    Two0201d24c two[8];
    Four0201d24c four[16];
} Grid0201d24c;

int func_0201d24c(int result, signed char index)
{
    Grid0201d24c *grid = (Grid0201d24c *)result;
    int d;
    unsigned char pos;
    unsigned char sub;

    result = 0;
    d = index - grid->base;
    if (d < 0 || d >= 32) {
        return 0;
    }
    if (grid->mode == 1) {
        pos = d / 4;
        sub = d % 4;
        switch (sub) {
        case 0:
            result = grid->two[pos].a;
            break;
        case 1:
            result = grid->two[pos].b;
            break;
        case 2:
            result = grid->two[pos].c;
            break;
        case 3:
            result = grid->two[pos].d;
            break;
        }
    } else if (grid->mode == 2) {
        pos = d / 2;
        sub = d % 2;
        switch (sub) {
        case 0:
            result = grid->four[pos].lo;
            break;
        case 1:
            result = grid->four[pos].hi;
            break;
        }
    }
    return result;
}
