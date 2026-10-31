extern void func_0200f70c(int *vec);
extern void func_02058a80(int *vec, int object);
extern void func_020083b0(int *dst, int *src);
extern int func_02036f3c(int object);
extern void *func_02008444(void);
extern int func_02010194(void *object);
extern void func_020083c0(int *dst, int *a, int *b);
extern int func_020452e8(int *vec);

void func_020612b8(char *param_1)
{
    int v20[3];
    int v2c[3];
    int v38[3];
    int v44[3];
    unsigned short i;
    int off;
    char *entry;
    int object;

    *(int *)(param_1 + 0xf4) = 0;
    func_0200f70c(v20);
    if (*(int *)(param_1 + 0xfc) != 0) {
        func_02058a80(v44, *(int *)(param_1 + 0xfc));
        func_020083b0(v20, v44);
    }
    for (i = 0; i < *(unsigned short *)(param_1 + 0xec); i++) {
        off = i * 0x18;
        if (func_02036f3c(*(int *)(*(char **)(param_1 + 0xf0) + off + 4)) == 0) {
            entry = *(char **)(param_1 + 0xf0) + off;
            object = *(int *)(entry + 4);
            if (object == func_02010194(func_02008444())) {
                *(int *)(param_1 + 0xf4) = *(int *)(entry + 0x14);
            } else {
                func_02058a80(v2c, object);
                v2c[2] = 0;
                func_020083b0((int *)(*(char **)(param_1 + 0xf0) + off + 8), v2c);
                func_020083c0(v38, (int *)(*(char **)(param_1 + 0xd0) + (*(unsigned char *)(param_1 + 0x7f) - 1) * 0xc), (int *)(*(char **)(param_1 + 0xf0) + off + 8));
                *(int *)(*(char **)(param_1 + 0xf0) + off) = func_020452e8(v38);
            }
        }
    }
}
