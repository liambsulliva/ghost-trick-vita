struct Voice {
    unsigned char pad0[0x46];
    unsigned short h46;
    unsigned short h48;
    unsigned char b4a;
};

void func_02054254(struct Voice *v, int value)
{
    if (value == 0xffff) {
        v->b4a = 1;
        value = 0xf;
    }
    v->h46 = 1;
    v->h48 = value;
}
