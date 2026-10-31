struct Entry {
    unsigned char active;
    unsigned char pad01[7];
    int position;
    int speed;
    unsigned char pad10[4];
};

struct Owner {
    unsigned char pad000[0xb8];
    struct Entry *entries;
    unsigned char pad0bc[8];
    unsigned char half;
};

extern void func_020484fc(struct Owner *owner, struct Entry *entry, int value);
extern void *func_02008444(void);
extern int func_0200f5e8(void *object);

void func_0204849c(struct Owner *param_1)
{
    unsigned int count;
    unsigned int i;
    struct Entry *entry;

    count = 0x80;
    if (param_1->half != 0) {
        count = 0x40;
    }
    for (i = 0; i < count; i++) {
        if (param_1->entries[i].active == 0) {
            func_020484fc(param_1, &param_1->entries[i], 0);
        } else {
            param_1->entries[i].position = param_1->entries[i].position + param_1->entries[i].speed;
            entry = &param_1->entries[i];
            if (entry->position > func_0200f5e8(func_02008444()) + 0xa0000) {
                entry->active = 0;
            }
        }
    }
}
