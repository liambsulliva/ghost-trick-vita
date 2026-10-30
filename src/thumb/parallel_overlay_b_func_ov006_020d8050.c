struct State {
    unsigned char pad[0xc0];
    short a, b, c, d;
    unsigned char rest[0x14];
    unsigned char rectangle[14];
};
struct Rectangle;
extern void func_02014ddc(void *, int);
extern void func_02014de4(void *, int);
extern void func_ov006_020d7e88(struct Rectangle *, short, short, short, short);
void func_ov006_020d8050(struct State *p, int x, int y)
{
    func_02014ddc(p, x << 12);
    func_02014de4(p, y << 12);
    func_ov006_020d7e88((struct Rectangle *)p->rectangle, p->a + x, p->b + y, p->c + x, p->d + y);
}
