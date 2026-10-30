extern unsigned short data_020d0e84[];

void func_02071e90(void)
{
    unsigned short *puVar1;
    unsigned int *puVar2;

    puVar1 = data_020d0e84;
    puVar1[0] = 0;
    puVar1[1] = 0;
    puVar1[2] = 0;
    puVar1[3] = 0;
    puVar1[4] = 0;
    puVar1[5] = 0;
    puVar1[6] = 0;
    puVar1[7] = 0;
    puVar1[8] = 0;
    puVar1[9] = 0;
    puVar1[10] = 0;
    puVar1[11] = 0;
    puVar2 = (unsigned int *)0x04000240;
    puVar1[12] = 0;
    *puVar2 = 0;
    *(unsigned char *)(puVar2 + 1) = 0;
    *(unsigned char *)((int)puVar2 + 5) = 0;
    *(unsigned char *)((int)puVar2 + 6) = 0;
    *(unsigned short *)(puVar2 + 2) = 0;
}
