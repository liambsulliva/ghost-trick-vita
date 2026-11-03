struct Code {
    unsigned short kind : 2;
    unsigned short index : 14;
};

struct Entry {
    unsigned short f0;
    unsigned short f2;
    unsigned short f4;
    unsigned short f6;
};

struct Loader {
    int a;
    int b;
    int c;
    char pad[0x400];
    unsigned short h40c;
    unsigned short h40e;
};

extern const struct Code data_02088910[];
extern const struct Entry data_0208c064[];
extern int func_02029ab4(char *object);
extern int func_020283ec(char *object);
extern int func_020086c8(char *object, unsigned short id);
extern int func_0200883c(void);
extern int func_02008824(int object);
extern int func_0201e888(int object, int a, unsigned short b, int c, int d);

int func_02029a20(struct Loader *p, char *s)
{
    int idx;
    const struct Entry *e;

    idx = ((const struct Code *)((int)data_02088910 + func_02029ab4(s) * 2))->index;
    e = &data_0208c064[idx];
    p->h40c = 0;
    p->h40e = func_020283ec(s);
    p->b = func_020086c8(s, p->h40e);
    p->a = func_0201e888(func_02008824(func_0200883c()), 0, e->f0, 0, 0);
    p->c = func_0201e888(func_02008824(func_0200883c()), 3, e->f4, 0, 0);
    if (p->a != 0 && p->b != 0 && p->c != 0) {
        return 1;
    }
    return 0;
}
