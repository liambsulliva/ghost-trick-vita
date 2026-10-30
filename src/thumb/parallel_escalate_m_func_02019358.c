struct Owner {
    unsigned char pad[0x2e];
    unsigned short x;
    unsigned char pad2[0xa2 - 0x30];
    unsigned short next;
};
extern void func_02015f3c(struct Owner *owner, int value);

int func_02019358(struct Owner *owner)
{
    if (owner->next != 0) {
        owner->x = owner->next;
        owner->next = 0;
        func_02015f3c(owner, 1);
        return 1;
    }
    func_02015f3c(owner, 0);
    return 0;
}
