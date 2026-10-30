struct Triple {
    unsigned int u0;
    int i1;
    int i2;
};

extern int func_0203f1ac();
extern int func_0203f1b4();
extern int func_0206ef28();
extern int func_0206f114();
extern int func_0206f174();
extern int func_0206f28c();

struct Obj0203f134 {
    unsigned char pad0[4];
    unsigned char f4;
    unsigned char pad5[0x1f];
    int f24;
    int f28;
    unsigned int f2c;
    int f30;
    unsigned int f34;
    unsigned int f38;
    int f3c;
    int f40;
    int f44;
};

void func_0203f134(struct Obj0203f134 *param_1,unsigned int param_2,int param_3,int param_4)

{
    int iVar1;
    unsigned int uVar2;
    unsigned int uVar3;
    int iVar4;
    int iVar5;
    struct Triple tri;

    uVar2 = param_1->f40 - 0x100;
    uVar3 = param_2;
    if ((unsigned int)((int)param_2 >> 0xc) > uVar2) {
        uVar3 = uVar2 * 0x1000;
    }
    uVar2 = param_1->f44 - 0xc0;
    iVar4 = param_3;
    if ((unsigned int)(param_3 >> 0xc) > uVar2) {
        iVar4 = uVar2 * 0x1000;
    }
    iVar5 = param_4;
    if (param_4 <= 0) {
        iVar5 = 0x1000;
    }
    func_0203f1ac(&tri,uVar3 - param_1->f24,param_1->f28 - iVar4,0);
    iVar1 = func_0206f114((int *)&tri);
    iVar1 = func_0206ef28(iVar1,iVar5);
    iVar1 = func_0203f1b4(iVar1);
    param_1->f3c = iVar1;
    func_0206f174(&tri,(int *)&tri);
    uVar2 = func_0206f28c(tri.u0,iVar5);
    param_1->f34 = uVar2;
    uVar2 = func_0206f28c(tri.i1,iVar5);
    param_1->f38 = -uVar2;
    param_1->f2c = uVar3;
    param_1->f30 = iVar4;
    param_1->f4 = 1;
    return;
}
