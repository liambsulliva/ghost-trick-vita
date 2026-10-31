struct Target {
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3(int a, int b);
};

extern "C" {
extern char data_020cf898[];
void func_0202fbcc(unsigned char *buf, int n);
int func_0200eac4(unsigned char *buf);
int func_0200eab8(unsigned char *buf);
int func_0200eab4(unsigned char *buf);
void func_0200eabc(unsigned char *buf, int n);
int func_02026c10(int p);
int func_02008828(int p, char *q);
int func_02014220(int p);
int func_02078fbc(int p, signed char *q);
int func_02028148(int p);
int func_02027c3c(signed char *p);
}

extern "C" void func_02026b40(Target *self, int param_2, signed char *param_3)
{
    signed char c;
    int item;
    unsigned char buf[16];

    func_0202fbcc(buf, 6);
    c = *param_3;
    if (c == '*') {
        while (func_0200eac4(buf) != 0) {
            item = func_0200eab8(buf);
            self->slot3(item, func_02026c10(param_2));
            func_0200eabc(buf, 0);
        }
    }
    else if (c == '@') {
        while (func_0200eac4(buf) != 0) {
            if (func_02008828(func_0200eab4(buf), data_020cf898) != 0) {
                item = func_0200eab8(buf);
                if (func_02078fbc(func_02014220(item), param_3 + 1) != 0) {
                    self->slot3(item, func_02026c10(param_2));
                }
            }
            func_0200eabc(buf, 0);
        }
    }
    else {
        if (c == '?') {
            item = func_02028148(param_2);
        }
        else {
            item = func_02027c3c(param_3);
        }
        self->slot3(item, func_02026c10(param_2));
    }
}
