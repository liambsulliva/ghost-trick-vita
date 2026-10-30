extern char data_020b87a0;

void func_0200dac8(int param)
{
    unsigned int i;

    i = 0;
    for (; i < 0x80; i = i + 1) {
        if (*(int *)(&data_020b87a0 + (i << 2)) == 0) {
            *(int *)(&data_020b87a0 + (i << 2)) = param;
            return;
        }
    }
}
