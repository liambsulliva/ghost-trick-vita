extern unsigned int *func_02008444(void);
extern void func_02011454(int param_1);
extern void *func_02031b44(char *object);
extern void func_020506f4();

void func_02040418(int param_1)
{
    void *puVar1;

    func_02011454((int)func_02008444());
    puVar1 = func_02031b44((char *)func_02008444());
    *(void **)(param_1 + 0x24) = puVar1;
    func_020506f4(8, 1, puVar1);
    return;
}
