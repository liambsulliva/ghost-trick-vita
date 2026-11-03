typedef int code();

struct Vec {
    int x;
    int y;
    int z;
};

struct Entry {
    int item;
    struct Vec pos;
    unsigned char flag;
};

struct Iter {
    int words[4];
};

struct Spawner {
    char pad00[0x5c];
    int count;
    unsigned char used;
    char pad61[0xb];
    struct Entry *entries;
    char pad70[0x14];
    int slots[2];
};

extern char data_020cf87c;
extern char data_020cfa48;
extern char data_0209dad4;

extern void func_0202fbcc(struct Iter *it, int kind);
extern int func_0200eac4(struct Iter *it);
extern int func_0200eab4(struct Iter *it);
extern int func_02008828(int object, void *type);
extern int func_0200eab8(struct Iter *it);
extern void func_0200eabc(struct Iter *it, int flag);
extern int func_02010b54(int object);
extern unsigned int func_020266f4(int object);
extern int func_020266dc(int object, unsigned int index);
extern void *func_02008444(void);
extern int func_020328f8(void *game);
extern int func_02045cec(void *game, int index);
extern int func_0205bf7c(void *type);
extern void func_0205d730(int slot, int item);
extern void func_020153d8(int slot, struct Spawner *spawner);
extern void func_0205d734(int slot, int index);
extern void *func_02009900(void);
extern void func_0202f7e8(void *a, int b, int c, int d);
extern void *func_02007bd8(void);
extern int *func_02007b70(void *a);
extern void func_02058a80(struct Vec *v, int node);
extern void func_020083b0(struct Vec *dst, struct Vec *src);

void func_0205d534(struct Spawner *spawner)
{
    struct Iter it;
    struct Iter it2;
    struct Vec pos;
    int object;
    unsigned int total;
    int item;
    int object2;
    unsigned int total2;
    int item2;
    unsigned int i;
    unsigned char j;
    int k;
    int *heap;

    spawner->count = 0;
    func_0202fbcc(&it, 6);
    while (func_0200eac4(&it) != 0) {
        if (func_02008828(func_0200eab4(&it), &data_020cf87c) != 0) {
            object = func_0200eab8(&it);
            if (func_02010b54(object) != 0) {
                total = func_020266f4(object);
                for (i = 0; i < total; i++) {
                    item = func_020266dc(object, i);
                    if (item != 0) {
                        spawner->count++;
                        if (spawner->used < 2) {
                            for (j = 0; j < 3; j++) {
                                if (j != func_020328f8(func_02008444()) && item == func_02045cec(func_02008444(), j)) {
                                    spawner->slots[spawner->used] = func_0205bf7c(&data_020cfa48);
                                    if (spawner->slots[spawner->used] != 0) {
                                        func_0205d730(spawner->slots[spawner->used], item);
                                        func_020153d8(spawner->slots[spawner->used], spawner);
                                        func_0205d734(spawner->slots[spawner->used], j);
                                        func_0202f7e8(func_02009900(), spawner->slots[spawner->used], 7, 0);
                                        spawner->used++;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        func_0200eabc(&it, 0);
    }
    if (spawner->entries != 0) {
        if (func_02007b70(func_02007bd8()) != 0) {
            heap = func_02007b70(func_02007bd8());
            (**(code **)(*heap + 0x10))(heap, spawner->entries);
            spawner->entries = 0;
        }
    }
    heap = func_02007b70(func_02007bd8());
    spawner->entries = (struct Entry *)(**(code **)(*heap + 8))(heap, spawner->count * 0x14, &data_0209dad4);
    k = 0;
    func_0202fbcc(&it2, 6);
    while (func_0200eac4(&it2) != 0) {
        if (func_02008828(func_0200eab4(&it2), &data_020cf87c) != 0) {
            object2 = func_0200eab8(&it2);
            if (func_02010b54(object2) != 0) {
                total2 = func_020266f4(object2);
                for (i = 0; i < total2; i++) {
                    item2 = func_020266dc(object2, i);
                    if (item2 != 0) {
                        spawner->entries[k].item = item2;
                        func_02058a80(&pos, item2);
                        func_020083b0(&spawner->entries[k].pos, &pos);
                        spawner->entries[k].flag = 0;
                        k++;
                    }
                }
            }
        }
        func_0200eabc(&it2, 0);
    }
}
