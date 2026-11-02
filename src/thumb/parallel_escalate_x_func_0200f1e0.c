struct Owner {
    char pad0[8];
    unsigned int flags8_low : 7;
    unsigned int dirty : 1;
    unsigned int flags8_high : 24;
    unsigned int flagsc_low : 10;
    unsigned int scroll : 1;
    unsigned int flagsc_high : 21;
    char pad10[0xd84];
    int x;
    int y;
    int offset_x;
    int offset_y;
    char pad_da4[0x10];
    int speed;
    int pos;
    int base_x;
    int base_y;
    char pad_dc4[0x11c];
    void *engine;
};

extern void *func_02008444(void);
extern int func_0200f320(struct Owner *owner);
extern int func_0200f330(void *object);
extern void func_0200f384(struct Owner *owner);
extern int func_0200ee64(int value);
extern void func_0200f354(void *engine, short value);
extern void func_0200f360(void *engine, short value);
extern void func_02009c7c(void *engine);
extern int func_0200f36c(void *engine);
extern int func_0200f378(void *engine);
extern void func_0200f484(struct Owner *owner);
extern void func_0200f57c(struct Owner *owner);
extern void func_02009c40(void *engine, unsigned short a, unsigned short b);

void func_0200f1e0(struct Owner *owner)
{
    int pos;

    if (owner->engine == 0) {
        return;
    }
    if (owner->scroll && func_0200f320(owner) == 0) {
        if (func_0200f330(func_02008444()) == 0) {
            owner->pos -= owner->speed;
            if (owner->pos < 0) {
                owner->pos = 0xff000;
            } else if (owner->pos > 0xff000) {
                owner->pos = 0;
            }
        }
        func_0200f384(owner);
        pos = owner->pos + owner->base_x;
        if (pos < 0) {
            pos = 0xff000;
        } else if (pos > 0xff000) {
            pos = 0;
        }
        func_0200f354(owner->engine, func_0200ee64(pos));
        func_0200f360(owner->engine, func_0200ee64(owner->base_y));
        func_02009c7c(owner->engine);
        owner->offset_x = owner->base_x + (func_0200f36c(owner->engine) << 12);
        owner->offset_y = owner->base_y + (func_0200f378(owner->engine) << 12);
        func_0200f484(owner);
    } else {
        if (owner->dirty) {
            owner->dirty = 0;
            func_0200f57c(owner);
            func_02009c40(owner->engine, func_0200ee64(owner->x), func_0200ee64(owner->y));
        }
        func_02009c7c(owner->engine);
        owner->offset_x = func_0200f36c(owner->engine) << 12;
        owner->offset_y = func_0200f378(owner->engine) << 12;
        func_0200f384(owner);
        func_0200f484(owner);
    }
}
