extern char data_020b18a0;
extern unsigned int func_020030a8(unsigned int param_1);
extern unsigned int func_02003030(unsigned int param_1);
extern unsigned int func_0200306c(unsigned int param_1);
extern unsigned int func_020030e4(unsigned int param_1, unsigned int param_2);
extern int func_02000d9c(int object, int index, int offset);
extern void func_0207c594(void *object, int size);
extern void func_02003138(int param_1, int param_2, void *param_3);
extern unsigned int func_0207b3dc(unsigned int param_1);
extern unsigned int func_0207bb44(unsigned int param_1, unsigned int param_2);
extern unsigned int func_0207b424(unsigned int param_1);
extern unsigned int func_0207c1d8(unsigned int param_1, unsigned int param_2);
extern unsigned int func_0207bd24(unsigned int param_1, unsigned int param_2);
extern unsigned int func_0207ba84(unsigned int param_1);
extern void func_020079fc(int param_1, unsigned int param_2);

void func_020046d0(unsigned int param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4, unsigned char param_5, unsigned int param_6, int param_7)
{
    unsigned int uVar6;
    int v;
    int uVar1;
    int *piVar2;
    unsigned int uVar3;
    unsigned int uVar4;
    int iVar5;
    unsigned short buf[18];

    uVar6 = 0xffff;
    v = param_6;
    if (param_1 != 0) {
        uVar1 = func_020030a8(param_1);
        uVar6 = uVar6 & uVar1;
    }
    if (param_2 != 0xffffffff) {
        uVar1 = func_02003030(param_2);
        uVar6 = uVar6 & uVar1;
    }
    if (param_3 != 0xffffffff) {
        uVar1 = func_0200306c(param_3);
        uVar6 = uVar6 & uVar1;
    }
    if (param_4 != 0xff || param_5 != 0xff) {
        uVar1 = func_020030e4(param_4, (unsigned int)param_5);
        uVar6 = uVar6 & uVar1;
    }
    for (uVar1 = 0; uVar1 < 16; uVar1++) {
        if ((((int)uVar6 >> uVar1) & 1) != 0) {
            iVar5 = *(int *)(*(int *)((char *)&data_020b18a0 + 0x10) + 0x1580);
            piVar2 = (int *)func_02000d9c(iVar5, *(short *)(*(int *)((char *)&data_020b18a0 + 0x10) + uVar1 * 0x4c + 0x15bc), *(int *)(*(int *)(iVar5 + 0x98) + 8));
            if (piVar2 == 0) {
                return;
            }
            iVar5 = *(int *)(*(int *)(*(int *)(*(int *)((char *)&data_020b18a0 + 0x10) + 0x1580) + 0x90) + *piVar2 * 0x10 + 0x14);
            if (iVar5 == 0) {
                return;
            }
            func_0207c594(buf, 0x24);
            func_02003138(iVar5 + *(int *)(iVar5 + 0x18), 0, buf);
            if (param_7 == 2) {
                v = v + buf[1];
            }
            else if (param_7 == 4) {
                v = (int)(buf[1] * v) / 100;
            }
            if ((int)v < 1) {
                v = 1;
            }
            else if (0x3ff < (int)v) {
                v = 0x3ff;
            }
            uVar3 = func_0207b3dc(v);
            uVar3 = func_0207bb44(0x43800000, uVar3);
            uVar4 = func_0207b424(buf[1]);
            uVar3 = func_0207c1d8(uVar3, uVar4);
            uVar3 = func_0207bd24(0x3f000000, uVar3);
            uVar3 = func_0207ba84(uVar3);
            func_020079fc(uVar1, uVar3);
        }
    }
}
