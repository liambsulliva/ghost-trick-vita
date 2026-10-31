struct Entry {
    unsigned short data[7];
};
struct Info {
    unsigned short a;
    unsigned short w;
    unsigned short h;
};
struct Obj {
    unsigned char pad0[0x14];
    struct Info *info;
    struct Info *info2;
    unsigned char pad1c[0x22 - 0x1c];
    unsigned char b22;
    unsigned char b23;
    unsigned char pad24[0x2a - 0x24];
    short state;
    unsigned char pad2c[0x34 - 0x2c];
    short s34;
    unsigned char pad36[0x3a - 0x36];
    unsigned char b3a;
    unsigned char pad3b[0x40 - 0x3b];
    short s40;
    unsigned char pad42[0x46 - 0x42];
    unsigned short u46;
    unsigned short u48;
    unsigned short u4a;
    unsigned short u4c;
    unsigned char pad4e[0x51 - 0x4e];
    unsigned char b51;
    unsigned char pad52[0x58 - 0x52];
    unsigned char b58;
    unsigned char pad59[0x74 - 0x59];
    unsigned short u74;
    unsigned char b76;
    unsigned char pad77[0x80 - 0x77];
    unsigned short u80;
    unsigned short u82;
    unsigned short count;
    unsigned char b86;
    unsigned char b87;
    unsigned short u88;
    unsigned char pad8a[0x8c - 0x8a];
    unsigned short u8c;
    struct Entry entries[8];
};
extern void func_0201b580(struct Entry *entry, int w, int h);
extern void func_0201b588(struct Entry *entry, int value);
extern void func_0201b58c(struct Entry *entry, int value, unsigned short scale);
extern int func_0201b594(struct Entry *entry);

int func_0201b4d0(struct Obj *obj)
{
    obj->b87 = 1;
    func_0201b580(&obj->entries[obj->count], obj->info->w, obj->info->h);
    func_0201b588(&obj->entries[obj->count], obj->u80);
    obj->u88 = obj->s34;
    func_0201b58c(&obj->entries[obj->count], obj->u88, (obj->b51 + obj->s40) * obj->b3a);
    if (obj->u8c != 0) {
        obj->state = 9;
    }
    if (obj->u82 == func_0201b594(&obj->entries[obj->count])) {
        obj->state = 6;
    }
    return 0;
}
