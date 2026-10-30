struct Owner {
    unsigned char pad[0x14];
    unsigned short *script;
    unsigned char pad1[0x34 - 0x18];
    short a;
    short b;
    unsigned char pad2[0x3a - 0x38];
    unsigned char c;
    unsigned char d;
    short e;
    unsigned char pad3[0x44 - 0x3e];
    unsigned short value;
    unsigned char pad4[0x5e - 0x46];
    unsigned short wait;
    unsigned char pad5[0x6c - 0x60];
    void *part;
    unsigned char step;
    unsigned char pad6[2];
    signed char start;
    unsigned char flag;
};
extern void func_0201e060(void *part, int step);
extern void func_02017760(struct Owner *owner);
extern void func_0201e04c(void *part, int step, int value, int flag);
extern void func_02017310(struct Owner *owner);
extern void func_02017110(struct Owner *owner, unsigned short *script);

int func_02019964(struct Owner *owner)
{
    func_0201e060(owner->part, owner->step);
    owner->wait = 0xfffe;
    owner->c = 0;
    owner->d = 0;
    owner->e = 0;
    owner->a = 0;
    owner->b = 0;
    owner->value = owner->start;
    owner->flag = 0;
    func_02017760(owner);
    owner->step++;
    if (owner->step > 2) {
        while (*owner->script != 0xfffe) {
            owner->script++;
        }
        return 1;
    }
    func_0201e04c(owner->part, owner->step, owner->value, 0);
    func_02017310(owner);
    func_02017110(owner, owner->script);
    return 0;
}
