typedef struct Loc16 {
    int a;
    unsigned int b;
    unsigned int c;
    unsigned int d;
} Loc16;

typedef struct Loc24 {
    int a;
    int b;
    int c;
    int d;
    int e;
    int f;
} Loc24;

extern int func_0207c7e0();
extern void func_0207da08();
extern int func_0207df34();
extern int func_0207d3f4();
extern unsigned char *func_0207d628();
extern unsigned char *func_0207c6f8();
extern void func_0207c674();
extern void func_0207cbc0();
extern void func_0207d804();
extern void func_0207e01c();

void func_0207d860(int *param_1)
{
    Loc24 buf;
    Loc16 loc;
    unsigned char *pbVar2;
    unsigned char *pbVar3;
    int iVar1;

    func_0207c7e0(param_1[4], &buf);
    if (buf.b == 0) {
        func_0207da08();
    }
    func_0207df34(param_1, &buf);
    if (*param_1 != 0) {
        param_1[3] = 0;
    }
    else {
        iVar1 = func_0207d3f4(param_1, &buf);
        param_1[3] = iVar1;
        if (iVar1 == 0) {
            func_0207da08();
        }
    }
    pbVar2 = func_0207d628(param_1, &buf, &loc);
    loc.b = (unsigned int)pbVar2[1] | ((unsigned int)pbVar2[2] << 8) | ((unsigned int)pbVar2[3] << 16) | ((unsigned int)pbVar2[4] << 24);
    pbVar3 = func_0207c6f8(pbVar2 + 5, &loc.c);
    func_0207c674(pbVar3, &loc.d);
    func_0207cbc0(param_1, &buf, pbVar2);
    func_0207d804(param_1, loc.d, loc.a);
    func_0207e01c(param_1, &buf, (unsigned char *)(buf.a + loc.c));
}
