typedef int code();

struct Owner {
    char pad0[0xc];
    int flags;
    char pad1[0xdfc - 0x10];
    int slots[8];
};

void func_02010c28(struct Owner *param_1, void *param_2, unsigned short param_3, unsigned short param_4)
{
    unsigned int i;
    int p;
    for (i = 0; i < 8; i++) {
        p = param_1->slots[i];
        if (p != 0) {
            if ((void *)(*(code **)(*(int *)p + 0xe8))(p) == param_2) {
                (*(code **)(*(int *)param_1->slots[i] + 0xe0))(param_1->slots[i], param_3, param_4);
                return;
            }
        }
    }
}
