#pragma opt_loop_invariants off

typedef struct Entry013dc {
    unsigned int key;
    unsigned int value;
} Entry013dc;

typedef struct List013dc {
    unsigned int count;
    Entry013dc entries[1];
} List013dc;

typedef struct Bank013dc {
    char pad[0x15a4];
    unsigned int flags;
    char pad15a8[0x604];
    List013dc *lists[2];
} Bank013dc;

typedef struct Game013dc {
    char pad[0x10];
    Bank013dc *bank;
} Game013dc;

extern Game013dc data_020b18a0;
extern void func_02001468(void *src, void *dst, int size);

void func_020013dc(void)
{
    Bank013dc *bank;
    List013dc *list; Entry013dc *entries; int i; int j;
    int ia; int ja;
    Entry013dc tmp;

    bank = data_020b18a0.bank;
    if ((bank->flags & 0x40000000) == 0) {
        list = bank->lists[0];
    } else {
        list = bank->lists[1];
    }
    if (list->count == 0) {
        return;
    }
    i = 0;
    entries = list->entries;
    for (; i < list->count - 1; i++) {
        ia = i * 8;
        for (j = i + 1; j < list->count; j++) {
            ja = j * 8;
            if (*(unsigned int *)((char *)entries + ia) > *(unsigned int *)((char *)entries + ja)) {
                func_02001468((char *)entries + ia, &tmp, 8);
                func_02001468((char *)entries + ja, (char *)entries + ia, 8);
                func_02001468(&tmp, (char *)entries + ja, 8);
            }
        }
    }
}
