extern unsigned int func_02015d04(int param_1);
extern const unsigned short data_02084848[];

unsigned int func_0201a9b8(int param_1, unsigned short *text)
{
    unsigned int count;
    unsigned int best;
    unsigned short c;

    count = 0;
    best = 0;
    if (text == 0) {
        return 0;
    }
    for (;;) {
        if (func_02015d04(*text) != 0) {
            if (best > count) {
                count = best;
            }
            break;
        }
        c = *text;
        if (c < 0xff00) {
            count++;
        } else if (c == 0xff01) {
            best = count;
            count = 0;
        } else {
            text += data_02084848[(unsigned short)(c - 0xff00)];
        }
        text++;
    }
    return count;
}
