#pragma opt_propagation off
class Window {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7();
    virtual void s8();
    virtual void s9();
    virtual void s10();
    virtual void s11();
    virtual void s12();
    virtual void s13();
    virtual void s14();
    virtual void s15();
    virtual void s16();
    virtual void s17();
    virtual void s18();
    virtual void s19();
    virtual void s20();
    virtual void s21();
    virtual void s22();
    virtual void s23();
    virtual void s24();
    virtual void s25();
    virtual void s26();
    virtual void s27();
    virtual void s28();
    virtual void s29();
    virtual void s30();
    virtual void s31();
    virtual void s32();
    virtual void s33();
    virtual void s34();
    virtual void s35();
    virtual void s36();
    virtual unsigned int s37();
};

struct FrameInfo {
    unsigned char edge_w;
    unsigned char rows;
    unsigned char mid_w;
    unsigned char tip_w;
    unsigned char tip_h;
};

struct WindowData {
    char pad00[0x30];
    short width;
    short top;
    char pad34[4];
    unsigned short style;
    char pad3a[0x32];
    unsigned char *left[2];
    unsigned char *middle[2];
    unsigned char *right[2];
    unsigned char *tip[2];
    char pad8c[4];
    int side;
};

extern "C" {
extern struct FrameInfo data_02084a48[];
extern void func_0207303c(void *src, void *dst, unsigned int size);
void func_0201a680(Window *window, unsigned char *dst, unsigned char count);
}

extern "C" void func_0201a680(Window *window, unsigned char *dst, unsigned char count)
{
    struct WindowData *self;
    unsigned int kind;
    unsigned int pitch;
    unsigned char gap;
    unsigned char j;
    unsigned char rows;
    unsigned char tip_h;
    unsigned char start2;
    unsigned char tip_w;
    unsigned char mid_w;
    unsigned char top;
    unsigned char x;
    unsigned char edge_w;
    unsigned char i;
    unsigned char k;
    unsigned char k2;
    int sd;
    unsigned char half;
    unsigned char start;
    unsigned char y;
    signed char m;
    unsigned char v;
    unsigned int row;
    int last;

    self = (struct WindowData *)window;
    x = self->width / 2;
    top = self->top;
    kind = 0;
    if (self->style > 1 && self->style < 4) {
        kind = 1;
    }
    if (kind > 1) {
        return;
    }
    edge_w = data_02084a48[kind].edge_w;
    rows = data_02084a48[kind].rows;
    mid_w = data_02084a48[kind].mid_w;
    tip_w = data_02084a48[kind].tip_w;
    tip_h = data_02084a48[kind].tip_h;
    pitch = window->s37() >> 1;
    gap = 0;
    for (i = 0; i < rows; i++) {
        func_0207303c(self->left[kind] + i * edge_w, dst + (top + i) * pitch + x, edge_w);
    }
    x += edge_w;
    if (self->side == 1) {
        gap = x - 8;
    }
    for (j = 0; j < count; j++) {
        for (i = 0; i < rows; i++) {
            func_0207303c(self->middle[kind] + i * edge_w, dst + (top + i) * pitch + x, mid_w);
        }
        x += mid_w;
    }
    if (self->side == 0) {
        gap = x - 8;
    }
    for (i = 0; i < rows; i++) {
        func_0207303c(self->right[kind] + i * edge_w, dst + (top + i) * pitch + x, edge_w);
    }
    y = top + rows;
    half = mid_w * (count - 1) / 2;
    sd = self->side;
    if (sd == 1) {
        start = gap + half;
        for (k = 0; k < tip_h; k++) {
            func_0207303c(self->tip[kind] + k * tip_w, dst + (y + k) * pitch + start, tip_w);
        }
    } else if (sd == 0) {
        start2 = gap - half;
        last = tip_w - 1;
        for (k2 = 0; k2 < tip_h; k2++) {
            row = k2 * tip_w;
            for (m = last; m >= 0; m--) {
                v = self->tip[kind][row + m];
                *(dst + (y + k2) * pitch + start2 + last - m) = (unsigned char)(v << 4) | (unsigned char)(v >> 4);
            }
        }
    }
}

