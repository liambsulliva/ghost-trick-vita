struct Owner {
    unsigned char pad[0x34];
    short base;
    unsigned char pad1[0x3a - 0x36];
    unsigned char scale;
    unsigned char pad2;
    short offset_x;
    short shift_x;
    short d;
    short e;
    short f;
    unsigned char pad3[0x51 - 0x46];
    unsigned char lift;
    unsigned char pad4[0x70 - 0x52];
    void *list;
    int mode;
};
extern char data_020b9df4[];
extern void *func_0201bddc(void *list, unsigned short id);
extern void *func_02018710(void *actor);
extern int func_02019094(void *object);
extern void func_02019098(void *object);
extern int func_02017214(char *object);
extern void func_0201c248(void *actor, int x, int y);

void func_0201966c(struct Owner *owner, int id)
{
    void *actor;
    void *part;
    short x;
    short y;
    actor = func_0201bddc(owner->list, id);
    if (actor == 0) {
        return;
    }
    part = func_02018710(actor);
    if (part != 0) {
        owner->mode = func_02019094(part);
        func_02019098(part);
    }
    x = owner->shift_x + (owner->offset_x + (owner->e + owner->base)) + func_02017214(data_020b9df4);
    y = owner->f + owner->scale * (owner->lift + owner->d) - 15;
    if (owner->mode == 0x100) {
        x = 0;
        y = 0xc;
    }
    func_0201c248(actor, x << 12, y << 12);
}
