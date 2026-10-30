extern int func_02010fa8(int node,int value);

int func_02010f84(int object,int value)
{
    int node;
    int result;
    int disp;

    disp = 0xfb8;
    result = 0;
    node = *(int *)((char *)object + disp);
    if (node != 0) {
        value = (value << 24) >> 24;
        if (func_02010fa8(node,value) != 0) {
            result = 1;
        }
    }
    return result;
}
