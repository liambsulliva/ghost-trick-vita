struct Vec {
    int x;
    int y;
    int z;
};

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
    virtual void s11();
    virtual void s12(struct Vec *v);
    virtual void s13();
    virtual void s14();
    virtual void s15();
    virtual void s16();
    virtual void s17();
    virtual void s18();
    virtual void s19();
    virtual void s20();
    virtual void s21(struct Vec *v);
    virtual void s22();
    virtual void s23();
    virtual void s24();
    virtual void s25();
    virtual void s26();
    virtual void s27();
    virtual void s28();
    virtual void s29();
    virtual void s30(unsigned short a);
    virtual void s31();
    virtual void s32();
    virtual void s33();
    virtual void s34();
    virtual void s35();
    virtual void s36();
    virtual void s37();
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void s42();
    virtual void s43();
    virtual void s44();
    virtual void s45();
    virtual void s46();
    virtual void s47();
    virtual void s48();
    virtual void s49();
    virtual void s50();
    virtual void s51();
    virtual void s52(int a);
    virtual void s53();
    virtual void s54();
    virtual void s55();
    virtual void s56();
    virtual void s57();
    virtual void s58();
    virtual void s59();
    virtual void s60();
    virtual void s61(unsigned int a);
    virtual void s62();
    virtual void s63(signed char a);
    virtual void s64(int a);
    virtual void s65();
    virtual void s66();
    virtual void s67();
    virtual void s68();
    virtual void s69();
    virtual void s70();
    virtual void s71();
    virtual void s72();
    virtual void s73();
    virtual void s74();
    virtual void s75();
    virtual void s76();
    virtual void s77();
    virtual void s78();
    virtual void s79();
    virtual void s80();
    virtual void s81();
    virtual void s82();
    virtual void s83();
    virtual void s84();
    virtual void s85();
    virtual void s86();
    virtual void s87();
    virtual void s88();
    virtual void s89();
    virtual void s90();
    virtual void s91(int a, int b, int c, signed char d);
    virtual void s92();
    virtual void s93();
    virtual void s94();
    virtual void s95();
    virtual void s96(int a);
};

class Factory {
public:
    virtual Sprite *create();
};

extern "C" {
extern unsigned short data_02086624[];
extern Factory *data_020ac194[];
extern char data_020cfe48[];
extern Sprite *func_02065ae0(char *name);
extern void func_02008398(Vec *vec, int x, int y, int z);
extern void func_02052c28(Sprite *sprite, void *anim, int a, int b);
extern void func_02052c3c(Sprite *sprite, void *anim);
extern void func_02052c44(Sprite *sprite, int value);
extern void *func_02009900(void);
extern void func_0202f7e8(void *a, void *b, int c, int d);
Sprite *func_02052ad8(int id, int x, int y, int scale_x, int scale_y, int layer, void *anim, short frame, short speed, unsigned short priority, unsigned short flip, short palette, short alpha);
}

Sprite *func_02052ad8(int id, int x, int y, int scale_x, int scale_y, int layer, void *anim, short frame, short speed, unsigned short priority, unsigned short flip, short palette, short alpha)
{
    unsigned int kind;
    Sprite *sprite;
    int color;
    Vec pos;
    Vec scale;

    kind = data_02086624[id];
    if (id == 0) {
        return 0;
    }
    if (palette < 0) {
        color = 0x1f;
    } else {
        color = palette;
    }
    if (kind >= 0x8000) {
        sprite = data_020ac194[kind - 0x8000]->create();
        if (sprite == 0) {
            return 0;
        }
        sprite->s64(palette);
        sprite->s61(kind);
    } else {
        sprite = func_02065ae0(data_020cfe48);
        if (sprite == 0) {
            return 0;
        }
        sprite->s61(kind);
        sprite->s96(id);
        sprite->s91(0x1f, 0x1f, 0x1f, color);
    }
    func_02008398(&pos, x, y, 0);
    sprite->s12(&pos);
    func_02008398(&scale, scale_x, scale_y, 0x1000);
    sprite->s21(&scale);
    if (anim != 0 && frame >= 0) {
        func_02052c28(sprite, anim, frame, speed);
    }
    if (anim != 0) {
        func_02052c3c(sprite, anim);
    }
    func_02052c44(sprite, layer);
    sprite->s30(priority);
    sprite->s52(flip != 0);
    sprite->s63(alpha);
    func_0202f7e8(func_02009900(), sprite, 7, 0);
    return sprite;
}
