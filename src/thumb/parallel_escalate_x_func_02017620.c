extern unsigned short data_02084848[];

const unsigned short *func_02017620(void *self, const unsigned short *text)
{
    const unsigned short *found = 0;
    unsigned short c;
    if (text != 0) {
        while ((c = *text) != 0xfffe) {
            if (c >= 0xff00) {
                if (c == 0xff21) {
                    found = text;
                }
                text += data_02084848[(unsigned short)(c - 0xff00)];
            }
            text++;
        }
    }
    return found;
}
