extern void func_0200a424(char *object);
extern void func_020098c0(unsigned int object);
extern void func_02031270(unsigned int object);
extern void func_0202f64c(char *object);
extern unsigned int func_02031254(unsigned int param_1);
extern unsigned int func_02009858(unsigned int object);
void func_0200ec24(char *param_1, unsigned int param_2)
{
    unsigned int p;
    *(unsigned int *)(param_1 + 0xd94) = 0;
    *(unsigned int *)(param_1 + 0xd98) = 0;
    *(unsigned short *)(param_1 + 0xdc8) = 0;
    *(unsigned short *)(param_1 + 0xdca) = 0;
    *(unsigned int *)(param_1 + 0xdcc) = 0;
    *(unsigned int *)(param_1 + 0xdd0) = 0;
    *(unsigned int *)(param_1 + 8) &= 0xffffff7f;
    *(unsigned int *)(param_1 + 0xc) &= 0xffffefff;
    p = *(unsigned int *)(param_1 + 0xee0);
    if (p != 0) {
        func_0200a424((char *)p);
        p = *(unsigned int *)(param_1 + 0xee0);
        if (p != 0) {
            func_020098c0(p);
            func_02031270(p);
        }
        *(unsigned int *)(param_1 + 0xee0) = 0;
        func_0202f64c(param_1 + 0xee8);
    }
    if (param_2 == 0) {
        p = func_02031254(0xaa48);
        if (p != 0) {
            p = func_02009858(p);
        }
        *(unsigned int *)(param_1 + 0xee0) = p;
    }
}
