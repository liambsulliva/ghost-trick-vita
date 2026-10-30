struct State {
    unsigned char pad[0xc0];
    unsigned char first;
    unsigned char second;
};
void func_ov006_020d8174(struct State *p, int a, int b)
{
    p->first = a;
    p->second = b;
}
