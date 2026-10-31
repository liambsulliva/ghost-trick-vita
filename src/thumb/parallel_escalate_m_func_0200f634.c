extern int data_020a2f28;
extern unsigned int func_02010194(char *object);
extern int func_0200f6f8(char *object);
extern void func_02058a80(int *a, unsigned int b);
extern void func_0200f70c(int *a);
extern void func_0205958c(int *a, int b, int c);
extern int func_020083a0(int a);
extern void func_02008378(int *a, int *b, int *c);
extern void func_020083b0(int *a, int *b);
extern int func_0200f710(int a);
extern void func_0201236c(char *a, int b);
extern void func_0200f61c(char *a, int b, int c, int d);
void func_0200f634(char *param_1, int param_2)
{
    int e[3];
    int d[3];
    int c[3];
    int b[3];
    int a[3];
    unsigned int r6;
    int total;
    int r7;
    int r4;
    int *obj;
    r6 = func_02010194(param_1);
    if (r6 == 0) {
        return;
    }
    if (func_0200f6f8(param_1) == 0) {
        return;
    }
    func_02058a80(e, r6);
    func_0200f70c(d);
    obj = *(int **)(param_1 + 0xe3c);
    if (obj != 0) {
        func_0205958c(a, (int)obj, 0);
        func_02008378(b, a, (int *)func_020083a0((int)obj));
        func_020083b0(d, b);
    }
    func_02008378(c, e, d);
    c[0] = c[0] >> 1;
    c[1] = c[1] >> 1;
    total = func_0200f710(c[0] - e[0]);
    if (total > 0x78000) {
        return;
    }
    r7 = c[0] - 0x80000;
    r6 = *(unsigned int *)(param_1 + 0xda0);
    r4 = param_2;
    if (param_2 < 0) {
        r6 = (c[1] - 0x60000) - (data_020a2f28 << 12);
        r4 = -param_2;
        func_0201236c(param_1 + 0xde4, 1);
    }
    func_0200f61c(param_1, r7, (int)r6, r4);
}
