struct W15Obj {
    virtual int s0();
    virtual int s1();
    virtual int s2();
    virtual int s3();
    virtual int s4();
    virtual int s5();
    virtual int s6();
    virtual int s7();
    virtual int s8();
    virtual int s9();
    virtual int s10();
    virtual int s11();
    virtual int s12();
    virtual int s13();
    virtual int s14();
};

extern "C" {
extern int func_02027518(int v);
extern void *func_02008444(void);
extern int func_0201260c(void);
extern int func_020322d4(void *p);
extern void func_020326a4(void *p);
extern unsigned int func_0203247c(void *p);
extern void func_0201192c(void *p, int v);
extern void func_0202fbcc(void *buf, int v);
extern int func_0200eac4(void *buf);
extern void *func_0200eab4(void *buf);
extern int func_02008828(void *p, void *cls);
extern void *func_0200eab8(void *buf);
extern void func_0200eabc(void *buf, int v);
extern void func_0203388c(void *p, int v);
extern void func_0202f64c(void *p);
extern void func_02032900(void *p, int v);
extern void func_02010ef4(void *p, int a, int b);
extern void *func_02009900(void);
extern void *func_0202fb48(void *p, void *q, int v);
extern void func_02008418(void *p);
extern void func_020373f0(void *p);
extern void func_020373f8(void *p, int a, int b, int c);

void func_02037274(int *param_1)
{
    unsigned int uVar1;
    void *puVar2;
    int iVar3;
    unsigned char *pcVar7;
    void *puVar6;
    void *puVar4;
    int *piVar5;
    unsigned char buf[16];

    iVar3 = func_02027518(0x1022);
    pcVar7 = (unsigned char *)0x020cdd0c;
    *pcVar7 = (unsigned char)iVar3;
    func_02008444();
    uVar1 = func_0201260c();
    if (uVar1 != 0) {
        puVar2 = func_02008444();
        iVar3 = func_020322d4(puVar2);
        if (iVar3 == 0 && *pcVar7 != 0) {
            *((unsigned char *)param_1 + 0x57) = 1;
        }
    }
    if (param_1[2] == 2) {
        *((unsigned char *)param_1 + 0x57) = 1;
        param_1[2] = 0;
    }
    if (param_1[2] == 3) {
        param_1[2] = 0;
    } else {
        pcVar7[1] = 0;
    }
    puVar2 = func_02008444();
    iVar3 = func_020322d4(puVar2);
    if (iVar3 != 0 && *pcVar7 == 0 && *((unsigned char *)param_1 + 0x57) == 0 && pcVar7[1] == 0) {
        pcVar7[2] = 1;
    }
    func_020326a4(param_1);
    puVar2 = func_02008444();
    *((unsigned char *)param_1 + 0x54) = (unsigned char)func_0203247c(puVar2);
    puVar2 = func_02008444();
    func_0201192c(puVar2, 8);
    func_0202fbcc(buf, 6);
    puVar4 = (void *)0x020cf87c;
    while (func_0200eac4(buf) != 0) {
        puVar6 = func_0200eab4(buf);
        if (func_02008828(puVar6, puVar4) != 0) {
            piVar5 = (int *)func_0200eab8(buf);
            ((W15Obj *)piVar5)->s14();
        }
        func_0200eabc(buf, 0);
    }
    func_0203388c((void *)param_1[8], 0);
    func_0202f64c(param_1 + 9);
    iVar3 = ((W15Obj *)param_1)->s11();
    if (iVar3 != 0) {
        func_02032900((void *)param_1[8], 1);
    }
    puVar2 = func_02008444();
    func_02010ef4(puVar2, 0xff, -1);
    puVar6 = func_02009900();
    puVar6 = func_0202fb48(puVar6, (void *)0x020ce7a0, -1);
    if (puVar6 != 0) {
        func_02008418(puVar6);
    }
    puVar2 = func_02008444();
    iVar3 = func_020322d4(puVar2);
    if (iVar3 != 0) {
        func_020373f0(param_1);
    }
    if (*pcVar7 != 0) {
        puVar2 = func_02008444();
        iVar3 = func_020322d4(puVar2);
        if (iVar3 == 0) {
            return;
        }
        if (*((unsigned char *)param_1 + 0x57) == 0) {
            return;
        }
    }
    func_020373f8(param_1, 1, 0, 0);
}
}
