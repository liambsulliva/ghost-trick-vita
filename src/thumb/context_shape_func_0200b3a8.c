struct Fields0200b3a8 {
    unsigned char before[0x20];
    unsigned int field_20;
    unsigned int field_24;
    unsigned int field_28[4];
};

extern int func_0200b40c();
extern int func_0200b41c();
extern int func_0200b3fc();
extern int func_0200b404();
extern int func_0206e5d4();
extern int func_0206ef38();

void func_0200b3a8(struct Fields0200b3a8 *param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned int uVar1;
    unsigned int uVar2;
    unsigned char auStack_28 [16];
    unsigned int uStack_18;

    param_1->field_20 = param_2;
    param_1->field_24 = param_3;
    uStack_18 = param_4;
    uVar1 = func_0206ef38(param_2);
    uVar2 = func_0206ef38(param_3);
    func_0200b3fc(param_1->field_28,uVar1,uVar2);
    uVar1 = func_0200b40c(param_4);
    uVar2 = func_0200b41c(param_4);
    func_0200b404(auStack_28,uVar1,uVar2);
    func_0206e5d4(param_1->field_28,auStack_28,param_1->field_28);
    return;
}
