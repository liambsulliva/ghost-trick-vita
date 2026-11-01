extern unsigned short data_02084848[];

int func_020175dc(void *self, const unsigned short *text)
{
    const unsigned short *lengths;
    unsigned short c;
    if (text != 0) {
        lengths = data_02084848;
        while ((c = *text) != 0xfffe) {
            if (c == 0xff21) {
                return 1;
            }
            if (c >= 0xff00) {
                text += lengths[(unsigned short)(c - 0xff00)];
            }
            text++;
        }
    }
    return 0;
}
