class Obj438a0 {
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
    virtual void s13(int a,int b,int c);
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
    virtual void s33(int a);
    virtual void s34();
    virtual void s35();
    virtual void s36();
    virtual void s37();
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void s42(int a,int b,int c,int d,int e,int f,int g);
    virtual void s43();
    virtual void s44();
    virtual void s45();
    virtual void s46(int a,int b);
};
extern "C" {
extern const unsigned short data_02081eb4[];
extern void *func_0201003c(void);
extern int func_02015b3c(void *mgr,int key);
extern void *func_0202f624(int size);
extern Obj438a0 *func_02043950(void *mem);
extern void func_0205765c(Obj438a0 *obj,int a,int b,int c,int d);
extern void *func_02009900(void);
extern void func_0202f7e8(void *list,Obj438a0 *obj,int a,int b);
Obj438a0 *func_020438a0(int param_1,int param_2,int param_3,int param_4)
{
    int idx;
    unsigned short id;
    Obj438a0 *obj;
    idx = func_02015b3c(func_0201003c(),param_2);
    if (idx == 0) {
        return 0;
    }
    id = data_02081eb4[idx];
    obj = (Obj438a0 *)func_0202f624(0xd0);
    if (obj != 0) {
        obj = func_02043950(obj);
    }
    if (obj != 0) {
        obj->s42(id,0,1,0,0,-1,0);
        obj->s33(0xf0);
        func_0205765c(obj,0,0,0x40000,0x18000);
        obj->s46(0x40000,0x18000);
        obj->s13((int)0xfff9c000,(param_3 * 0x40 - 0x2a) * 0x1000,0);
        func_0202f7e8(func_02009900(),obj,7,0);
    }
    return obj;
}
}
