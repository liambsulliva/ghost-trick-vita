class Obj3d934 {
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
    virtual void s24();
    virtual void s25();
    virtual void s26();
    virtual void s27();
    virtual void s28();
    virtual void s29();
    virtual void s30();
    virtual void s31();
    virtual void s32();
    virtual void s33();
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
    virtual void s46();
    virtual void s47();
    virtual void s48();
    virtual void s49();
    virtual void s50();
    virtual void s51();
    virtual void s52();
    virtual void s53(int a);
};
extern "C" {
extern const char data_0209ab20[];
extern void *func_020312e4(void);
extern void func_02008ff4(void *mgr);
extern void func_0203cbe0(void *mgr,int on);
extern unsigned int func_0203cc14(void *mgr);
extern void *func_02008444(void);
extern void func_0200fcc0(void *sys,int mode);
extern void func_0200fbc8(void *sys);
extern int func_02027d4c(const char *name,int mode);
extern void *func_0202f624(int size);
extern Obj3d934 *func_02067a84(void *mem);
extern void *func_02009900(void);
extern void func_0202f7e8(void *list,Obj3d934 *obj,int a,int b);
extern void func_02022f6c(unsigned char *self);
extern void func_02075f84(void);
void func_0203d934(unsigned char *param_1)
{
    unsigned int result;
    int handle;
    Obj3d934 *obj;
    switch (param_1[4]) {
    case 0:
        func_02008ff4(func_020312e4());
        func_0203cbe0(func_020312e4(),1);
        param_1[4] = 1;
        return;
    case 1:
        result = func_0203cc14(func_020312e4());
        if (result < 1) {
            return;
        }
        {
            if (result == 2) {
                func_0200fbc8(func_02008444());
                handle = func_02027d4c(data_0209ab20,2);
                obj = (Obj3d934 *)func_0202f624(0xd4);
                if (obj != 0) {
                    obj = func_02067a84(obj);
                }
                obj->s53(handle);
                func_0202f7e8(func_02009900(),obj,1,0);
                param_1[6] = 0;
                param_1[4] = 2;
                return;
            }
            func_0200fcc0(func_02008444(),2);
            func_0203cbe0(func_020312e4(),0);
            func_02022f6c(param_1);
            return;
        }
        return;
    case 2:
        param_1[6] = param_1[6] + 1;
        if (param_1[6] > 0x10) {
            func_02075f84();
        }
        return;
    }
}
}
