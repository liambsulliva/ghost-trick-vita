struct Holder {
    unsigned char pad00[0x14];
    unsigned int count;
    char *items;
};

extern void *func_0200b208(void);
extern void *func_02023ebc(void *manager, unsigned int key);
extern void func_0200cb60(void *object);
extern int func_02042170(void *object);
extern void func_02023ef4(void *manager);
extern struct Holder *func_02023d60(int size);
extern struct Holder *func_02042294(struct Holder *object);
extern void *func_0200883c(void);
extern void *func_0201a4e8(void *object);
extern int *func_0201e888(void *object, int a, unsigned int b, int c, int d);
extern char *func_0207dc50(unsigned int count, int size, int align, void *ctor, void *dtor);
extern unsigned char *func_0202dfa0(unsigned char *object);
extern void func_0202dbdc(void);
extern int func_0200cb5c(void *object);
extern int func_02023f34(void *manager, struct Holder *holder, int *file, int value, unsigned int key);

int func_0204235c(unsigned int param_1)
{
    unsigned int key;
    void *found;
    unsigned int i;
    struct Holder *holder;
    int *file;
    int *table;
    char *base;
    char *head;
    char *entry;

    key = (param_1 & 0xffff) << 10 | 0x8c000000;
    found = func_02023ebc(func_0200b208(), key);
    if (found != 0) {
        func_0200cb60(found);
        return func_02042170(found);
    }
    func_02023ef4(func_0200b208());
    holder = func_02023d60(0x24);
    if (holder != 0) {
        holder = func_02042294(holder);
    }
    if (holder != 0) {
        file = func_0201e888(func_0201a4e8(func_0200883c()), 3, param_1, 0, 0);
        if (file != 0) {
            table = file + 1;
            base = (char *)(table + file[0]);
            head = base + 8;
            holder->count = *(unsigned int *)(head + *(int *)(head + 0x28));
            holder->items = func_0207dc50(holder->count, 0x6c, 8, (void *)func_0202dfa0, (void *)func_0202dbdc);
            for (i = 0; i < holder->count; i++) {
                entry = base + table[i + 1];
                if (entry != 0) {
                    *(char **)(holder->items + i * 0x6c + 4) = entry;
                }
            }
            return func_02023f34(func_0200b208(), holder, file, func_0200cb5c(func_0201a4e8(func_0200883c())), key);
        }
    }
    return 0;
}
