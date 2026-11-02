class Label {
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
    virtual void s33(int a);
    virtual void s34();
    virtual void s35();
    virtual void s36();
    virtual void s37(int a);
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void s42();
    virtual void s43();
    virtual void s44(void *blit);
    virtual void s45();
    virtual void s46(int w, int h);
};

class Window : public Label {
public:
    char pad4[0xc4];
    int screen;
    void *blit;
    Label *frame;
};

extern "C" {
extern char data_020cf724[];
extern void *func_02031254(int size);
extern void *func_02042718(void *object);
extern void *func_0201003c(void);
extern void func_02015c6c(void *object, int screen, int flag);
extern void *func_02015c7c(void *object);
extern void *func_02019cd8(void *object);
extern void func_02042820(void *blit, void *dst, unsigned int size, unsigned int count, unsigned char mode, int flag, unsigned int id, int request);
extern void *func_020556d4(void *object);
extern void func_020428f0(void *blit, void *data, int size, int p3, int p4, unsigned short c, int d, int f, int g);
extern void func_02029014(void *blit, int value);
extern void func_02029018(void *blit, int value);
extern void func_0202901c(void *blit, int value);
extern void func_0205765c(Window *window, int a, int b, int c, int d);
extern Label *func_0205693c(char *name);
extern void func_0200ee34(Label *label, int a, int b, int c);
extern void func_020153d8(Label *label, Window *window);
extern void *func_02009900(void);
extern void func_0202f7e8(void *a, void *b, int c, int d);
void func_02067b24(Window *window);
}

void func_02067b24(Window *window)
{
    void *blit;
    void *screen;

    blit = func_02031254(0x54);
    if (blit != 0) {
        blit = func_02042718(blit);
    }
    window->blit = blit;
    func_02015c6c(func_0201003c(), window->screen, 0);
    screen = func_02015c7c(func_0201003c());
    func_02042820(window->blit, func_02019cd8(screen), 0x6000, 3, 1, 0, 0xffff, -1);
    func_020428f0(window->blit, func_020556d4(screen), 0x20, 3, 0, 0xffff, 0, -1, 0x100);
    func_02029014(window->blit, 5);
    func_02029018(window->blit, 5);
    func_0202901c(window->blit, 3);
    window->s44(window->blit);
    func_0205765c(window, 0, 0, 0x100000, 0xc0000);
    window->s46(0x100000, 0xc0000);
    window->s33(0x20);
    window->s37(0x3f);
    (window->frame = func_0205693c(data_020cf724))->s46(0x100000, 0xc0000);
    func_0200ee34(window->frame, 0, 0, 0);
    window->frame->s37(0x3e);
    window->frame->s33(0x30);
    func_020153d8(window->frame, window);
    func_0202f7e8(func_02009900(), window->frame, 1, 0);
}
