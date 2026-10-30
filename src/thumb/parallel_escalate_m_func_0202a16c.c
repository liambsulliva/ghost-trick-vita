void func_02007320(unsigned char *param_1);

void func_0202a16c(char *object, unsigned char *param_2)
{
    int i;
    int bits;
    unsigned char *rec;

    if (*(unsigned char *)(object + 0x24) != 1) {
        func_02007320(param_2);
        i = 0;
        do {
            rec = (unsigned char *)(object + 0x1e4 + i * 0xc);
            if (param_2 != 0) {
                rec[1] = 3;
            }
            else {
                bits = *rec;
                bits = bits & ~1;
                *rec = bits;
                rec[1] = 0;
                rec[2] = 0;
            }
            i++;
        } while (i < 8);
    }
}
