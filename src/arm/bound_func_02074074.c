extern char data_020d0f0c;
extern int func_02073b9c();
extern int func_02073c7c();
extern int func_02074120();
extern int func_020741dc();
extern int func_02074398();
extern int func_020743c8();
extern int func_020745e8();
extern int func_02075f84();

void func_02074074(void)

{
    int iVar1;

    iVar1 = **(int **)(((int)&data_020d0f0c) + 8);
    func_02074398();
    func_020745e8(iVar1);
    if (*(int *)(iVar1 + 0x78) != 0) {
        func_02073b9c(*(int *)(iVar1 + 0x78),iVar1);
    }
    func_02073c7c(iVar1);
    *(unsigned int *)(iVar1 + 100) = 2;
    func_02074120(iVar1 + 0x9c);
    func_020743c8();
    func_020741dc();
    func_02075f84();
    return;
}
