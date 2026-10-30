struct State {
    unsigned char pad[0x27];
    unsigned char flag;
    unsigned char pad2;
    unsigned char value;
};
void func_ov006_020d5778(struct State *p, int value)
{
    p->flag = 1;
    p->value = value;
}
