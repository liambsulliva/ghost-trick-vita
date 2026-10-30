struct Entry {
    short x;
    short y;
    short pad;
    short height;
    short rest[5];
};
struct Owner {
    unsigned char pad[0x10];
    void *model;
    unsigned char pad1[0x20 - 0x14];
    unsigned short row;
    unsigned char pad2[0x30 - 0x22];
    short speed;
    short limit;
    unsigned char pad3[0x38 - 0x34];
    unsigned short kind;
    unsigned char pad4[0x40 - 0x3a];
    short a;
    short height;
    short b;
    unsigned short x;
    short y;
    short x2;
    short c;
    unsigned char pad5[0x60 - 0x4e];
    void *shadow;
};
extern struct Entry data_02084a90[];
extern char data_020b9df4[];
extern short func_020176d8(struct Owner *owner, int kind, int row);
extern short func_02017c70(struct Owner *owner, unsigned char kind);
extern void func_0201c6c4(char *object, void *model);
extern void func_0201cf2c(char *object, void *shadow);
extern void func_02016fbc(char *object, int value);

void func_0201909c(struct Owner *owner)
{
    unsigned short row;
    owner->row = 0;
    row = owner->row;
    owner->speed = 0;
    owner->limit = 0x90;
    owner->height = data_02084a90[row].height;
    owner->b = func_020176d8(owner, owner->kind, row);
    owner->a = func_02017c70(owner, owner->kind);
    owner->x = data_02084a90[owner->row].x;
    owner->y = data_02084a90[owner->row].y;
    owner->x2 = owner->x;
    owner->c = 0xf6;
    func_0201c6c4(data_020b9df4, owner->model);
    func_0201cf2c(data_020b9df4, owner->shadow);
    func_02016fbc(data_020b9df4, 3);
}
