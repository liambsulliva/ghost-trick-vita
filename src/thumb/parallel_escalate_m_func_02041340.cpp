class Obj41340 {
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
};
extern "C" {
struct Entry41340 {
    unsigned short id;
    unsigned short pad;
};
extern struct Entry41340 data_0209b1b8[];
extern void *func_02008444(void);
extern unsigned int func_020283b0(void *sys);
extern void func_0200b598(char *res,unsigned int id,int flags);
extern int func_0200f330(void *sys);
extern int func_0202ff58(Obj41340 *obj);
extern void func_02008418(Obj41340 *obj);
extern void func_02010dc4(Obj41340 *obj);
extern int func_02010b54(Obj41340 *obj);
void func_02041340(char *param_1)
{
    unsigned int value;
    if (*(unsigned int *)(param_1 + 0xd4) != func_020283b0(func_02008444())) {
        value = func_020283b0(func_02008444());
        *(unsigned int *)(param_1 + 0xd4) = value;
        value = data_0209b1b8[value].id;
        *(unsigned int *)(param_1 + 0xbc) = value;
        func_0200b598(param_1 + 0x28,value,0xf);
    }
    if (func_0200f330(func_02008444()) != 0) {
        if (func_0202ff58(*(Obj41340 **)(param_1 + 0xd0)) != 0) {
            func_02008418(*(Obj41340 **)(param_1 + 0xd0));
        }
    }
    else {
        if (func_0202ff58(*(Obj41340 **)(param_1 + 0xd0)) == 0) {
            func_02010dc4(*(Obj41340 **)(param_1 + 0xd0));
        }
    }
    if (*(int *)(param_1 + 0xd4) == 0) {
        if (func_02010b54(*(Obj41340 **)(param_1 + 0xd0)) != 0) {
            (*(Obj41340 **)(param_1 + 0xd0))->s11();
            return;
        }
    }
    else {
        if (func_02010b54(*(Obj41340 **)(param_1 + 0xd0)) == 0) {
            (*(Obj41340 **)(param_1 + 0xd0))->s10();
        }
    }
}
}
