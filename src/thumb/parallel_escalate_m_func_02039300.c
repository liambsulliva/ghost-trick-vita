struct Child02039300 {
    char pad[0x28];
    int value;
};

struct Self02039300 {
    char pad[0xb0];
    int first;
    int second;
    char pad0b8[0xc];
    int first_base;
    int second_base;
    struct Child02039300 *child;
};

extern void func_02039318(struct Child02039300 *child);

void func_02039300(struct Self02039300 *self)
{
    self->first = self->first_base;
    func_02039318(self->child);
}
