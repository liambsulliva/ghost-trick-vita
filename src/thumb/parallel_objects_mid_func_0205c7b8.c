extern int func_02010b54();
extern int func_02029030();
extern int func_02029640();
extern int func_02029708();
extern unsigned char * func_0202970c();
extern int func_0205c830();
extern int func_0205c838();

void func_0205c7b8(unsigned char *param_1)

{
    unsigned char *puVar2;
    int iVar3;
    int iVar4;
    unsigned int uVar5;

    puVar2 = func_0202970c();
    iVar3 = func_02029708(puVar2);
    iVar4 = func_02029640(param_1);
    if (iVar4 == 0 || ((func_02010b54(iVar4) != 0 || func_0205c830(iVar4) != 0) && func_0205c838(iVar4) != 0)) {
        if (*(int *)(param_1 + 0x54) != 0) {
            for (uVar5 = 0; uVar5 < *(unsigned int *)(param_1 + 0x48); uVar5 = uVar5 + 1) {
                func_02029030((unsigned int *)(*(int *)(param_1 + 0x54) + uVar5 * 0x44),iVar3,((unsigned int)0x00000e9fu),0x10,0x1000,0xa80,'\0',0,0);
            }
        }
    }
    return;
}
