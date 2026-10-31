class Source02057d5c {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7();
    virtual void draw(void *quads, void *pos, int a, int b);
};

class Sprite02057d5c {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7();
    virtual void s8();
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
    virtual void s22();
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
    virtual void setAlpha(int a);
    virtual void s34();
    virtual void setPriority(int a);
    virtual void s36();
    virtual void s37();
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void setAnim(int id, int a, int b, int c, int d, int e, int f);
};

struct Tint {
    unsigned char rgb[4];
    int level;
};

extern "C" {
extern int func_02047914(Source02057d5c *src);
extern int func_02044d74(Sprite02057d5c *spr);
extern int func_02044d78(Sprite02057d5c *spr);
extern void func_0205765c(Sprite02057d5c *spr, int a, int b, int c, int d);
extern void func_0202e46c(void *quad);
extern void func_0202c700(Sprite02057d5c *spr, unsigned int i, void *quad);
extern void *func_02047b38(Source02057d5c *src);
extern void func_02047b3c(Tint *tint, void *data);
extern void func_0200ee34(Sprite02057d5c *spr, int r, int g, int b);

int func_02057d5c(unsigned char *param_1, Source02057d5c *param_2, int param_3, int param_4, unsigned short *param_5)
{
    unsigned char quads[32];
    Tint tint;
    int iVar1;
    int iVar3;
    unsigned int uVar5;

    iVar1 = func_02047914(param_2);
    if (*(short *)(param_1 + 0xe4) != iVar1) {
        return 0;
    }
    if (*(unsigned char *)(param_1 + 0xe7) == 0) {
        (*(Sprite02057d5c **)(param_1 + 0xe0))->setAnim((unsigned short)(*(int *)(param_1 + 0xdc) + 0xc6), 0, 1, 0, 1, -1, 0);
        iVar1 = func_02044d74(*(Sprite02057d5c **)(param_1 + 0xe0));
        iVar3 = func_02044d78(*(Sprite02057d5c **)(param_1 + 0xe0));
        func_0205765c(*(Sprite02057d5c **)(param_1 + 0xe0), 0, 0, iVar1, iVar3);
        *(unsigned char *)(param_1 + 0xe7) = 1;
    }
    func_0202e46c(quads);
    func_0202e46c(quads + 8);
    func_0202e46c(quads + 16);
    func_0202e46c(quads + 24);
    param_2->draw(quads, param_1 + 0xe8, param_3, param_4);
    uVar5 = 0;
    do {
        func_0202c700(*(Sprite02057d5c **)(param_1 + 0xe0), uVar5, quads + uVar5 * 8);
        uVar5 = uVar5 + 1;
    } while (uVar5 < 4);
    func_02047b3c(&tint, func_02047b38(param_2));
    (*(Sprite02057d5c **)(param_1 + 0xe0))->setAlpha(*param_5);
    *param_5 = *param_5 - 1;
    func_0200ee34(*(Sprite02057d5c **)(param_1 + 0xe0), tint.rgb[0], tint.rgb[1], tint.rgb[2]);
    iVar1 = (tint.level * 0x1f) / 100;
    (*(Sprite02057d5c **)(param_1 + 0xe0))->setPriority((signed char)(iVar1 >> 0xc));
    return 1;
}
}
