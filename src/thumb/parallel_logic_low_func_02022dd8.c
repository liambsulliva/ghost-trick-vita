extern unsigned int func_02022f04(int object,int index);

unsigned int func_02022dd8(int object)
{
    int count;

    count = *(int *)(object + 0x14);
    if (count != 0) {
        return func_02022f04(*(int *)(object + 0x1c),count - 1);
    }
    return 0;
}
