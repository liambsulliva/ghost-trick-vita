typedef struct {
    short slot[16];
    unsigned int count;
} Row0207727c;

typedef struct {
    unsigned int f0;
    unsigned int f4;
    unsigned short f8;
    unsigned short fa;
    char pad0c[0x14];
    Row0207727c rows[16];
    short extra[16];
} Obj0207727c;

extern void func_0207487c(void *start, unsigned int size);

void func_0207727c(Obj0207727c *param_1)
{
    int i;
    int j;

    param_1->f4 = 0;
    param_1->f8 = 0;
    param_1->fa = 0;
    param_1->f0 = 0;
    for (i = 0; i < 16; i++) {
        param_1->rows[i].count = 0;
        for (j = 0; j < 16; j++) {
            param_1->rows[i].slot[j] = -1;
        }
    }
    for (i = 0; i < 16; i++) {
        param_1->extra[i] = -1;
    }
    func_0207487c(param_1, 0x280);
}
