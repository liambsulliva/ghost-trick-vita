struct Part;
struct Owner {
    void *vtable;
    unsigned char pad[0x68 - 4];
    int field68;
    struct Part *part;
    unsigned char b70;
    unsigned char b71;
    unsigned char b72;
    unsigned char b73;
    unsigned char b74;
};
extern char data_020a3dd0[];
extern void func_02016c4c(struct Owner *owner);
extern struct Part *func_02031254(unsigned int size);
extern struct Part *func_0201dfc0(struct Part *part);

struct Owner *func_020196f0(struct Owner *owner)
{
    struct Part *part;

    func_02016c4c(owner);
    owner->vtable = data_020a3dd0;
    owner->field68 = 0;
    owner->b70 = 0;
    owner->b71 = 0;
    owner->b72 = 0;
    owner->b73 = 0;
    owner->b74 = 0;
    owner->part = 0;
    part = func_02031254(4);
    if (part != 0) {
        part = func_0201dfc0(part);
    }
    owner->part = part;
    return owner;
}
