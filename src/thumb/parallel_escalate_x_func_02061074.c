struct Obj02061074 {
    char pad0[0x7c];
    unsigned char low : 5;
    unsigned char bit5 : 1;
    unsigned char bit6 : 1;
    unsigned char bit7 : 1;
    char pad7d;
    unsigned char active;
    char pad7f[0x6d];
    unsigned short count;
};

int func_02061074(struct Obj02061074 *param_1)
{
    if (param_1->count == 0) {
        return 0;
    }
    if (param_1->bit5 == 0) {
        return 0;
    }
    if (param_1->active != 0) {
        return 1;
    }
    if (param_1->bit6 != 0) {
        return 1;
    }
    return 0;
}
