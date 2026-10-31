class Task {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1c();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2c();
    virtual void v30(void *data);
};

extern "C" {

extern int func_02010194(int param_1);
extern int func_02012518(int a, unsigned int b, unsigned int c);
extern void func_0200da04(unsigned int a, unsigned int b, unsigned int c, unsigned int d);
extern void *func_02009900(void);
extern void func_0202fad4(void *a, unsigned int b);
extern void *func_0202f624(int size);
extern void *func_02011760(void *param_1);
extern void *func_02011794(void *param_1);
extern void func_02058a80(void *param_1, int param_2);
extern void func_0202f7e8(void *param_1, void *param_2, unsigned int param_3, int param_4);
extern unsigned char data_02083dc0[];
extern unsigned char data_02083dfc[];
extern unsigned char data_02083de0[];
extern unsigned char data_02083dcc[];
extern unsigned char data_02083e78[];
extern unsigned char data_02083de4[];
extern unsigned char data_020cff28[];
extern unsigned char data_020cff44[];

void func_02011690(int param_1, unsigned int param_2)
{
    int iVar1;
    Task *piVar4;
    void *puVar3;
    unsigned char *puVar5;
    unsigned char auStack_28[12];
    unsigned char auStack_1c[12];

    iVar1 = func_02010194(param_1);
    if (iVar1 == 0) {
        return;
    }
    switch (param_2) {
    case 0:
    default:
        if (func_02012518(param_1, (unsigned int)data_02083dc0, (unsigned int)data_02083dfc) != 0) {
            func_0200da04((unsigned int)data_02083de0, (unsigned int)data_02083dcc, (unsigned int)data_02083e78, (unsigned int)data_02083de4);
            return;
        }
    case 2:
        puVar3 = func_02009900();
        func_0202fad4(puVar3, (unsigned int)data_020cff28);
        piVar4 = (Task *)func_0202f624(0xd8);
        if (piVar4 != 0) {
            piVar4 = (Task *)func_02011760(piVar4);
        }
        if (piVar4 == 0) {
            return;
        }
        puVar5 = auStack_28;
        func_02058a80(puVar5, iVar1);
        piVar4->v30(puVar5);
        puVar3 = func_02009900();
        func_0202f7e8(puVar3, piVar4, 7, 0);
        return;
    case 1:
        puVar3 = func_02009900();
        func_0202fad4(puVar3, (unsigned int)data_020cff44);
        piVar4 = (Task *)func_0202f624(0xd8);
        if (piVar4 != 0) {
            piVar4 = (Task *)func_02011794(piVar4);
        }
        if (piVar4 == 0) {
            return;
        }
        puVar5 = auStack_1c;
        func_02058a80(puVar5, iVar1);
        piVar4->v30(puVar5);
        puVar3 = func_02009900();
        func_0202f7e8(puVar3, piVar4, 7, 0);
        return;
    case 0xffffffff:
        return;
    }
}
}
