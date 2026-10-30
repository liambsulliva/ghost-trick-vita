struct State { int pad[8]; int a; int pad2; int b; };

unsigned int func_ov004_020d4698(struct State *p)
{
    if (p->a == 0) {
        return 0;
    }
    return p->b != 0 ? 0x80 : 0x100;
}
