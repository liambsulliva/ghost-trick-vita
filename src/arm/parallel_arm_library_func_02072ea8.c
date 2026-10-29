void func_02072ea8(char *src, char *dest, int size)
{
    int i;

    i = 0;
    while (1) {
        if (i >= size) {
            return;
        }
        *(unsigned short *)(dest + i) = *(unsigned short *)(src + i);
        i = i + 2;
    }
}
