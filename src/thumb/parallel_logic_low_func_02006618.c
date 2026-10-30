extern char data_020b18a0;
extern int func_02007a8c();

void func_02006618(void)
{
    unsigned int *puVar1;
    unsigned int uVar2;

    puVar1 = (unsigned int *)(*(int *)(((int)&data_020b18a0) + 0x10) + 0x4c50);
    if (((*puVar1 & 2) != 0) && ((*puVar1 & 1) != 0)) {
        if ((int)puVar1[0xd] >= 0) {
            uVar2 = 1 << puVar1[0xd];
        }
        else {
            uVar2 = 0;
        }
        func_02007a8c(puVar1[0xb], puVar1[0xc], uVar2, 0);
    }
    return;
}
