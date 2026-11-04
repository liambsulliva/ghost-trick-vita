typedef int code();

struct Owner {
    char pad0[0xc];
    int flags;
    char pad1[0xdfc - 0x10];
    int slots[8];
};

int func_02010c70(struct Owner *param_1, void *param_2)
{
    unsigned int i;
    int p;
    int r;
    for (i = 0; i < 8; i++) {
        p = param_1->slots[i];
        if (p != 0 && (void *)(*(code **)(*(int *)p + 0xe8))(p) == param_2) {
            return 1;
        }
    }
    return 0;
}
