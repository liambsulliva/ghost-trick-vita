class Actor {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void slot8();
    virtual void slot9();
    virtual void slot10();
    virtual void slot11();
    virtual void slot12();
    virtual int raised();
};
struct Entry {
    short x;
    short y;
    short pad;
    short height;
    short rest[5];
};
struct Owner {
    Actor actor;
    unsigned char pad[0x10 - 4];
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
extern "C" {
extern struct Entry data_02084a90[];
extern char data_020b9df4[];
short func_020176d8(Owner *owner, int kind, int row);
short func_02017c70(Owner *owner, unsigned char kind);
void func_0201c6c4(char *object, void *model);
void func_0201cf2c(char *object, void *shadow);
void func_02016fbc(char *object, int value);

void func_02019134(Owner *owner)
{
    unsigned short row;
    owner->row = 4;
    row = owner->row;
    owner->limit = 0;
    owner->height = data_02084a90[row].height;
    owner->speed = 0x30;
    owner->b = func_020176d8(owner, owner->kind, row);
    owner->a = func_02017c70(owner, owner->kind);
    owner->x = data_02084a90[owner->row].x;
    owner->y = data_02084a90[owner->row].y;
    owner->x2 = owner->x - owner->speed;
    owner->c = 0xc6;
    if (owner->actor.raised() != 0) {
        owner->speed = owner->speed - 0x18;
    }
    func_0201c6c4(data_020b9df4, owner->model);
    func_0201cf2c(data_020b9df4, owner->shadow);
    func_02016fbc(data_020b9df4, 3);
}
}
