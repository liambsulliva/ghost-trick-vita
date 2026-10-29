extern int func_02020dac(int node);
extern int func_020790c8(int a,int b);

int func_02020db0(int object,int value)
{
    int node;
    int which;

    which = value;
    node = *(int *)(object + 4);
    goto test;
body:
    if (func_020790c8(func_02020dac(node),which) == 0) {
        return node;
    }
    node = *(int *)(node + 0x34);
test:
    if (node != 0) {
        goto body;
    }
    return 0;
}
