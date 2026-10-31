class Owned02050ec0 {
public:
    virtual ~Owned02050ec0();
};

class Runner02050ec0 {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14(int a, int b);
};

extern "C" {
extern int func_02050f08(Runner02050ec0 *run);
extern int func_02023fdc(Runner02050ec0 *run);

void func_02050ec0(char *param_1, int param_2, int param_3, int param_4)
{
    if (*(Owned02050ec0 **)(param_1 + 0x3c) != 0) {
        delete *(Owned02050ec0 **)(param_1 + 0x3c);
    }
    *(unsigned char *)(param_1 + 0x4c) = 0;
    *(int *)(param_1 + 0x3c) = 0;
    *(int *)(param_1 + 0x44) = 0;
    *(int *)(param_1 + 0x34) = param_2;
    ((Runner02050ec0 *)(param_1 + 4))->v14(param_3, param_4);
    while (func_02050f08((Runner02050ec0 *)(param_1 + 4)) == 0) {
        func_02023fdc((Runner02050ec0 *)(param_1 + 4));
    }
}
}
