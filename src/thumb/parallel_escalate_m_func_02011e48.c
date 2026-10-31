typedef void code();
extern int func_02011f30(void *param_1);
extern int func_02008828(void *param_1, void *param_2);
extern void func_02011dfc(void *param_1, int param_2);
extern void func_02011e00(void *param_1, int param_2);
extern void func_02011e04(void *param_1, int param_2);
extern void *func_02031254(unsigned int param_1);
extern void *func_02011e08(void *param_1);
extern void func_02011da4();
extern char data_020ca150[];

void func_02011e48(unsigned char *param_1, int param_2, int param_3, int param_4, unsigned char param_5)
{
    unsigned char *puVar1;
    unsigned char *puVar2;
    unsigned char *puVar3;
    unsigned int uVar4;
    int iVar5;

    iVar5 = 0xf21;
    if (param_5 >= param_1[iVar5]) {
        return;
    }
    uVar4 = (unsigned int)param_5 * 4;
    puVar1 = param_1 + (iVar5 + 0x83);
    puVar2 = *(unsigned char **)(puVar1 + uVar4);
    if (puVar2 != 0) {
        if (func_02011f30(puVar2) != 0) {
            return;
        }
        puVar1 = param_1 + (iVar5 + 3);
        if (*(unsigned char **)(puVar1 + uVar4) != 0) {
            if (func_02008828(*(unsigned char **)(puVar1 + uVar4), data_020ca150) != 0) {
                puVar2 = *(unsigned char **)(puVar1 + uVar4);
                func_02011dfc(puVar2, param_3);
                func_02011e00(puVar2, param_2);
                func_02011e04(puVar2, param_4);
                return;
            }
            if (*(unsigned char **)(puVar1 + uVar4) != 0) {
                puVar2 = *(unsigned char **)(puVar1 + uVar4);
                if (puVar2 != 0) {
                    (**(code **)(*(int *)puVar2 + 4))(puVar2);
                }
                *(unsigned char **)(puVar1 + uVar4) = 0;
            }
            puVar3 = func_02031254(0x14);
            if (puVar3 != 0) {
                puVar3 = func_02011e08(puVar3);
            }
            if (puVar3 != 0) {
                func_02011dfc(puVar3, param_3);
                func_02011e00(puVar3, param_2);
                func_02011e04(puVar3, param_4);
                *(unsigned char **)(puVar1 + uVar4) = puVar3;
            }
            return;
        }
        func_02011da4(param_1, param_2, param_3, param_4);
        return;
    }
    puVar3 = func_02031254(0x14);
    if (puVar3 != 0) {
        puVar3 = func_02011e08(puVar3);
    }
    if (puVar3 != 0) {
        func_02011dfc(puVar3, param_3);
        func_02011e00(puVar3, param_2);
        func_02011e04(puVar3, param_4);
        *(unsigned char **)(puVar1 + uVar4) = puVar3;
    }
}
