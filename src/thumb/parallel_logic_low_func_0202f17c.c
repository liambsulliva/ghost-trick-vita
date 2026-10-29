void func_0202f17c(int object,int target)
{
    int node;
    int prev;

    node = *(int *)(object + 0xc);
    prev = 0;
    goto test;
body:
    if (node == target) {
        if (prev == 0) {
            *(int *)(object + 0xc) = *(int *)(node + 4);
            return;
        }
        *(int *)(prev + 4) = *(int *)(node + 4);
        return;
    }
    prev = node;
    node = *(int *)(node + 4);
test:
    if (node != 0) {
        goto body;
    }
}
