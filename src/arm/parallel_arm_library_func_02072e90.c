void func_02072e90(unsigned short value, char *dest, int size)
{
    int i;

    i = 0;
    while (1) {
        if (i >= size) {
            return;
        }
        *(unsigned short *)(dest + i) = value;
        i = i + 2;
    }
}
