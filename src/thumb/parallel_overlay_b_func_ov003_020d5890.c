extern int func_02022df0();
extern int func_020082f8();
extern unsigned char func_ov003_020d58c0();
extern int func_ov003_020d58e8();
extern char data_020cda64;

struct Ov3Obj {
    unsigned char pad0[40];
    int field28;
};

void func_ov003_020d5890(struct Ov3Obj *param_1)
{
    int iVar1;
    unsigned char uVar2;

    iVar1 = func_02022df0(param_1);
    if (iVar1 != 0) {
        iVar1 = func_020082f8(iVar1, ((unsigned int)&data_020cda64));
        if (iVar1 != 0) {
            uVar2 = func_ov003_020d58c0(iVar1);
            if (uVar2 != param_1->field28) {
                param_1->field28 = uVar2;
                func_ov003_020d58e8(param_1);
            }
        }
    }
}
