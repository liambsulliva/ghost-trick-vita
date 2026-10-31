extern void *func_0200d8a8();
extern void *func_0200d7e4();
extern void func_020514b8();

int func_0205145c(unsigned char *param_1, int param_2, int param_3, int param_4)
{
    unsigned char *puVar2;
    unsigned int uVar3;

    if (param_1[256] != 0) {
        return 0;
    }
    puVar2 = func_0200d8a8(16, param_2, param_3, param_4);
    if (puVar2 != 0) {
        puVar2 = func_0200d7e4(puVar2);
    }
    if (puVar2 == 0) {
        return 0;
    }
    func_020514b8(puVar2, param_2);
    uVar3 = *(unsigned int *)(param_1 + 0xf8);
    if (uVar3 < 16) {
        *(unsigned int *)(param_1 + 0xf8) = *(unsigned int *)(param_1 + 0xf8) + 1;
        *(unsigned char **)(param_1 + uVar3 * 4 + 0xb8) = puVar2;
        return 1;
    }
    return 0;
}
