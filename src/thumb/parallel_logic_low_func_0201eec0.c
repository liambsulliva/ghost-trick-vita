int func_0201eec0(int object,int value)
{
    int node;
    int found;

    node = *(int *)(object + 0xc);
    while (node != 0) {
        if (*(int *)(node + 0x18) == value) {
            return node;
        }
        found = func_0201eec0(node,value);
        if (found != 0) {
            return found;
        }
        node = *(int *)(node + 8);
    }
    return 0;
}
