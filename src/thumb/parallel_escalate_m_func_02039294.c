struct Child02039294 {
    char pad[0x28];
    int value;
};

struct Self02039294 {
    char pad[0xb0];
    int first;
    int second;
    char pad0b8[0xc];
    int first_base;
    int second_base;
    struct Child02039294 *child;
};

extern void func_0203a328(struct Child02039294 *child, int value);

void func_02039294(struct Self02039294 *self, int value)
{
    self->first += value;
    func_0203a328(self->child, value);
}
