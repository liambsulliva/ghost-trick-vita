class Allocator {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void *Alloc(int size);
};
class Target {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void Seek(int offset,int origin);
    virtual void Read(void *buffer,int size);
};
extern "C" {
struct Flags {
    unsigned char bit0 : 1;
    unsigned char bit1 : 1;
    unsigned char rest : 6;
};
struct Sprite {
    Target *target;
    unsigned char byte4;
    struct Flags flags;
    signed char x;
    signed char y;
    unsigned short width;
};
extern void *func_02007bd8(void);
extern Allocator *func_02007b70(void *system);
extern void func_02009a18(void *object,int value,int extra);
void *func_0200a7dc(struct Sprite *sprite,int x,int y,void *buffer)
{
    if (sprite->target != 0) {
        if (buffer == 0) {
            buffer = func_02007b70(func_02007bd8())->Alloc(0x100);
        }
        if (buffer != 0) {
            sprite->target->Seek((((x - sprite->x) + (y - sprite->y) * ((unsigned int)sprite->width >> 4)) << 8) + 0x400,0);
            sprite->target->Read(buffer,0x100);
            return buffer;
        }
    }
    else if (sprite->flags.bit1) {
        if (buffer == 0) {
            buffer = func_02007b70(func_02007bd8())->Alloc(0x100);
        }
        func_02009a18(buffer,-1,0x100);
        return buffer;
    }
    return 0;
}
}
