extern "C" {
extern int func_0202037c(...);
extern int func_0202038c(...);
extern void func_020082d8(int *dst, int *src);
}

struct Vec02022258 {
    int x;
    int y;
    int z;
    Vec02022258(const Vec02022258 &other) { func_020082d8((int *)this, (int *)&other); }
};

class T02022258 {
public:
    virtual void method();
};

extern "C" {

Vec02022258 func_02022258(unsigned char *param_1)
{
    int iVar1;
    Vec02022258 (T02022258::*pm1)(int);
    Vec02022258 (T02022258::*pm2)();

    iVar1 = func_0202037c(param_1);
    if (iVar1 != 0) {
        iVar1 = func_0202038c(param_1);
        if (iVar1 != 0) {
            pm1 = *(Vec02022258 (T02022258::**)(int))(param_1 + 0xc);
            return (((T02022258 *)*(int *)(param_1 + 8))->*pm1)(*(int *)(param_1 + 0x2c));
        }
        else {
            pm2 = *(Vec02022258 (T02022258::**)())(param_1 + 0xc);
            return (((T02022258 *)*(int *)(param_1 + 8))->*pm2)();
        }
    }
    else {
        return ((Vec02022258 *)*(int *)(param_1 + 0xc))[*(int *)(param_1 + 0x2c)];
    }
}
}
