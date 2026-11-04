class Item {
public:
    virtual void f0();
    virtual void f1();
    virtual void f2();
    virtual void f3();
    virtual void f4();
    virtual void f5();
    virtual void f6();
    virtual void f7();
    virtual void f8();
    virtual void f9();
    virtual void f10();
    virtual void f11();
    virtual void f12();
    virtual void f13();
    virtual void f14();
    virtual void f15();
    virtual void f16();
    virtual void f17();
    virtual void f18();
    virtual void f19();
    virtual void f20();
    virtual void f21();
    virtual void f22();
    virtual void f23();
    virtual void f24();
    virtual void f25();
    virtual void f26();
    virtual void f27();
    virtual void f28();
    virtual void f29();
    virtual void f30();
    virtual void f31();
    virtual void next();
    virtual void f33();
    virtual void f34();
    virtual void f35();
    virtual void f36();
    virtual void set(int value);
    virtual int get();
};

extern "C" {
void func_02022b34(unsigned short *state)
{
    int value;
    unsigned int i;
    int next = 1;
    for (i = 0; i < state[0]; i++) {
        unsigned short *index = (unsigned short *)((char *)state + i * 2);
        value = (*(Item **)((char *)state + index[0x504] * 4 + 8))->get();
        if (value < 0x30) {
            (*(Item **)((char *)state + index[0x504] * 4 + 8))->set(next);
            next = (signed char)(next + 1);
            if (next >= 0x30) {
                next = 1;
            }
        }
        (*(Item **)((char *)state + index[0x504] * 4 + 8))->next();
        (*(Item **)((char *)state + index[0x504] * 4 + 8))->set((signed char)value);
    }
    state[0] = 0;
    state[2] = state[1];
}
}
