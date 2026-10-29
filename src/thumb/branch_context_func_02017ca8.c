extern unsigned int * func_0201003c();
extern int func_02015a88();

unsigned int func_02017ca8(int param_1)

{
    unsigned int *puVar1;
    unsigned int uVar2;

    puVar1 = func_0201003c();
    uVar2 = func_02015a88((unsigned int)puVar1,(unsigned int)*(unsigned short *)(param_1 + 0x8a));
    if ((uVar2 != 0) && (*(char *)(param_1 + 0x8c) == '\0')) {
        return 1;
    }
    return 0;
}
