extern int func_02010194(int object);
extern void func_020589ec(char *object);
extern void func_020589e0(char *object);

void func_020100f4(char *param_1, unsigned int param_2)
{
    char *owner;
    unsigned int kind;
    int offset;

    owner = (char *)func_02010194((int)param_1);
    if (owner != 0) {
        func_020589ec(owner);
    }
    kind = (((unsigned int)*(unsigned int *)(param_1 + 0xc)) << 17) >> 30;
    if (kind == 1) {
        offset = 0xe44;
    } else if (kind == 2) {
        offset = 0xe48;
    } else {
        offset = 0xe40;
    }
    *(unsigned int *)(param_1 + offset) = param_2;
    owner = (char *)func_02010194((int)param_1);
    if (owner != 0) {
        func_020589e0(owner);
    }
}
