class Object0206614c_0 {
public:
    virtual ~Object0206614c_0();
};

extern "C" {
typedef int code(...);

void func_0206614c(int param_1)

{
    if (*(char *)(param_1 + 0xcc) != '\0') {
        *(unsigned int *)(param_1 + 200) = 0;
    }
    if (*(int *)(param_1 + 200) != 0) {
        *(unsigned int *)(*(int *)(param_1 + 200) + 0xb4) = 0;
    }
    if (*(int **)(param_1 + 200) != (int *)0x0) {
        delete ((Object0206614c_0 *)(*(int **)(param_1 + 200)));
        *(unsigned int *)(param_1 + 200) = 0;
    }
    return;
}
}
