struct Rectangle {
    unsigned char pad[6];
    short a;
    short b;
    short c;
    short d;
};
void func_ov006_020d7e88(struct Rectangle *p, short a, short b, short c, short d)
{
    p->a = a;
    p->b = b;
    p->c = c;
    p->d = d;
}
