struct Fade;
extern void func_0201c350(struct Fade *fade);
extern unsigned short *func_0201c478(struct Fade *fade);
extern int func_0201c47c(struct Fade *fade);
extern void func_0201c04c(int target, int level);

struct FadeView {
    unsigned char pad[0x28];
    int target;
};

void func_0201c4a0(struct Fade *fade)
{
    unsigned short *pal;
    unsigned int k;
    unsigned int i;
    unsigned short c;
    unsigned short r;
    unsigned short g;
    unsigned short b;

    func_0201c350(fade);
    pal = func_0201c478(fade);
    if (pal != 0) {
        k = func_0201c47c(fade);
        for (i = 1; i < 0x10; i++) {
            c = pal[i];
            r = (c & 0x1f) << 3;
            g = ((c & 0x3e0) >> 5) << 3;
            b = ((c & 0x7c00) >> 10) << 3;
            r = 0xff - (((0xff - r) * k) >> 4);
            g = 0xff - (((0xff - g) * k) >> 4);
            b = 0xff - (((0xff - b) * k) >> 4);
            pal[i] = (b >> 3) << 10 | ((r >> 3) | (g >> 3) << 5);
        }
        if (((struct FadeView *)fade)->target != 0) {
            func_0201c04c(((struct FadeView *)fade)->target, (signed char)((k * 0x1f) >> 4));
        }
    }
}
