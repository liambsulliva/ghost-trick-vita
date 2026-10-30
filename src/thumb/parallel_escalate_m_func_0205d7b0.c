extern int func_02008378();
extern int func_020083b0();
extern int func_020083c0();
extern int func_020349b0();
extern int func_020452e8();
extern int func_0205d820();
extern int func_0205d834();
extern int func_0206ef28();
extern int func_0206f2a4();

struct Triple {
    unsigned int u0;
    int i1;
    unsigned int u2;
};

void func_0205d7b0(unsigned int param_1,unsigned int param_2,unsigned int *param_3,unsigned short *param_4,
                                    unsigned int *param_5,unsigned int *param_6,unsigned int param_7,unsigned int param_8)

{
    unsigned short sVar1;
    unsigned int uVar2;
    struct Triple tri;
    unsigned char auStack_2c [12];
    unsigned char auStack_38 [12];

    func_02008378(auStack_38,param_7,param_8);
    func_020349b0(auStack_2c,auStack_38,0x800);
    func_020083b0(param_2,auStack_2c);
    func_020083c0(&tri,param_7,param_2);
    uVar2 = func_020452e8(&tri);
    *param_3 = uVar2;
    sVar1 = func_0206f2a4(func_0206ef28(tri.u0,uVar2));
    *param_4 = sVar1;
    if (tri.i1 < 0) {
        *param_4 = -*param_4;
    }
    uVar2 = func_0205d820(*param_4);
    *param_5 = uVar2;
    uVar2 = func_0205d834(*param_4);
    *param_6 = uVar2;
    return;
}
