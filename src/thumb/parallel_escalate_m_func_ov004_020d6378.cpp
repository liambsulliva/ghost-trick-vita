class Sound {
public:
    virtual void v00();
    virtual void v04();
};
class Bank {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10(int a);
};
struct PanelBits {
    unsigned char aa8_b0:1;
    unsigned char aa8_b1:1;
    unsigned char aa8_mid:3;
    unsigned char aa8_b5:1;
    unsigned char aa8_high:2;
    unsigned char aa9_low:3;
    unsigned char aa9_b3:1;
    unsigned char aa9_b4:1;
    unsigned char aa9_high:3;
    unsigned char mode:3;
};
union PanelFlags {
    int word;
    struct PanelBits bits;
};
struct Panel {
    unsigned char pad000[4];
    unsigned char alarm[0x34];
    int w38;
    int w3c;
    unsigned char pad040[0xc];
    unsigned char file4c[0x48];
    unsigned char file94[0x9cc - 0x94];
    int w9cc;
    int w9d0;
    int w9d4;
    unsigned char pad9d8[0xaa0 - 0x9d8];
    int waa0;
    Sound *sound;
    union PanelFlags flags;
};
extern "C" {
extern unsigned int func_02074500(void);
extern void func_ov004_020d5b38(struct Panel *p);
extern void func_02075708(void *alarm);
extern unsigned short func_ov004_020d6b98(int a);
extern void func_ov004_020d3cc4(unsigned int a);
extern void func_0206c614(void *file);
extern unsigned short func_ov004_020d61f0(void);
extern unsigned short func_ov004_020d6200(unsigned short a);
extern void *func_02007bd8(void);
extern Bank *func_02007b70(void *object);

void func_ov004_020d6378(struct Panel *param_1)
{
    unsigned short ime;
    unsigned short enabled;

    if ((func_02074500() & 0x40000000) != 0 || (func_02074500() & 0x80000000) != 0) {
        if (param_1->flags.bits.aa8_b5 == 1) {
            func_ov004_020d5b38(param_1);
        }
    }
    func_02075708(param_1->alarm);
    if (param_1->w38 != 0) {
        func_ov004_020d6b98(param_1->w38);
    }
    if (param_1->w3c != 0) {
        func_ov004_020d6b98(param_1->w3c);
    }
    param_1->w38 = 0;
    param_1->w3c = 0;
    if (param_1->w9cc != 0) {
        func_ov004_020d3cc4(param_1->w9cc);
        func_0206c614(param_1->file4c);
    }
    if (param_1->w9d0 != 0) {
        func_ov004_020d3cc4(param_1->w9d0);
        func_0206c614(param_1->file94);
    }
    if (param_1->flags.bits.aa9_b3) {
        ime = func_ov004_020d61f0();
        enabled = 1;
        if (ime == 0) {
            enabled = 0;
        }
        if (param_1->sound != 0) {
            param_1->sound->v04();
        }
        if (!param_1->flags.bits.aa9_b4 && param_1->waa0 != 0) {
            if (func_02007b70(func_02007bd8()) != 0) {
                func_02007b70(func_02007bd8())->v10(param_1->waa0);
                param_1->waa0 = 0;
            }
        }
        func_ov004_020d6200(enabled);
    }
    param_1->waa0 = 0;
    param_1->flags.word = 0;
    param_1->flags.bits.mode = 0;
    param_1->w9cc = 0;
    param_1->w9d0 = 0;
    param_1->w9d4 = 0;
    param_1->flags.bits.aa8_b0 = 0;
    param_1->flags.bits.aa8_b1 = 0;
}
}
