extern int *func_0202f624(int size);
extern int *func_020413fc(int *p);
extern void func_020415b0(int obj, unsigned int id, unsigned int v, int a, int b, int c);
extern void func_02041518(int obj, unsigned int id, unsigned int v);
extern int func_02009900(void);
extern void func_0202f7e8(int a, int b, int c, int d);
extern int func_02008444(void);
extern int func_020322d4(int p);

struct Entry407f4 {
    unsigned short id;
    unsigned short v;
};

void func_020407f4(int *param_1, const struct Entry407f4 *param_2)
{
    int *piVar2;
    int iVar7;
    unsigned int uVar6;
    int sVar1;
    int sVar10;
    int v;

    piVar2 = func_0202f624(0xec);
    if (piVar2 != 0) {
        piVar2 = func_020413fc(piVar2);
    }
    param_1[10] = (int)piVar2;
    if (piVar2 != 0) {
        iVar7 = 0;
        while ((uVar6 = param_2[iVar7].id) != 0) {
            sVar10 = 0;
            sVar1 = 0;
            if (uVar6 == 0x12e7 || uVar6 == 0x12e7 - 7) {
                sVar10 = 0x14;
            }
            if (uVar6 == 0x807e - 1 || uVar6 == 0x807e || uVar6 == 0x807e + 2) {
                sVar1 = -6;
            }
            if ((unsigned short)(uVar6 + 0x7f7d) <= 1) {
                sVar1 = -4;
            }
            v = param_2[iVar7].v;
            func_020415b0(param_1[10], uVar6, (unsigned char)v, sVar10, sVar1, 0);
            iVar7 = iVar7 + 1;
        }
        func_0202f7e8(func_02009900(), param_1[10], 10, 0);
    }
    if (func_020322d4(func_02008444()) != 0) {
        func_020415b0(param_1[10], 0x12e5, 9, 0, 0, 1);
    } else {
        func_02041518(param_1[10], 0x12e5, 9);
    }
}
