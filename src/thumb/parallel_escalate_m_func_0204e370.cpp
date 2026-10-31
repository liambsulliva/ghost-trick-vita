class Actor {
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
    virtual void v30();
    virtual void v34();
    virtual void v38(int value);
};
extern "C" {
extern char data_020cec70;
extern char data_0209bd98;
extern char data_0209bd94;
extern char data_0209bd9c;
extern char *func_02009900(void);
extern int func_0204e2c8(void *table);
extern void func_020153d8(int object, int *actor);
extern void func_0202f7e8(char *list, int object, int a, int b);
extern void func_0204e550(int *actor, int *a, int *b, int *c, char *d, int e, int f);

void func_0204e370(int *param_1)
{
    int iVar1;
    int iVar2;
    char *puVar3;
    int iVar4;
    int value;

    iVar4 = param_1[0x1c];
    if (iVar4 != 0) {
        iVar1 = param_1[10];
        param_1[10] = iVar1 + iVar4;
        if ((0 < iVar4) && (iVar1 + iVar4 > param_1[0x1b])) {
            param_1[0x1c] = 0;
            iVar2 = func_0204e2c8(&data_020cec70);
            if (iVar2 != 0) {
                func_020153d8(iVar2, param_1);
                puVar3 = func_02009900();
                func_0202f7e8(puVar3, iVar2, 7, 0);
            }
        }
        if ((param_1[0x1c] < 0) && (param_1[10] < param_1[0x1b])) {
            param_1[0x1c] = 0;
            iVar4 = func_0204e2c8(&data_020cec70);
            if (iVar4 != 0) {
                func_020153d8(iVar4, param_1);
                puVar3 = func_02009900();
                func_0202f7e8(puVar3, iVar4, 7, 0);
            }
        }
        ((Actor *)param_1)->v38(param_1[10]);
    }
    value = 0x481;
    func_0204e550(param_1, param_1 + 0x11, param_1 + 0x12, param_1 + 0x17, &data_0209bd98, 0xad, value);
    func_0204e550(param_1, param_1 + 0x13, param_1 + 0x14, param_1 + 0x18, &data_0209bd94, 0xae, value + 1);
    func_0204e550(param_1, param_1 + 0x15, param_1 + 0x16, param_1 + 0x19, &data_0209bd9c, 0xac, value + 2);
}
}
