struct Obj02033124 {
    char pad[0x7d];
    unsigned char flag : 1;
};

unsigned int func_02033124(struct Obj02033124 *param_1)
{
    if (param_1->flag == 1) {
        return 1;
    }
    return 0;
}
