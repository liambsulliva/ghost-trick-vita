typedef int code();
extern int func_02031254();
extern int func_0200b460();
extern int func_0200b518();
extern int func_0200b598();
extern int func_020280a8();
extern int func_0200ba7c();

void func_02041518(int param_1, unsigned int param_2, unsigned int param_3)
{
    int *puVar2;
    int iVar3;

    if (*(unsigned char *)(param_1 + 0xbc) < 0xb) {
        puVar2 = (int *)func_02031254(0x88);
        if (puVar2 != (int *)0x0) {
            puVar2 = (int *)func_0200b460(puVar2);
        }
        *(int *)(param_1 + (unsigned int)*(unsigned char *)(param_1 + 0xbc) * 4 + 0xc0) = (int)puVar2;
        iVar3 = *(int *)(param_1 + (unsigned int)*(unsigned char *)(param_1 + 0xbc) * 4 + 0xc0);
        if (iVar3 != 0) {
            func_0200b518(iVar3, 1, 0x80000000, 0x80000000);
            func_0200b598(*(int *)(param_1 + (unsigned int)*(unsigned char *)(param_1 + 0xbc) * 4 + 0xc0), param_2, 0xf);
            func_020280a8(*(int *)(param_1 + (unsigned int)*(unsigned char *)(param_1 + 0xbc) * 4 + 0xc0), param_3);
            func_0200ba7c(*(int *)(param_1 + (unsigned int)*(unsigned char *)(param_1 + 0xbc) * 4 + 0xc0), param_1 + 0xb0);
            *(unsigned char *)(param_1 + 0xbc) = *(unsigned char *)(param_1 + 0xbc) + 1;
        }
    }
}
