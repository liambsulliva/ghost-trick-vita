extern unsigned short data_020c9d6c[];

void func_020229c4(unsigned int mask, unsigned short value)
{
    int index;

    index = 0;
    while (index < 32) {
        if (mask & (1u << index)) {
            data_020c9d6c[index] = value;
            return;
        }
        index++;
    }
}
