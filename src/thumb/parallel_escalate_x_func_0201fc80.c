extern int func_0201f934(int object, unsigned int index);
int func_0201fc80(int object, unsigned int index)
{
    int result;
    int slot;
    if (func_0201f934(object, index) == 0) {
        result = 0;
    } else {
        result = 0;
        slot = *(int *)(*(int *)(object + 0xc) + index * 4);
        if (slot != 0 && (((unsigned int)*(unsigned char *)(slot + 6)) << 29) >> 31 != 0) {
            result = 1;
        }
    }
    return result;
}
