class Item {
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
    virtual int s24();
    virtual int s25();
};

class Fade {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7();
    virtual void s8(int a);
    unsigned char pad4[0x44];
    void *res;
    short hold;
    short fall;
    short rise;
    short step;
    unsigned char keep;
    unsigned char ready;
    unsigned char shown;
    unsigned char active;
    int level;
};

extern "C" {
extern char data_020cf898[];
extern void *func_02029640(Fade *p);
extern Item *func_020082f8(void *a, char *b);
extern int func_02010b54(Item *a);
extern void func_02029028(void *a);
void func_020294f0(Fade *p);
}

void func_020294f0(Fade *p)
{
    Item *it;
    int v;

    if (p->res == 0) {
        return;
    }
    it = func_020082f8(func_02029640(p), data_020cf898);
    if (it != 0) {
        if (func_02010b54(it) == 0) {
            p->ready = 0;
            p->level = 0;
            if (it->s24() != 0 || it->s25() != 0) {
                p->shown = 1;
            }
            return;
        }
        if (p->ready == 0) {
            p->ready = 1;
            if (p->shown == 0) {
                p->active = 1;
                p->step = 0;
            }
            p->shown = 0;
        }
    }
    if (p->active != 0) {
        v = p->step * 0x1f / 0x3c;
        p->level = v;
        p->step++;
        if (v > 0x1f) {
            p->level = 0x1f;
            p->active = 0;
        }
        return;
    }
    if (p->hold > 0) {
        p->hold--;
        if (p->hold <= 0) {
            p->fall = 100;
        }
    }
    if (p->fall > 0) {
        p->fall--;
        if (p->fall <= 0) {
            if (p->keep != 0) {
                p->rise = 100;
                return;
            }
            p->level = 0;
            return;
        }
        if (p->keep == 0) {
            p->level = p->fall * 0x1f / 100;
        }
    }
    if (p->rise != 0) {
        p->level = p->rise * 0x1f / 100;
        p->rise--;
        if (p->rise <= 0) {
            p->s8(0);
            return;
        }
    }
    func_02029028(p->res);
}
