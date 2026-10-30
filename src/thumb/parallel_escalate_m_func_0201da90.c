extern int func_02015d04(int c);
extern const unsigned short data_02084848[];

int func_0201da90(int param_1, unsigned short *text)
{
    unsigned short c;

    for (;;) {
        if (func_02015d04(*text) != 0) {
            return 0;
        }
        c = *text;
        if (c < 0xff00) {
            return 1;
        }
        if (c == 0xff2e) {
            return 1;
        }
        text = text + data_02084848[(unsigned short)(c - 0xff00)] + 1;
    }
}
