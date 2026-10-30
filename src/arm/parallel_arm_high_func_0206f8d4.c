extern void func_0206f9fc();

void func_0206f8d4(void)
{
    volatile unsigned int *puVar1;
    volatile unsigned short *puVar2;
    unsigned int *puVar3;

    puVar1 = (volatile unsigned int *)0x04000600;
    puVar2 = (volatile unsigned short *)0x04000060;
    do {
    } while ((*puVar1 & 0x8000000) != 0);
    *puVar1 = *puVar1 | 0x8000;
    *puVar2 = *puVar2 | 0x2000;
    *puVar2 = *puVar2 | 0x1000;
    func_0206f9fc();
    puVar3 = (unsigned int *)0x040004a4;
    *puVar3 = 0x001f0080;
    puVar3[1] = 0;
    puVar3[2] = 0;
}
