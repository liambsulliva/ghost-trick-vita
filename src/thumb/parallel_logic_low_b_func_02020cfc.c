int func_02020cfc(int *object)
{
    int node;
    int next;

    node = object[1];
    goto test;
body:
    next = *(int *)(node + 0x34);
    if (next == 0) {
        return node;
    }
    node = next;
test:
    if (node != 0) {
        goto body;
    }
    return 0;
}
