class Obj34c50 {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7();
    virtual void s8(int a);
    virtual void s9();
    virtual void s10();
    virtual void s11();
    virtual void s12();
    virtual void s13();
    virtual void s14();
    virtual void s15();
    virtual void s16();
    virtual void s17();
    virtual void s18();
    virtual void s19();
    virtual void s20();
    virtual void s21();
    virtual void s22(int a, int b, int c);
    virtual void s23();
    virtual void s24();
    virtual void s25();
    virtual void s26();
    virtual void s27();
    virtual void s28();
    virtual void s29();
    virtual void s30();
    virtual void s31();
    virtual void s32();
    virtual void s33();
    virtual void s34();
    virtual void s35(int a);
};
extern "C" {
extern int func_02008444(void);
extern int func_0200f5c8(int p);
extern int func_0200f5e8(int p);
extern int func_0201f0e4(int a, int b, int c, int d, unsigned int e, unsigned int f);

void func_02034c50(int *param_1)
{
    int iY;
    unsigned int uVar5;
    unsigned int uVar3;
    unsigned int uVar4;
    int iVar2;
    unsigned int uVar6;
    unsigned int uVar7;
    unsigned int uVar8;

    if ((unsigned int)param_1[0x14] >= (unsigned int)param_1[0x15]) {
        ((Obj34c50 *)param_1)->s8(0);
        return;
    }
    iVar2 = func_0200f5c8(func_02008444());
    param_1[10] = (param_1[0x12] - iVar2) - 0x80000;
    iVar2 = func_0200f5e8(func_02008444());
    iY = param_1[0x13];
    iY = -iY;
    param_1[0xb] = (iY + iVar2) + 0x60000;
    uVar5 = param_1[0x1b];
    uVar3 = param_1[0x14];
    if (uVar3 > uVar5) {
        uVar3 = uVar5;
    }
    uVar4 = param_1[0x1a];
    if (uVar3 < uVar4) {
        iVar2 = param_1[0x16];
    } else {
        iVar2 = func_0201f0e4(param_1[0x16], param_1[0x17], param_1[0x18], param_1[0x19], uVar3 - uVar4, uVar5 - uVar4);
    }
    ((Obj34c50 *)param_1)->s22(iVar2, iVar2, 0x1000);
    uVar6 = param_1[0x14];
    uVar7 = param_1[0x21];
    if (uVar6 > uVar7) {
        uVar6 = uVar7;
    }
    uVar8 = param_1[0x20];
    if (uVar6 < uVar8) {
        iVar2 = param_1[0x1c];
    } else {
        iVar2 = func_0201f0e4(param_1[0x1c], param_1[0x1d], param_1[0x1e], param_1[0x1f], uVar6 - uVar8, uVar7 - uVar8);
    }
    ((Obj34c50 *)param_1)->s35((signed char)(iVar2 >> 0xc));
    param_1[0x14] = param_1[0x14] + 1;
}
}
