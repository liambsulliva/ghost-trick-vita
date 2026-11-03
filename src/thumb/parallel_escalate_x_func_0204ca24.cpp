class Part {
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
    virtual void s12(int *pos);
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
    virtual void s33(int value);
    virtual void s34();
    virtual void s35(signed char level);
};

struct Frame {
    int vtable;
    char pad04[0x24];
    int x;
    int y;
    char pad30[0x86];
    short level;
    Part *parts[4];
};

struct Holder {
    int level;
    struct Frame *object;
};

extern "C" {
extern struct Holder data_020ceaa4;

void func_0204ca24(struct Frame *frame)
{
    if (frame->level < 0) {
        frame->level = 0x1a;
    }
    frame->parts[0]->s35(frame->level);
    frame->parts[1]->s35(frame->level);
    frame->parts[2]->s35(frame->level);
    frame->parts[3]->s35(frame->level);
    frame->parts[0]->s12(&frame->x);
    frame->parts[1]->s13(frame->x - 0xcb000, frame->y, 0);
    frame->parts[2]->s13(frame->x + 0xcd000, frame->y, 0);
    frame->parts[3]->s13(frame->x + 0xdc000, frame->y, 0);
    if (data_020ceaa4.object == 0) {
        data_020ceaa4.object = frame;
    }
}
}
