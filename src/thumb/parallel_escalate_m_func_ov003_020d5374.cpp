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
class Panel {
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
    virtual void ShowLeft();
    virtual void ShowRight();
};

struct Pad {
    unsigned int cur;
    unsigned int trig;
    unsigned int rel;
    unsigned int held;
};

extern "C" {
extern struct Pad data_020c9d50;
void func_0203bfb4(Item *object, int value);
void *func_0200847c(void);
unsigned int func_0202a0c0(void *object, int value);
void func_ov003_020d4818(Panel *panel);
void func_ov003_020d5374(Panel *panel);
}

void func_ov003_020d5374(Panel *panel)
{
    int i;
    int mode;
    Item *item;

    mode = 0;
    for (i = 0; i < 2; i++) {
        item = ((Item **)panel)[i + 0x58];
        if (item != 0 && item->IsReady() != 0) {
            mode = i + 4;
        }
    }
    if ((data_020c9d50.held & 0xc00) != 0) {
        ((unsigned char *)panel)[0x16a] = 1 - ((unsigned char *)panel)[0x16a];
        func_0203bfb4(*(Item **)((char *)panel + ((unsigned char *)panel)[0x16a] * 4 + 0x160), 1);
        func_0203bfb4(*(Item **)((char *)panel + (1 - ((unsigned char *)panel)[0x16a]) * 4 + 0x160), 0);
        func_0202a0c0(func_0200847c(), 1);
    }
    if ((data_020c9d50.trig & 0x1000) != 0) {
        mode = ((unsigned char *)panel)[0x16a] + 4;
    }
    if ((data_020c9d50.trig & 0x2000) != 0) {
        mode = 3;
    }
    if (mode == 3 || mode == 5) {
        panel->ShowRight();
        func_0203bfb4(((Item **)panel)[0x58], 0);
        func_0203bfb4(((Item **)panel)[0x59], 2);
        func_0202a0c0(func_0200847c(), 3);
        return;
    }
    if (mode == 4) {
        panel->ShowLeft();
        func_0203bfb4(((Item **)panel)[0x58], 2);
        func_0203bfb4(((Item **)panel)[0x59], 0);
        func_0202a0c0(func_0200847c(), 2);
        return;
    }
    func_ov003_020d4818(panel);
}
