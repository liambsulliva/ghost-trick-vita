extern int func_020790c8(int a,int b);

int func_0201ee90(int object,int value)
{
    int node;
    int found;

    node = *(int *)(object + 0xc);
    goto test;
body:
    if (func_020790c8(*(int *)(node + 4),value) == 0) {
        return node;
    }
    found = func_0201ee90(node,value);
    if (found != 0) {
        return found;
    }
    node = *(int *)(node + 8);
test:
    if (node != 0) {
        goto body;
    }
    return 0;
}
