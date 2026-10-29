class Object020187e8_0 {
public:
    virtual ~Object020187e8_0();
};

extern "C" {
typedef int code(...);

void func_020187e8(int param_1)

{
    if (*(int **)(param_1 + 0x80) != (int *)0x0) {
        delete ((Object020187e8_0 *)(*(int **)(param_1 + 0x80)));
        *(unsigned int *)(param_1 + 0x80) = 0;
    }
    return;
}
}
