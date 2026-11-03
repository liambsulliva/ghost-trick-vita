class Obj0205b164 {
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
    virtual void m15(int a);
};

extern "C" {
extern char *func_0205b420(char *param_1, int param_2, unsigned char param_3);
extern void *func_0202f624(int size);
extern unsigned int func_0202f64c(int *param_1);
extern void func_0202f7e8(int param_1, int param_2, unsigned int param_3, int param_4);
extern void func_0205b014(char *object, int a, int b);
extern unsigned char *func_02009900(void);
extern void *func_0200847c(void);
extern unsigned int func_0202a0c0(int param_1, int param_2);
extern void func_0205ba1c(int param_1, int param_2);

void func_0205b164(char *obj)
{
    int i;
    signed char c;
    char *item;
    Obj0205b164 *view;
    int t;

    view = *(Obj0205b164 **)(obj + 0x40);
    t = (1 - *(signed char *)(obj + 0x2f)) * 0x2a;
    view->m15((t + *(short *)(obj + 0x2c)) << 0xc);
    for (i = 0; i < *(unsigned short *)(obj + 0x28); i++) {
        c = (signed char)(i - (*(signed char *)(obj + 0x2e) - *(signed char *)(obj + 0x2f)));
        if (*(unsigned char *)(obj + 0x30) != 0) {
            if (c < 0 || (int)(*(unsigned short *)(obj + 0x2a) - 1) < c) {
                func_0202f64c((int *)((int)obj + 0x44) + i);
            } else if (*(int *)(obj + (i << 2) + 0x44) == 0) {
                item = (char *)func_0202f624(0xa4);
                if (item != 0) {
                    item = func_0205b420(item, (unsigned char)i, *(unsigned char *)(obj + 0x30));
                }
                *(char **)(obj + (i << 2) + 0x44) = item;
                func_0205b014(item, 0, *(short *)(obj + 0x2c));
                func_0202f7e8((int)func_02009900(), *(int *)(obj + (i << 2) + 0x44), 1, 0);
            }
        }
        if (*(int *)(obj + (i << 2) + 0x44) != 0) {
            func_0205ba1c(*(int *)(obj + (i << 2) + 0x44), c);
        }
    }
    func_0202a0c0((int)func_0200847c(), 1);
    *(unsigned char *)(obj + 0x13) = 8;
}
}
