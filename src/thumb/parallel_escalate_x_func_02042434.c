typedef struct Item42434 {
    unsigned char active;
    char pad1[0x6b];
} Item42434;

typedef struct Holder42434 {
    char pad0[0x14];
    unsigned int count;
    Item42434 *items;
    char pad1c[4];
    struct Holder42434 *next;
} Holder42434;

typedef struct Root42434 {
    char pad0[0x58];
    Holder42434 *list;
} Root42434;

extern Root42434 data_020ce5c4;

void func_02042434(void)
{
    Holder42434 *holder;
    unsigned int i;

    for (holder = data_020ce5c4.list; holder != 0; holder = holder->next) {
        for (i = 0; i < holder->count; i++) {
            holder->items[i].active = 0;
        }
    }
}
