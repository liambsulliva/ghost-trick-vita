extern int func_0202fc60(int object);

int func_0202fd20(int object,int index)
{
    int node;
    int i;

    node = *(int *)(object + 4);
    i = 0;
    while (node != 0) {
        if (i == index) {
            return node;
        }
        i = i + 1;
        node = func_0202fc60(node);
    }
    return 0;
}
