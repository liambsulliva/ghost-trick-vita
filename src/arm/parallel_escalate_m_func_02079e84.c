extern void func_0207a1b0();
extern void func_0207a118();
extern void func_0207a140();
extern char data_020d2fe0;
extern void func_020760c8(void);
extern void func_020761d0(int tag, void (*callback)());
extern int func_0207621c(int tag, int proc);
extern void func_02079b6c(void);
extern void func_02079df4(int arg);
extern void func_02079f34(void);
extern void func_0207a1c4(void *work);

void func_02079e84(void)
{
    char *base;

    base = (char *)0x020d2f04;
    if (*(int *)(base + 8) != 0) {
        return;
    }
    *(int *)(base + 8) = 1;
    func_02079b6c();
    *(int *)(base + 0xc) = 0;
    func_020760c8();
    do {
    } while (func_0207621c(0xd, 1) == 0);
    func_020761d0(0xd, func_0207a118);
    func_02079f34();
    func_020761d0(0xd, 0);
    func_020761d0(0xd, func_0207a140);
    *(int *)(base + 0x18) = 0;
    func_0207a1c4(&data_020d2fe0);
    func_020761d0(0x11, func_0207a1b0);
    func_02079df4(0);
}
