typedef int code();

void *func_0200883c(void);
void *func_02008824(void *param_1);
int *func_0201e888(int *param_1, int param_2, int param_3, int param_4, int param_5);
int func_0200cb5c(void *param_1);
void *func_02007bd8(void);
int *func_02007b70(void *param_1);
void func_0207303c(int *param_1, unsigned short *param_2, unsigned int param_3);
unsigned int *func_02009c1c(void);
void func_0202f1bc(unsigned int *param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4, unsigned int param_5);

int func_0203f3ac(void)
{
    void *puVar1;
    int *piVar2;
    int *piVar5;
    int *piVar7;
    unsigned int uVar3;
    unsigned int uVar4;
    unsigned int uVar5;
    unsigned int *puVar6;
    unsigned short *puVar7;
    unsigned short *puVar8;

    puVar1 = func_0200883c();
    piVar2 = (int *)func_02008824(puVar1);
    puVar7 = 0;
    piVar5 = func_0201e888(piVar2, 2, 1, 1, 0);
    puVar1 = func_0200883c();
    puVar1 = func_02008824(puVar1);
    uVar3 = func_0200cb5c(puVar1);
    if (piVar5 != 0) {
        puVar1 = func_02007bd8();
        piVar2 = func_02007b70(puVar1);
        puVar7 = (unsigned short *)(**(code **)(*piVar2 + 0xc))(piVar2, uVar3);
        if (puVar7 != 0) {
            func_0207303c(piVar5, puVar7, uVar3);
        }
    }
    puVar1 = func_0200883c();
    piVar2 = (int *)func_02008824(puVar1);
    puVar8 = 0;
    piVar7 = func_0201e888(piVar2, 2, 2, 1, 0);
    puVar1 = func_0200883c();
    puVar1 = func_02008824(puVar1);
    uVar4 = func_0200cb5c(puVar1);
    if (piVar7 != 0) {
        puVar1 = func_02007bd8();
        piVar2 = func_02007b70(puVar1);
        puVar8 = (unsigned short *)(**(code **)(*piVar2 + 0xc))(piVar2, uVar4);
        if (puVar8 != 0) {
            func_0207303c(piVar7, puVar8, uVar4);
        }
    }
    puVar1 = func_0200883c();
    piVar2 = (int *)func_02008824(puVar1);
    piVar7 = func_0201e888(piVar2, 0, 1, 1, 0);
    puVar1 = func_0200883c();
    puVar1 = func_02008824(puVar1);
    uVar5 = func_0200cb5c(puVar1);
    puVar6 = func_02009c1c();
    func_0202f1bc(puVar6, (unsigned int)piVar7, 0x6000, uVar5, 0x15);
    if (puVar7 != 0 && puVar8 != 0) {
        puVar6 = func_02009c1c();
        func_0202f1bc(puVar6, (unsigned int)puVar7, 0, uVar3, 0x19);
        puVar6 = func_02009c1c();
        func_0202f1bc(puVar6, (unsigned int)puVar8, 0, uVar4, 0x1d);
        return 1;
    }
    return 0;
}
