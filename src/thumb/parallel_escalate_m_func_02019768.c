struct Part;
struct Owner {
    void *vtable;
    unsigned char pad[0x6c - 4];
    struct Part *part;
};
extern char data_020a3dd0[];
extern void func_0201dfc8(struct Part *part);
extern void func_02031270(void *object);
extern void func_02016e24(struct Owner *owner);

static inline void delete_part(struct Part *part)
{
    if (part != 0) {
        func_0201dfc8(part);
        func_02031270(part);
    }
}

struct Owner *func_02019768(struct Owner *owner)
{
    owner->vtable = data_020a3dd0;
    if (owner->part != 0) {
        delete_part(owner->part);
        owner->part = 0;
    }
    func_02016e24(owner);
    func_02031270(owner);
    return owner;
}
