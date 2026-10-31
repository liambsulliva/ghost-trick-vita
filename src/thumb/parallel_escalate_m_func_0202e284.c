struct Flags0202e284 {
    unsigned char active : 1;
};

struct Entry0202e284 {
    char data[0x6c];
};

struct Owner0202e284 {
    char before[0x144];
    unsigned int count;
    char middle[0x1f0 - 0x148];
    struct Entry0202e284 *entries;
    int busy;
};

extern void func_0202e258(int object);
extern int func_0202e2e4(struct Entry0202e284 *entry);
extern int func_0202e17c(struct Entry0202e284 *entry);
extern void func_0202db3c(struct Entry0202e284 *entry);

static inline struct Entry0202e284 *GetEntry0202e284(struct Owner0202e284 *owner, unsigned int index)
{
    return &owner->entries[index];
}

void func_0202e284(int object)
{
    unsigned int i;

    if (((struct Flags0202e284 *)(object + 4))->active == 0) {
        func_0202e258(object);
        return;
    }
    if (((struct Owner0202e284 *)object)->busy == 0) {
        for (i = 0; i < ((struct Owner0202e284 *)object)->count; i++) {
            if (func_0202e2e4(GetEntry0202e284((struct Owner0202e284 *)object, i)) == 0) {
                if (func_0202e17c(GetEntry0202e284((struct Owner0202e284 *)object, i)) != 0) {
                    func_0202db3c(GetEntry0202e284((struct Owner0202e284 *)object, i));
                }
            }
        }
    }
}
