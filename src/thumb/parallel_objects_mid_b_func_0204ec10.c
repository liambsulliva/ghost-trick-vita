typedef int code();

struct ScriptTimer {
    void **vtable;
    char pad[0x44];
    short h48;
};

void func_0204ec10(struct ScriptTimer *param_1)
{
    param_1->h48++;
    if (param_1->h48 > 0x40) {
        ((code *)((int *)param_1->vtable)[8])(param_1, 0);
    }
}
