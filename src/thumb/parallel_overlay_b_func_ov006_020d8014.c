struct State {
    unsigned char pad[0xc0];
    short a;
    short b;
    short c;
    short d;
    unsigned char rest[0x14];
    unsigned char rectangle[14];
};
struct Rectangle;
extern void func_ov006_020d7e88(struct Rectangle *, short, short, short, short);
void func_ov006_020d8014(struct State *p, short a, short b, short c, short d)
{
    p->a = a;
    p->b = b;
    p->c = c;
    p->d = d;
    func_ov006_020d7e88((struct Rectangle *)p->rectangle, p->a, p->b, p->c, p->d);
}
