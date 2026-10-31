extern int func_0207a4dc();
extern int func_0207a578();

typedef struct Obj {
    unsigned int w0;
    unsigned int f4a : 2;
    unsigned int f4b : 3;
    unsigned int f4c : 2;
    unsigned int f4d : 3;
    unsigned int f4e : 22;
    unsigned int f8 : 3;
    unsigned int f8r : 29;
    unsigned char c0c;
    unsigned char flag0d;
    unsigned char pad0e[2];
    unsigned int w10;
    unsigned int w14;
    unsigned int w18;
    unsigned int w1c;
    unsigned int w20;
    unsigned int w24;
    unsigned int w28;
} Obj;

unsigned int func_0207a5fc(Obj *param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4)
{
    unsigned int uVar1;
    int iVar2;

    if (param_1 == (Obj *)0) {
        uVar1 = func_0207a4dc();
        return uVar1;
    }
    if (param_1->flag0d != 0 || param_1->f4d == 0) {
        return 0xffffffff;
    }
    if (param_1->f4b == 1) {
        return 0;
    }
    if (param_1->f8 >= 3) {
        param_1->f8 = 2;
    }
    if (param_1->f8 == 2) {
        param_1->w28 = 0;
    }
    if (param_1->f8 != 1) {
        param_1->f8 = 0;
        return 0;
    }
    iVar2 = func_0207a578(param_1, 0, param_3, param_4);
    if (iVar2 == 0) {
        param_1->f8 = 0;
        param_1->w18 = 0;
        param_1->w28 = 0;
        return 0;
    }
    param_1->flag0d = 1;
    param_1->w28 = 0;
    return 0xffffffff;
}
