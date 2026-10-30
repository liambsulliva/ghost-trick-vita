class T02020b04 {
public:
    virtual void method(unsigned int value);
};

extern "C" {
extern int func_0202037c(...);

void func_02020b04(unsigned char *param_1, unsigned int param_2)
{
    int iVar1;
    void (T02020b04::*pm)(unsigned int);

    iVar1 = func_0202037c(param_1);
    if (iVar1 != 0) {
        if (*(int *)(param_1 + 0x24) != 0) {
            pm = *(void (T02020b04::**)(unsigned int))(param_1 + 0x24);
            (((T02020b04 *)*(int *)(param_1 + 8))->*pm)(param_2);
        }
    }
    else {
        *(unsigned int *)(param_1 + 0x14) = param_2;
    }
}
}
