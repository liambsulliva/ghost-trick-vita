typedef void code();

extern int func_020109c0();

struct Owner {
    char pad0[0xc];
    int flags;
    char pad1[0xdfc - 0x10];
    int slots[8];
};

void func_02010974(struct Owner *param_1, int param_2)
{
    unsigned int i;

    i = 0;
    do {
        if (param_1->slots[i] != 0) {
            if (func_020109c0(param_1->slots[i]) == 0) {
                if (param_2 == 0) {
                    (*(code **)(*(int *)param_1->slots[i] + 0xd8))(param_1->slots[i]);
                }
                else if (((((unsigned int)param_1->flags) << 16) >> 31) == 0) {
                    (*(code **)(*(int *)param_1->slots[i] + 0xd4))(param_1->slots[i]);
                }
            }
        }
        i = i + 1;
    } while (i < 8);
}
