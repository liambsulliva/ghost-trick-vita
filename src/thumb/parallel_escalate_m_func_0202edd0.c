extern int func_0202d6a4(int object,int index);

unsigned int func_0202edd0(unsigned char *param_1)
{
    unsigned char kind;
    int object;

    if (param_1[0xc] == 0) {
        return param_1[0];
    }
    kind = param_1[1];
    if (kind == 8) {
        return 2;
    }
    if (kind == 9) {
        return 2;
    }
    if (kind == 10) {
        return 2;
    }
    if (kind == 11) {
        return 1;
    }
    if (kind == 12) {
        return 1;
    }
    object = *(int *)(param_1 + 8);
    if (object != 0) {
        return func_0202d6a4(object,*(int *)(param_1 + 4)) & 0xff;
    }
    return param_1[0];
}
