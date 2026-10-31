class Sprite {
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
    virtual void show();
    virtual void s12();
    virtual void setPos(int x, int y, int z);
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
    virtual void setScale(int scale);
    virtual void s34();
    virtual void s35();
    virtual void s36();
    virtual void s37();
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void setAnim(unsigned short id, int a, int b, int c, int d, int e, int f);
    virtual void s43();
    virtual void s44();
    virtual void s45();
    virtual void setSize(int w, int h);
};
extern "C" {
extern char data_020b9d84[];
extern char data_020b9df4[];
void *func_02031254(unsigned int size);
int func_0201bce4(void *object);
void *func_02009900(void);
int func_0202fb48(void *scene, char *kind, int start);
void func_020183d0(int object, int value);
int func_02018710(Sprite *sprite);
int func_02019094(int object);
int func_02019098(int object);
int func_02017214(char *object);
Sprite *func_0201bddc(int slot, unsigned short key);
void func_0201c248(Sprite *sprite, int a, int b);

void func_02018fa8(int param_1, unsigned int param_2)
{
    int iVar1;
    void *mem;
    Sprite *sprite;
    int iVar4;
    int sum;

    if (*(int *)(param_1 + 0x80) == 0) {
        mem = func_02031254(0x14);
        iVar1 = (int)mem;
        if (mem != 0) {
            iVar1 = func_0201bce4(mem);
        }
        *(int *)(param_1 + 0x80) = iVar1;
        iVar1 = func_0202fb48(func_02009900(), data_020b9d84, -1);
        if (iVar1 != 0) {
            func_020183d0(iVar1, *(int *)(param_1 + 0x80));
        }
    }
    sprite = func_0201bddc(*(int *)(param_1 + 0x80), (unsigned short)param_2);
    if (sprite != 0) {
        iVar1 = func_02018710(sprite);
        if (iVar1 != 0) {
            *(int *)(param_1 + 0x84) = func_02019094(iVar1);
            func_02019098(iVar1);
        }
        iVar1 = func_02017214(data_020b9df4);
        sum = (int)*(short *)(param_1 + 0x30) + (int)*(short *)(param_1 + 0x42) + (int)*(short *)(param_1 + 0x34) + (int)*(short *)(param_1 + 0x3c) + (int)*(short *)(param_1 + 0x3e);
        iVar1 = (sum + iVar1) * 0x10000 >> 0x10;
        if (*(int *)(param_1 + 0x84) == 0x100) {
            iVar1 = 0;
        }
        iVar4 = (int)(((int)*(short *)(param_1 + 0x44) +
                       (unsigned int)*(unsigned char *)(param_1 + 0x3a) *
                       ((unsigned int)*(unsigned char *)(param_1 + 0x51) + (int)*(short *)(param_1 + 0x40)) + 1) *
                      0x10000) >> 0x10;
        if (*(unsigned short *)(param_1 + 0x20) == 4) {
            iVar4 = (iVar4 + -0x90) * 0x10000 >> 0x10;
        }
        sprite->s10();
        func_0201c248(sprite, iVar1 << 0xc, iVar4 << 0xc);
    }
}
}
