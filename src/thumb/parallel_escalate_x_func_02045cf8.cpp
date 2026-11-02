struct ViewFlags {
    unsigned char b0 : 1;
    unsigned char b1 : 1;
    unsigned char hidden : 1;
};

struct SlideFlags {
    unsigned char busy : 1;
    unsigned char b1 : 1;
    unsigned char b2 : 1;
    unsigned char dir : 2;
    unsigned char mode : 3;
};

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
    virtual void s13(int x, int y, int z);
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
    virtual void s37();
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void s42();
    virtual void s43();
    virtual void s44();
    virtual void s45();
    virtual void s46();
    virtual void s47();
    virtual void s48();
    virtual void s49();
    virtual void s50();
    virtual void s51();
    virtual void s52();
    virtual void s53();
    virtual void s54();
    unsigned char pad04[0xc];
    unsigned char state;
    unsigned char pad11[0xb6];
    unsigned char lock;
    unsigned char padc8[0x18];
    ViewFlags view;
    unsigned char pade1[0x4f];
    SlideFlags slide;
    unsigned char pad131[0x17];
    int x;
    int y;
    unsigned char pad150[8];
    int wait;
    unsigned char pad15c[4];
    int handle;
};

extern "C" {
extern void *func_02008444(void);
extern int func_02031af4(void *system, int handle);
extern void func_02031b04(void *system, int *handle);
extern int func_02010fbc(void *system, int a);
extern int func_02010b38(void *system);
extern int func_0201132c(void *system);
extern int func_02011320(void *system);
extern int func_0203435c(void *system);
extern int func_02045ec4(void *system);
extern void func_02010384(void *system, int a);
extern void func_02045ed4(Window *w);
void func_02045cf8(Window *w);
}

void func_02045cf8(Window *w)
{
    if (func_02031af4(func_02008444(), w->handle) != 0) {
        w->wait = 0;
        func_02031b04(func_02008444(), &w->handle);
    }
    if (w->slide.busy) {
        if (func_02010fbc(func_02008444(), 0) == 0 || func_02010b38(func_02008444()) != 0) {
            return;
        }
        w->slide.busy = 0;
        if (w->slide.mode == 0) {
            if (func_0201132c(func_02008444()) != 0) {
                w->x = -0x4000;
                w->y = -0x53000;
            } else {
                w->x = 0x34000;
                w->y = -0x56000;
            }
            w->s13(w->x, w->y, 0);
            w->slide.mode = 1;
            w->slide.dir = 2;
        }
    }
    if (w->slide.busy) {
        return;
    }
    switch (w->state) {
    case 0:
        if (func_02011320(func_02008444()) != 3) {
            return;
        }
        if (func_0203435c(func_02008444()) != 0) {
            return;
        }
        if (func_02045ec4(func_02008444()) != 0) {
            return;
        }
        w->state = 1;
        w->s53();
        if (w->slide.mode == 3 && !w->view.hidden && w->lock == 0) {
            func_02010384(func_02008444(), 1);
        }
    case 1:
        if (func_02011320(func_02008444()) != 3 || func_0203435c(func_02008444()) != 0) {
            w->state = 3;
            w->s54();
        } else {
            func_02045ed4(w);
        }
        break;
    case 2:
        w->state = 1;
        break;
    case 3:
        w->state = 0;
        w->s54();
        if (w->slide.mode == 3 && !w->view.hidden && w->lock == 0) {
            func_02010384(func_02008444(), 0);
        }
    case 4:
        w->s54();
        break;
    }
}
