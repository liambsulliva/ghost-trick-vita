struct Obj {
    char pad0[0x10];
    unsigned char state;
    char pad11[0xdc];
    unsigned char level;
};

void func_0203b218(struct Obj *p)
{
    if (p->state == 5) {
        if (p->level < 5) {
            p->level++;
        }
    }
}
