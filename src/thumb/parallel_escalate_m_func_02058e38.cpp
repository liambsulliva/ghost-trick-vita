class Obj02058e38 {
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
};

extern "C" {
extern int func_02010b54(Obj02058e38 *obj);
extern void func_0200f70c(int *vec);
extern void func_020592c8(Obj02058e38 *obj);
extern void func_020083b0(int *vec, char *src);
extern unsigned int *func_02008444(void);
extern int func_0200f5c8(unsigned int *p);
extern int func_0200f5e8(unsigned int *p);
extern void func_0200ba7c(char *dst, int *vec);
extern int func_0200b808(char *p);
}

extern "C" void func_02058e38(Obj02058e38 *param_1)
{
    int aiStack[3];

    if (*((unsigned char *)param_1 + 0xc) == 3 && func_02010b54(param_1) != 0) {
        func_0200f70c(aiStack);
        func_020592c8(param_1);
        func_020083b0(aiStack, (char *)param_1 + 0xb0);
        aiStack[0] = aiStack[0] - func_0200f5c8(func_02008444());
        aiStack[1] = aiStack[1] - func_0200f5e8(func_02008444());
        func_0200ba7c((char *)param_1 + 0x28, aiStack);
        if (func_0200b808((char *)param_1 + 0x28) == 0) {
            param_1->v2c();
        }
    }
}
