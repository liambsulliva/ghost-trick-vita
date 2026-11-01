class Item {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1c();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2c();
    virtual void v30();
    virtual void v34();
    virtual void v38();
    virtual void v3c();
    virtual void v40();
    virtual void v44();
    virtual void v48();
    virtual void v4c();
    virtual void v50();
    virtual void v54();
    virtual void v58();
    virtual void v5c();
    virtual void v60();
    virtual void v64();
    virtual void v68();
    virtual void v6c();
    virtual void v70();
    virtual void v74();
    virtual void v78();
    virtual void v7c();
    virtual void v80();
    virtual void v84();
    virtual void v88();
    virtual void v8c();
    virtual void v90();
    virtual void v94();
    virtual void v98();
    virtual void v9c();
    virtual void va0();
    virtual void va4();
    virtual void va8();
    virtual void vac();
    virtual void vb0();
    virtual void vb4();
    virtual void vb8();
    virtual void vbc();
    virtual void vc0();
    virtual void vc4();
    virtual void vc8();
    virtual void vcc();
    virtual void vd0();
    virtual int IsReady();
};

struct Pad {
    unsigned int cur;
    unsigned int trig;
    unsigned int rel;
    unsigned int held;
};

extern "C" {
extern struct Pad data_020c9d50;
unsigned int func_0201572c(void);
void func_0203bfb4(Item *object, int value);
void *func_0200847c(void);
unsigned int func_0202a0c0(void *object, int value);
int func_0203cf6c(Item *object);
void func_0203cff4(unsigned char *p, unsigned char sel);

int func_0203ce68(unsigned char *p, int confirm)
{
    int sel;

    if (func_0201572c() != 0) {
        return -1;
    }
    if ((data_020c9d50.held & 0xc00) != 0) {
        p[5] = (p[5] + 1) & 1;
        func_0203bfb4(*(Item **)(p + p[5] * 4 + 0x38), 1);
        func_0203bfb4(*(Item **)(p + (1 - p[5]) * 4 + 0x38), 0);
        func_0202a0c0(func_0200847c(), 1);
        return -2;
    }
    sel = -1;
    if ((data_020c9d50.trig & 0x1000) != 0) {
        sel = *(signed char *)(p + 5);
    }
    if ((data_020c9d50.trig & 0x2000) != 0) {
        sel = 1;
        p[5] = 1;
    }
    if ((*(Item **)(p + 0x38))->IsReady() != 0) {
        sel = 0;
        p[5] = 0;
    }
    if ((*(Item **)(p + 0x3c))->IsReady() != 0) {
        sel = 1;
        p[5] = 1;
    }
    if (sel == 0) {
        if (confirm != 0 && func_0203cf6c(*(Item **)(p + 0x38)) == 0) {
            func_0203cff4(p, p[5]);
            func_0202a0c0(func_0200847c(), 1);
            return -2;
        }
        func_0203bfb4(*(Item **)(p + 0x38), 2);
        func_0203bfb4(*(Item **)(p + 0x3c), 0);
        func_0202a0c0(func_0200847c(), 2);
    } else if (sel == 1) {
        func_0203bfb4(*(Item **)(p + 0x3c), 2);
        func_0203bfb4(*(Item **)(p + 0x38), 0);
        func_0202a0c0(func_0200847c(), 3);
    }
    return sel;
}
}
