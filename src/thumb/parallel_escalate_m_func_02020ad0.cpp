class T02020ad0 {
public:
    virtual int method();
};

extern "C" {
extern int func_0202037c(...);

int func_02020ad0(unsigned char *param_1)
{
    int iVar1;
    int (T02020ad0::*pm)();

    iVar1 = func_0202037c(param_1);
    if (iVar1 != 0) {
        if (*(int *)(param_1 + 0x14) != 0) {
            pm = *(int (T02020ad0::**)())(param_1 + 0x14);
            return (((T02020ad0 *)*(int *)(param_1 + 8))->*pm)();
        }
        return 0;
    }
    return *(int *)(param_1 + 0x14);
}
}
