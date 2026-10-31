extern "C" {
extern unsigned char data_020c9d50;
extern int func_02056164(int);
extern unsigned int func_0200847c(void);
extern void func_0202a16c(unsigned int, int);
extern void func_0202a0c0(unsigned int, int);
extern void func_0203bfb4(int, int);

void func_0205627c(unsigned char *p)
{
    int obj;

    obj = *(int *)(p + 0x34);
    if (obj == 0) return;
    if (func_02056164(obj) == 0) return;
    if (*(int *)(p + 0x28) != 0) return;
    obj = *(int *)(p + 0x34);
    if ((*(int (**)(int))(*(int *)obj + 0xd4))(obj) != 0) goto L_2ba;
    if (p[0x30] != 0) goto L_2ea;
    if ((*(unsigned int *)((int)&data_020c9d50 + 4) & 0x1000) == 0) goto L_2ea;
L_2ba:
    func_0202a16c(func_0200847c(), 0);
    func_0202a0c0(func_0200847c(), 2);
    *(int *)(p + 0x28) = 1;
    func_0203bfb4(*(int *)(p + 0x34), 2);
    if (*(int *)(p + 0x38) != 0) func_0203bfb4(*(int *)(p + 0x38), 0);
    return;
L_2ea:
    obj = *(int *)(p + 0x38);
    if (obj == 0) return;
    if ((*(int (**)(int))(*(int *)obj + 0xd4))(obj) != 0) goto L_312;
    if (p[0x30] != 1) goto L_33c;
    if ((*(unsigned int *)((int)&data_020c9d50 + 4) & 0x1000) == 0) goto L_33c;
L_312:
    func_0202a16c(func_0200847c(), 0);
    func_0202a0c0(func_0200847c(), 2);
    *(int *)(p + 0x28) = 2;
    func_0203bfb4(*(int *)(p + 0x38), 2);
    func_0203bfb4(*(int *)(p + 0x34), 0);
    return;
L_33c:
    if ((*(unsigned int *)((int)&data_020c9d50 + 0xc) & 0x300) == 0) return;
    func_0202a0c0(func_0200847c(), 1);
    p[0x30] = 1 - p[0x30];
    if (p[0x30] == 0) {
        func_0203bfb4(*(int *)(p + 0x38), 0);
        func_0203bfb4(*(int *)(p + 0x34), 1);
    } else {
        func_0203bfb4(*(int *)(p + 0x38), 1);
        func_0203bfb4(*(int *)(p + 0x34), 0);
    }
}
}
