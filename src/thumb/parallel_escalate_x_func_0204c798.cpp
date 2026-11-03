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
    virtual void s46(int value);
};

struct Frame {
    int vtable;
    char pad04[0x24];
    int x;
    int y;
    char pad30[0x88];
    Part *parts[4];
};

extern "C" {
extern int data_020aad20;
extern char data_020cfc30;
extern void func_02047120(struct Frame *frame);
extern Part *func_02061cec(void *type);
extern void func_0204c640(Part *part, int a, int b, int c, int d);
extern void func_0204c540(Part *part, int frame);
extern void *func_02009900(void);
extern void func_0202f7e8(void *a, Part *b, int c, int d);
extern void func_020153d8(Part *part, struct Frame *frame);

struct Frame *func_0204c798(struct Frame *frame)
{
    Part *part;

    func_02047120(frame);
    frame->vtable = (int)&data_020aad20;
    frame->parts[0] = 0;
    frame->parts[1] = 0;
    frame->parts[2] = 0;
    frame->parts[3] = 0;
    part = func_02061cec(&data_020cfc30);
    frame->parts[0] = part;
    part->s12(&frame->x);
    frame->parts[0]->s46(0);
    func_0204c640(frame->parts[0], -0xb9000, 0x4b000, 0xbf000, -0x50000);
    frame->parts[0]->s33(0x610);
    func_0204c540(frame->parts[0], 0x610);
    func_0202f7e8(func_02009900(), frame->parts[0], 7, 0);
    func_020153d8(frame->parts[0], frame);
    part = func_02061cec(&data_020cfc30);
    frame->parts[1] = part;
    part->s13(frame->x - 0xcb000, frame->y, 0);
    frame->parts[1]->s46(0);
    func_0204c640(frame->parts[1], -0x11000, 0x3c000, 0x12000, -0x50000);
    frame->parts[1]->s33(0x610);
    func_0204c540(frame->parts[1], 0x610);
    func_0202f7e8(func_02009900(), frame->parts[1], 7, 0);
    func_020153d8(frame->parts[1], frame);
    part = func_02061cec(&data_020cfc30);
    frame->parts[2] = part;
    part->s13(frame->x + 0xcd000, frame->y, 0);
    frame->parts[2]->s46(0);
    func_0204c640(frame->parts[2], -0xe000, 0x3b000, 0xe000, -0x50000);
    frame->parts[2]->s33(0x610);
    func_0204c540(frame->parts[2], 0x610);
    func_0202f7e8(func_02009900(), frame->parts[2], 7, 0);
    func_020153d8(frame->parts[2], frame);
    part = func_02061cec(&data_020cfc30);
    frame->parts[3] = part;
    part->s13(frame->x + 0xdc000, frame->y, 0);
    frame->parts[3]->s46(0);
    func_0204c640(frame->parts[3], -0x1000, 0x1a000, 0x2000, -0x50000);
    frame->parts[3]->s33(0x610);
    func_0204c540(frame->parts[3], 0x610);
    func_0202f7e8(func_02009900(), frame->parts[3], 7, 0);
    func_020153d8(frame->parts[3], frame);
    return frame;
}
}
