class T02020b3c {
public:
    virtual int slot0();
    virtual int slot1();
    virtual int slot2();
    virtual int slot3();
    virtual int method();
};

extern "C" {
extern int func_0202037c(...);
extern int func_0202038c(...);
extern int func_02020ad0(...);
extern int func_02020be4(...);

int func_02020b3c(unsigned char *param_1)
{
    int c;
    int n;
    int (T02020b3c::*pm1)(int);
    int (T02020b3c::*pm2)();

    if (func_0202037c(param_1) != 0) {
        if (func_0202038c(param_1) != 0) {
            pm1 = *(int (T02020b3c::**)(int))(param_1 + 0xc);
            return (((T02020b3c *)*(int *)(param_1 + 8))->*pm1)(*(int *)(param_1 + 0x2c));
        }
        pm2 = *(int (T02020b3c::**)())(param_1 + 0xc);
        return (((T02020b3c *)*(int *)(param_1 + 8))->*pm2)();
    }
    if (func_0202038c(param_1) != 0) {
        c = *(int *)(param_1 + 0xc);
        if (c == 0) {
            return 0;
        }
        if (*(unsigned short *)(param_1 + 4) == 1) {
            if (func_02020ad0(param_1) != 0) {
                n = func_02020be4(((T02020b3c *)*(int *)(param_1 + 0xc))->method());
                return *(int *)(param_1 + 0xc) + n * *(int *)(param_1 + 0x2c);
            }
            return 0;
        }
        return *(int *)(c + *(int *)(param_1 + 0x2c) * 4);
    }
    if (*(unsigned short *)(param_1 + 4) == 1) {
        return *(int *)(param_1 + 0xc);
    }
    return **(int **)(param_1 + 0xc);
}
}
