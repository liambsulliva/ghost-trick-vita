struct State {
    unsigned char pad[0xd0];
    int a;
    int b;
    int c;
};
void func_ov006_020d8000(struct State *p, int a, int b, int c)
{
    p->a = a;
    p->b = b;
    p->c = c;
}
