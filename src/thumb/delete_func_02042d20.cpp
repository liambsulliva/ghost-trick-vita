class Object02042d20_0 {
public:
    virtual ~Object02042d20_0();
};

extern "C" {
typedef int code(...);
extern int func_02023d1c(...);
extern int func_02042b20(...);

void func_02042d20(int param_1)

{
    func_02023d1c();
    if (*(int *)(param_1 + 0x14) != 0) {
        func_02042b20();
    }
    if (*(int **)(param_1 + 0x14) != (int *)0x0) {
        delete ((Object02042d20_0 *)(*(int **)(param_1 + 0x14)));
        *(unsigned int *)(param_1 + 0x14) = 0;
    }
    return;
}
}
