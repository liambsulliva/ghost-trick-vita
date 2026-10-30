struct Child02039324 {
    char pad[0x28];
    int value;
};

struct Self02039324 {
    char pad[0xb0];
    int first;
    int second;
    char pad0b8[0xc];
    int first_base;
    int second_base;
    struct Child02039324 *child;
};

extern void func_0203933c(struct Child02039324 *child);

void func_02039324(struct Self02039324 *self)
{
    self->second = self->second_base;
    func_0203933c(self->child);
}
