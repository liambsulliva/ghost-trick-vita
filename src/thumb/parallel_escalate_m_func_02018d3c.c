struct Owner {
    unsigned char pad[0x20];
    unsigned short mode;
    unsigned char pad2[0x7c - 0x22];
    int target;
    unsigned char pad3[0x9a - 0x80];
    unsigned short wait;
    unsigned char pad4[0xa5 - 0x9c];
    unsigned char moved;
    unsigned char follow;
    unsigned char pad5[0xac - 0xa7];
    unsigned char path[4];
};
extern void *func_02008444(void);
extern int func_02018db4(void *object);
extern int func_02018dc4(struct Owner *owner);
extern void func_02017ddc(struct Owner *owner);
extern int func_0201931c(struct Owner *owner);
extern void func_0201d7bc(void *path, int target, unsigned char mode);
extern void func_02017760(struct Owner *owner);

int func_02018d3c(struct Owner *owner)
{
    if (func_02018db4(func_02008444()) != 0) {
        return 1;
    }
    if (owner->wait != 0) {
        owner->wait--;
        return 1;
    }
    if (func_02018dc4(owner) != 0) {
        func_02017ddc(owner);
        owner->moved = 0;
    }
    if (func_0201931c(owner) != 0) {
        return 1;
    }
    if (owner->follow != 0) {
        func_0201d7bc(owner->path, owner->target, owner->mode);
    }
    func_02017760(owner);
    return 0;
}
