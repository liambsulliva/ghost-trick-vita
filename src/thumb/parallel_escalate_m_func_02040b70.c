struct Widths02040b70 {
    unsigned char w[4];
};

extern struct Widths02040b70 data_0209b154;
extern unsigned short data_0209b1a2[];
extern char *func_02078e84(char *destp, const signed char *srcp);
extern void func_02040c14(unsigned char *object, signed char *text);
extern void func_020415b0(int screen, int glyph, int x, int y, int w, int flag);

void func_02040b70(unsigned char *p)
{
    signed char text[8];
    struct Widths02040b70 widths;
    unsigned int i;
    unsigned int digit;
    unsigned short glyph;

    func_02078e84((char *)text, (const signed char *)(p + 0x54));
    func_02040c14(p, text);
    widths = data_0209b154;
    for (i = 0; i < 4; i++) {
        digit = (unsigned char)(text[i] - 0x30);
        if (digit < 10) {
            glyph = data_0209b1a2[digit];
            if (digit != 0 || i != 0) {
                func_020415b0(*(int *)(p + 0x28), glyph, 6, 0x14, widths.w[i], 0);
            }
        }
    }
    if (p[0x59] != 0) {
        func_020415b0(*(int *)(p + 0x28), 0x12d3, 6, 0x14, 0x58, 0);
    }
    else {
        func_020415b0(*(int *)(p + 0x28), 0x12d4, 6, 0x14, 0x58, 0);
    }
}
