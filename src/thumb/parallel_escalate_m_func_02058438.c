void func_0205813c(unsigned char *p, unsigned char first, unsigned char second, int up);

void func_02058438(unsigned char *p, unsigned int a, unsigned int b, unsigned int high, unsigned int low, int offset, unsigned char raise_flag, unsigned char up_flag)
{
    int raised;

    p[0xdf] = b;
    p[0xe0] = a;
    raised = 0;
    if (p[0xdf] < low) {
        p[0xdf] = low;
        raised = 1;
    }
    if (p[0xdf] > high) {
        p[0xdf] = high;
    }
    if (p[0xe0] < low) {
        p[0xe0] = low;
    }
    if (p[0xe0] > high) {
        p[0xe0] = high;
    }
    if (a < b) {
        if (up_flag != 0) {
            p[0xe1] = 1;
        }
        func_0205813c(p, p[0xe0] - offset, p[0xdf] - offset, 1);
    }
    else {
        func_0205813c(p, p[0xe0] - offset, p[0xdf] - offset, 0);
    }
    *(short *)(p + 0xdc) = 0;
    if (raised != 0 && raise_flag != 0) {
        p[0xe1] = 1;
    }
}
