extern "C" {
void func_02056c8c();
extern char data_0209b874;
extern char data_020a9d64;
int func_02007b70();
void func_02007bd8();
void func_020083b0(int a, unsigned char *b);
void func_0200ee34(int a, int b, int c, int d);
void func_02047120();
void func_020471ec(unsigned char *a, void *b);
int func_0207dc50(int a, int b, int c, unsigned int d, unsigned int e);
}

struct Obj0204aa58Cpp {
    virtual void s0(); virtual void s1(); virtual int s2(int a, int b); virtual void s3();
    virtual void s4(); virtual void s5(); virtual void s6(); virtual void s7();
    virtual void s8(); virtual void s9(); virtual void s10(); virtual void s11();
    virtual void s12(); virtual void s13(); virtual void s14(); virtual void s15();
    virtual void s16(); virtual void s17(); virtual void s18(); virtual void s19();
    virtual void s20(); virtual void s21(); virtual void s22(); virtual void s23();
    virtual void s24(); virtual void s25(); virtual void s26(); virtual void s27();
    virtual void s28(); virtual void s29(); virtual void s30(); virtual void s31();
    virtual void s32(); virtual void s33(int a); virtual void s34(); virtual void s35();
    virtual void s36(); virtual void s37(); virtual void s38(); virtual void s39();
    virtual void s40(); virtual void s41(); virtual void s42(int a, int b, int c, int d, int e, int f, int g); virtual void s43();
    virtual void s44(); virtual void s45(); virtual void s46(int a, int b); virtual void s47();
    virtual void s48(); virtual void s49(); virtual void s50(); virtual void s51();
    virtual void s52(); virtual void s53(); virtual void s54(); virtual void s55();
    virtual void s56(); virtual void s57(); virtual void s58(); virtual void s59();
    virtual void s60(); virtual void s61(); virtual void s62(); virtual void s63();
    virtual void s64(); virtual void s65(); virtual void s66(); virtual void s67();
    virtual void s68();
};


extern "C" Obj0204aa58Cpp *func_0204aa58(Obj0204aa58Cpp *param_1)
{
    int iVar1;
    Obj0204aa58Cpp *piVar2;
    unsigned int uVar3;
    unsigned char auStack_20 [12];

    func_02047120();
    *(int *)param_1 = (int)&data_020a9d64;
    uVar3 = 0;
    ((int *)param_1)[0x2e] = 0;
    ((int *)param_1)[0x2f] = 0;
    *(unsigned short *)((int *)param_1 + 0x30) = 1;
    *(unsigned short *)((int)param_1 + 0xc2) = 1;
    iVar1 = func_0207dc50(1,200,8,(unsigned int)0x02056b2du,(unsigned int)func_02056c8c);
    ((int *)param_1)[0x2e] = iVar1;
    func_0200ee34(iVar1,0x1f,0,0);
    ((Obj0204aa58Cpp *)((int *)param_1)[0x2e])->s42(0xf,0,1,0,1,-1,0);
    ((Obj0204aa58Cpp *)((int *)param_1)[0x2e])->s46(0x8000,0x8000);
    ((Obj0204aa58Cpp *)((int *)param_1)[0x2e])->s33(0x560);
    func_02007bd8();
    piVar2 = (Obj0204aa58Cpp *)func_02007b70();
    iVar1 = piVar2->s2(0x180,(int)&data_0209b874);
    ((int *)param_1)[0x2f] = iVar1;
    func_020471ec(auStack_20,param_1);
    do {
        func_020083b0(((int *)param_1)[0x2f] + uVar3 * 0xc,auStack_20);
        uVar3 = uVar3 + 1;
    } while (uVar3 < 0x20);
    param_1->s33(0x560);
    return param_1;
}
