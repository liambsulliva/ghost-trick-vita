struct Child020392b0 {
    char pad[0x28];
    int value;
};

struct Self020392b0 {
    char pad[0xb0];
    int first;
    int second;
    char pad0b8[0xc];
    int first_base;
    int second_base;
    struct Child020392b0 *child;
};

extern void func_0203a330(struct Child020392b0 *child, int value);

void func_020392b0(struct Self020392b0 *self, int value)
{
    self->second += value;
    func_0203a330(self->child, -value);
}
