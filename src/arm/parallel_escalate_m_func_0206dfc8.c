extern char data_0209e3b8;
unsigned int func_0206df74(int, unsigned short *, int, unsigned int);
unsigned int func_0206df9c(int, unsigned short *, int, unsigned int);
unsigned int func_0206c0f0(char *, char *, char *);

unsigned int func_0206dfc8(char *obj, int a, int b, int c, int d, int e, void *f, void *g)
{
    *(int *)(obj + 0x38) = e;
    *(int *)(obj + 0x28) = a;
    *(int *)(obj + 0x30) = c;
    *(int *)(obj + 0x2c) = b;
    *(int *)(obj + 0x3c) = b;
    *(int *)(obj + 0x34) = d;
    *(int *)(obj + 0x40) = d;
    *(void **)(obj + 0x48) = f != 0 ? f : (void *)func_0206df74;
    *(void **)(obj + 0x4c) = g != 0 ? g : (void *)func_0206df9c;
    *(int *)(obj + 0x44) = 0;
    return func_0206c0f0(obj, obj + 0x28, &data_0209e3b8);
}
