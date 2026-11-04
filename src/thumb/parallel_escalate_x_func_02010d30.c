typedef int code();

struct Owner {
    char pad0[0xc];
    int flags;
    char pad1[0xdfc - 0x10];
    int slots[8];
};

void func_02010d30(struct Owner *param_1)
{
    param_1->flags = (param_1->flags & ~0x80) | 0x100;
    param_1->flags |= 0x200;
}
