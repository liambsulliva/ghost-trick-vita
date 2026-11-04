extern "C" {
extern void func_02029a04(void *object);
extern void func_02031270(void *object);
extern void func_02042718(void);
extern void func_02042774(void);
extern void *func_0207dc50(unsigned int count, int size, int align, void (*ctor)(void), void (*dtor)(void));
extern void *func_02007bd8(void);
extern void *func_02008444(void);
extern int func_0200f5c8(void *object);
extern int func_0200f5e8(void *object);
extern void *func_020083a0(int id);
extern void func_020082d8(void *dst, void *src);
extern void func_0207303c(void *src, void *dst, int size);
extern char data_02085178;
}

struct Pos02028c34 {
    short x;
    short y;
};

struct Record02028c34 {
    short x;
    short y;
    unsigned int data[8];
    int value;
};

struct Vec02028c34 {
    int x;
    int y;
    int z;
};

struct Elem02028c34 {
    char data[0x54];
};

class Manager02028c34 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual Record02028c34 *slot2(unsigned int size, void *tag);
};

class Part02028c34 {
public:
    ~Part02028c34() { func_02029a04(this); }
    static void operator delete(void *object) { func_02031270(object); }
};

struct Owner02028c34 {
    unsigned int count;
    int total;
    Elem02028c34 *elems;
    Part02028c34 *part;
    Record02028c34 *records;
    int id;
    short palette[16];
    unsigned char flip;
    unsigned char dirty;
    unsigned char active;
    unsigned char mode;
    short min_x;
    short min_y;
    short max_x;
    short max_y;
};

extern "C" {
extern Manager02028c34 *func_02007b70(void *object);
extern unsigned int func_02028ff8(Part02028c34 *part);
extern Pos02028c34 *func_02029004(Part02028c34 *part);
extern void *func_02029008(Part02028c34 *part);
extern unsigned char **func_0202900c(Part02028c34 *part);
extern void func_02029014(Elem02028c34 *elem, int value);
extern void func_02029018(Elem02028c34 *elem, int value);
extern void func_0202901c(Elem02028c34 *elem, int value);
extern int func_02029020(Elem02028c34 *elem);
extern int func_02029024(Elem02028c34 *elem);
extern void func_02042820(Elem02028c34 *elem, void *data, int size, int a, int b, int c, int d, int e);
extern void func_020428f0(Elem02028c34 *elem, void *data, int size, int a, int b, int c, int d, int e, int f);

void func_02028c34(Owner02028c34 *owner, int id, unsigned char flip, int check, unsigned char mode)
{
    Pos02028c34 *pos;
    void *colors;
    unsigned char **tiles;
    Vec02028c34 vec;
    unsigned int i;
    int hidden;
    int dx;
    int dy;
    unsigned int row;
    unsigned int col;
    unsigned char byte;
    unsigned char lo;
    unsigned char hi;
    unsigned int word;
    unsigned int shift;
    int k;

    if (owner->part == 0 || owner->dirty != 0) {
        return;
    }
    owner->id = id;
    owner->count = func_02028ff8(owner->part);
    owner->flip = flip;
    owner->mode = mode;
    if (owner->elems == 0) {
        owner->elems = (Elem02028c34 *)func_0207dc50(owner->count, 0x54, 8, func_02042718, func_02042774);
    }
    if (owner->records == 0) {
        owner->records = func_02007b70(func_02007bd8())->slot2(owner->count * 0x28, &data_02085178);
    }
    if (owner->elems == 0 || owner->records == 0) {
        return;
    }
    pos = func_02029004(owner->part);
    colors = func_02029008(owner->part);
    tiles = func_0202900c(owner->part);
    func_0200f5c8(func_02008444());
    func_0200f5e8(func_02008444());
    func_020082d8(&vec, func_020083a0(owner->id));
    if (check != 0) {
        hidden = 1;
        for (i = 0; i < owner->count; i++) {
            dx = vec.x + pos[i].x * 0x1000 - func_0200f5c8(func_02008444()) - 0x80000;
            dy = pos[i].y * -0x1000 - vec.y + func_0200f5e8(func_02008444()) + 0x60000;
            if (dx <= 0x84000 && dx >= -0x84000 && dy <= 0x64000 && dy >= -0x64000) {
                hidden = 0;
                break;
            }
        }
        if (hidden) {
            return;
        }
    }
    for (i = 0; i < owner->count; i++) {
        if (owner->mode != 0) {
            ((Record02028c34 *)((char *)owner->records + i * 0x28))->data[0] = 0;
            ((Record02028c34 *)((char *)owner->records + i * 0x28))->data[1] = 0;
            ((Record02028c34 *)((char *)owner->records + i * 0x28))->data[2] = 0;
            ((Record02028c34 *)((char *)owner->records + i * 0x28))->data[3] = 0;
            for (row = 0; row < 4; row++) {
                for (col = 0; col < 8; col++) {
                    byte = (*(unsigned char **)((char *)tiles + i * 4))[col * 4 + row];
                    lo = byte & 0xf;
                    hi = (byte & 0xf0) >> 4;
                    word = (col & ~1) >> 1;
                    shift = (col & 1) * 16 + row * 4;
                    if (lo != 0) {
                        owner->records[i].data[word] = (1 << shift) | owner->records[i].data[word];
                    }
                    if (hi != 0) {
                        owner->records[i].data[word] |= 1 << (shift + 2);
                    }
                }
            }
        } else {
            func_0207303c((*(unsigned char **)((char *)tiles + i * 4)), ((Record02028c34 *)((char *)owner->records + i * 0x28))->data, 0x20);
        }
        ((Record02028c34 *)((char *)owner->records + i * 0x28))->x = ((Pos02028c34 *)((char *)pos + i * 4))->x;
        ((Record02028c34 *)((char *)owner->records + i * 0x28))->y = ((Pos02028c34 *)((char *)pos + i * 4))->y;
        if (owner->flip != 0) {
            ((Record02028c34 *)((char *)owner->records + i * 0x28))->x = -8 - ((Record02028c34 *)((char *)owner->records + i * 0x28))->x;
        }
        if (owner->min_x > ((Record02028c34 *)((char *)owner->records + i * 0x28))->x) {
            owner->min_x = ((Record02028c34 *)((char *)owner->records + i * 0x28))->x;
        }
        if (owner->min_y > ((Record02028c34 *)((char *)owner->records + i * 0x28))->y) {
            owner->min_y = ((Record02028c34 *)((char *)owner->records + i * 0x28))->y;
        }
        if (owner->max_x < ((Record02028c34 *)((char *)owner->records + i * 0x28))->x) {
            owner->max_x = ((Record02028c34 *)((char *)owner->records + i * 0x28))->x;
        }
        if (owner->max_y < ((Record02028c34 *)((char *)owner->records + i * 0x28))->y) {
            owner->max_y = ((Record02028c34 *)((char *)owner->records + i * 0x28))->y;
        }
        func_02029014(&owner->elems[i], 0);
        func_02029018(&owner->elems[i], 0);
        if (owner->mode != 0) {
            func_0202901c(&owner->elems[i], 2);
            func_02042820(&owner->elems[i], ((Record02028c34 *)((char *)owner->records + i * 0x28))->data, 0x10, 4, 1, 0, 0xffff, -1);
        } else {
            func_0202901c(&owner->elems[i], 3);
            func_02042820(&owner->elems[i], ((Record02028c34 *)((char *)owner->records + i * 0x28))->data, 0x20, 4, 1, 0, 0xffff, -1);
        }
        if (i == 0) {
            func_0207303c(colors, owner->palette, 0x20);
            if (owner->mode != 0) {
                for (k = 0; k < 0x10; k++) {
                    if (k == 0) {
                        owner->palette[k] = 0;
                    } else {
                        owner->palette[k] = 0x7fff;
                    }
                }
                func_020428f0(&owner->elems[i], owner->palette, 8, 4, 0, 0xffff, 0, -1, 0x100);
            } else {
                func_020428f0(&owner->elems[i], owner->palette, 0x20, 4, 0, 0xffff, 0, -1, 0x100);
            }
        }
        ((Record02028c34 *)((char *)owner->records + i * 0x28))->value = func_02029020(&owner->elems[i]);
    }
    owner->total = func_02029024(owner->elems);
    owner->dirty = 1;
    if (owner->part != 0) {
        delete owner->part;
        owner->part = 0;
    }
}
}
