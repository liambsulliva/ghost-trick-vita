#pragma opt_propagation off
struct Text {
    int start;
    union {
        unsigned short *codes;
        int addr;
    } cursor;
    int active;
    char pad0c[0xc];
    unsigned short code;
    char pad1a[3];
    unsigned char lines;
    char pad1e[0x20];
    unsigned short last;
};

extern unsigned short data_02084848[];
extern unsigned int func_02015cc4(int code);
extern int func_0201c710(struct Text *text);

void func_0201c760(struct Text *text)
{
    int off;
    if (text->active == 0 || text->cursor.codes == 0) {
        return;
    }
    while (1) {
        text->code = *text->cursor.codes;
        if (text->code > 0xff00) {
            if (func_02015cc4(text->code) != 0) {
                return;
            }
            off = data_02084848[(unsigned short)(*text->cursor.codes - 0xff00)];
            text->cursor.codes += off;
        } else {
            switch (func_0201c710(text)) {
            case 0:
                text->lines++;
                break;
            case 2:
                return;
            default:
                return;
            }
        }
        text->last = text->code;
        text->cursor.codes++;
    }
}
