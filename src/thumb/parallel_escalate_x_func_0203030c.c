extern char *data_020a5574;

void func_0203030c(int param_1, int param_2, int param_3, short param_4, unsigned int param_5)
{
    unsigned char *src;
    unsigned short *dst;
    short i;
    int row;
    int addr;
    unsigned int count;

    src = (unsigned char *)(data_020a5574 + 0x20);
    count = (unsigned int)(long long)param_5;
    if (param_1 == 0) {
        row = 0x12;
    } else {
        row = 0x92;
    }
    addr = (int)data_020a5574 + row * 0x10 + param_3 * 0x40;
    dst = (unsigned short *)addr + param_2;
    for (i = 0; i < count; i++) {
        if (*src == 0) {
            break;
        }
        *dst = 0xf000;
        if (*src >= 0x20 && *src <= 0x7f) {
            *dst += (unsigned short)(*src + param_4 * 0x60 - 0x20);
        }
        dst++;
        src++;
    }
}
