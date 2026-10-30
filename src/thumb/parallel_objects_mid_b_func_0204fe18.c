extern char data_0209be84;
extern char *func_02010b7c();
extern unsigned int func_02015188();
extern int func_0202cb64();
extern int func_0204faf4();
extern int func_02010b60();
extern int func_02010b64();
extern void func_020229b4();

void func_0204fe18(int param_1)
{
    int iVar6;
    int i;
    char *slot;
    char *puVar1;
    int iVar4;
    int iVar5;
    iVar6 = -1;
    puVar1 = func_02010b7c();
    if (func_02015188(puVar1) == 0) {
        return;
    }
    puVar1 = func_02010b7c();
    if (func_0202cb64(puVar1) != 0) {
        return;
    }
    for (i = 0; i < 0xc; i++) {
        slot = (char *)param_1 + i * 4;
        if (*(int *)(slot + 0x48) != 0 && func_0204faf4(*(int *)(slot + 0x48)) == 1) {
            if (i == 3 || i == 1) {
                puVar1 = func_02010b7c();
                iVar4 = func_02010b60(puVar1);
                puVar1 = func_02010b7c();
                iVar5 = func_02010b64(puVar1);
                if ((*(int (**)())(*(int *)(*(int *)(slot + 0x48)) + 0xc8))(*(int *)(slot + 0x48), iVar4, iVar5) != 0) {
                    iVar6 = i;
                    break;
                }
            } else {
                puVar1 = func_02010b7c();
                iVar4 = func_02010b60(puVar1);
                puVar1 = func_02010b7c();
                iVar5 = func_02010b64(puVar1);
                if ((*(int (**)())(*(int *)(*(int *)(slot + 0x48)) + 0xbc))(*(int *)(slot + 0x48), iVar4, iVar5) != 0) {
                    iVar6 = i;
                    break;
                }
            }
        }
    }
    if (iVar6 >= 0) {
        func_020229b4(((int *)&data_0209be84)[iVar6]);
    }
}
