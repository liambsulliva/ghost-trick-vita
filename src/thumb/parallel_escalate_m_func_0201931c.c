struct Owner {
    unsigned char pad[0x59];
    unsigned char result;
    unsigned char pad2[0xa4 - 0x5a];
    unsigned char active;
    unsigned char count;
};
extern int func_02017e64(void);

int func_0201931c(struct Owner *owner)
{
    if (owner->active != 0 && func_02017e64() == 0) {
        if (owner->count != 0) {
            owner->count--;
        } else {
            owner->active = 0;
        }
        return 1;
    }
    return owner->result;
}
