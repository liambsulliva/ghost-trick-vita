extern int func_02010fe0(int node,int value);

int func_02010fbc(int object,int value)
{
    int node;
    int result;
    int disp;

    disp = 0xfb8;
    result = 0;
    node = *(int *)((char *)object + disp);
    if (node != 0) {
        value = (value << 24) >> 24;
        if (func_02010fe0(node,value) != 0) {
            result = 1;
        }
    }
    return result;
}
