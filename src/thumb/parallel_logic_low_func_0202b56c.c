typedef int code();

void func_0202b56c(int object)
{
    int node;
    int current;

    node = *(int *)(object + 0x74);
    while (node != 0) {
        current = node;
        node = *(int *)(node + 0x78);
        if (current != 0) {
            if (current != 0) {
                (**(code **)(*(int *)current + 4))(current);
            }
        }
    }
}
