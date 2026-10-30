struct Slot {
    int first;
    int second;
    int pad;
    short kind;
    unsigned short id;
};
struct Owner {
    char pad[0x48];
    struct Slot slots[5];
};
extern void *func_0201003c(void);
extern int func_02015b58(void *list, int id);
extern int func_02043814(struct Owner *owner, int id, unsigned short kind);
extern int func_020438a0(struct Owner *owner, int id, unsigned short kind);
void func_020436c8(struct Owner *owner, int kind, int id)
{
    struct Slot *slot;
    unsigned int i;
    int result;
    unsigned short narrow;
    slot = 0;
    for (i = 0; i < 5; i++) {
        if (owner->slots[i].first == 0) {
            slot = &owner->slots[i];
            break;
        }
    }
    if (slot != 0 && func_02015b58(func_0201003c(), id) != 0) {
        slot->kind = kind;
        slot->id = id;
        narrow = kind;
        result = func_02043814(owner, id, narrow);
        if (result != 0) {
            slot->first = result;
        }
        result = func_020438a0(owner, id, narrow);
        if (result != 0) {
            slot->second = result;
        }
    }
}
