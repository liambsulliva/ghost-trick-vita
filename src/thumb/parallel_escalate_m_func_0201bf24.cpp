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
struct Slot {
    int head;
    int list[3];
    unsigned char used;
};
extern "C" {
extern char data_020b9dbc[];
int func_0201c150(Slot *slot, int key);
int func_0201c1c8(Slot *slot, int key);
Sprite *func_0201638c(void *pool);
void func_0201c0bc(Slot *slot, Sprite *sprite, int extra);
void func_0205765c(Sprite *sprite, int a, int b, int c, int d);
void *func_02009900(void);
void func_0202f7e8(void *scene, Sprite *sprite, int a, int b);
void func_0201bec0(int *list, Sprite *sprite);

Sprite *func_0201bf24(Slot *slot, int key, int extra)
{
    int id;
    Sprite *sprite;
    if (func_0201c150(slot, key) == 0) {
        return 0;
    }
    id = func_0201c1c8(slot, key);
    sprite = func_0201638c(data_020b9dbc);
    if (sprite != 0) {
        sprite->setAnim(id, 0, 1, 0, 1, -1, 0);
        if (extra != 0) {
            func_0201c0bc(slot, sprite, extra);
        }
        sprite->setScale(0x100);
        func_0205765c(sprite, 0, 0, 0x100000, 0x28000);
        sprite->setSize(0x100000, 0x28000);
        sprite->setPos(0, -0x3a000, 0);
        sprite->show();
        func_0202f7e8(func_02009900(), sprite, 1, 0);
        func_0201bec0(slot->list, sprite);
        slot->used = 1;
    }
    return sprite;
}
}
