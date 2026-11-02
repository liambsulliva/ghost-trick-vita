struct Card {
    unsigned char pad0[0x28];
    unsigned short count;
    unsigned short h2a;
    short h2c;
    signed char cur;
    signed char pick;
    unsigned char b30;
};

extern void func_0205b164(struct Card *p);

void func_0205b110(struct Card *p)
{
    if (p->b30 == 0) {
        if (p->pick > 0) {
            p->pick = p->pick - 1;
        } else {
            p->pick = p->count - 1;
        }
        p->cur = p->pick;
    } else {
        if (p->cur > 0) {
            p->cur = p->cur - 1;
        }
        if (p->pick > 0) {
            p->pick = p->pick - 1;
        }
    }
    func_0205b164(p);
}
