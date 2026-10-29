extern int func_02020254(int object);
extern int func_02020dac(int node);
extern int func_020790c8(int a,int b);

int func_02020d78(int object,int key,int value)
{
    int node;

    node = *(int *)(object + 4);
    goto test;
body:
    if (key == func_02020254(node)) {
        if (func_020790c8(func_02020dac(node),value) == 0) {
            return node;
        }
    }
    node = *(int *)(node + 0x34);
test:
    if (node != 0) {
        goto body;
    }
    return 0;
}
