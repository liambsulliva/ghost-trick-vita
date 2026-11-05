#pragma opt_common_subs off
struct State {
    unsigned int before[4];
    int data;
};

struct Scan {
    unsigned int state[2];
    unsigned char flag;
    unsigned char pad;
    unsigned short f1;
    unsigned short f2;
    unsigned short pad2;
    unsigned int tail;
};

struct Key {
    unsigned int id : 24;
    unsigned int rest : 8;
};

struct Load {
    unsigned int key;
    unsigned int size;
};

struct LoadList {
    int count;
    struct Load entries[1];
};

struct File {
    unsigned int pad00[2];
    unsigned int size;
    unsigned int pad0c[11];
    unsigned int base;
    unsigned int table[1];
};

struct Lists {
    char pad[0x1bac];
    struct LoadList *list_a;
    struct LoadList *list_b;
};

extern struct State data_020b18a0;
extern void func_02077518();
extern int func_02077538();
extern int func_02077718();
extern void func_020776e4();
extern int func_0200286c();
extern int func_02000d64(int obj, int id, unsigned char *buf, int size, int offset);
extern void func_02074860(unsigned char *buf, int size);

int func_020010ac(struct Key *key, unsigned char *archive, struct File *file, unsigned int group, unsigned char *buf)
{
    struct Scan s;
    unsigned int index;
    unsigned int entry;
    unsigned int kind;
    unsigned int pos;
    unsigned char *p;
    int end;
    unsigned int gs;
    int total;
    int size;
    int start;
    int sub;
    int ret;
    unsigned int flags;
    struct LoadList *list;
    struct Load *e;
    struct Load *ents;
    unsigned int cnt;
    unsigned int w;

    total = 0;
    func_02077518(&s, archive);
    gs = group << 24;
    while (func_02077538(archive, &s.flag, &s) != 0) {
        if (s.flag != 1 || s.f2 != group) {
            continue;
        }
        if (s.state[1] != 0) {
            index = s.state[0];
            entry = *(unsigned int *)(archive + 0x3c + index * 4);
            kind = entry;
            if ((kind & 0xff) == 0x10) {
                sub = archive[(entry << 8) >> 16];
                sub = sub + s.state[1] - 1;
            } else {
                p = archive + ((entry << 8) >> 16);
                if (s.state[1] != 0) {
                    sub = *(p + s.state[1] - 1);
                } else {
                    sub = 0xff;
                }
            }
        } else {
            index = s.state[0] - 1;
            sub = 0;
            kind = *(unsigned int *)(archive + 0x3c + index * 4) & 0xff;
        }
        if (*(unsigned char *)(data_020b18a0.data + 0x1b7d) != 0) {
            if ((kind & 0xff) == 0x10) {
                w = index / 32;
                if (((*(unsigned int *)(data_020b18a0.data + w * 4 + 0x13e0) >> (index - w * 32)) & 1) == 0) {
                    continue;
                }
                if (func_0200286c(archive, &s) == 0) {
                    continue;
                }
            } else {
                w = index / 32;
                if (((*(unsigned int *)(data_020b18a0.data + w * 4 + 0x13e0) >> (index - w * 32)) & 1) == 0) {
                    continue;
                }
            }
        }
        if (func_02077718(file, s.f1) != 0) {
            continue;
        }
        pos = file->base + s.f1;
        start = file->table[pos];
        if (s.f1 < file->base - 1) {
            end = file->table[(int)(pos + 1)];
        } else {
            end = file->size;
        }
        size = end - start;
        flags = *(unsigned int *)(data_020b18a0.data + 0x15a4);
        if (flags & 0x80000000) {
            if ((flags & 0x40000000) == 0) {
                list = ((struct Lists *)data_020b18a0.data)->list_a;
                cnt = list->count;
                ents = list->entries;
                e = ents + cnt;
                list->count = cnt + 1;
            } else {
                list = ((struct Lists *)data_020b18a0.data)->list_b;
                cnt = list->count;
                ents = list->entries;
                e = ents + cnt;
                list->count = cnt + 1;
            }
            e->key = gs | ((unsigned char)sub | ((unsigned short)index << 8));
            e->size = size;
        }
        func_020776e4(file, s.f1, buf);
        ret = func_02000d64(*(int *)(data_020b18a0.data + 0x1580), key->id, buf, size, start);
        if (!(ret != -2 && ret != -3)) {
            return ret;
        }
        func_02074860(buf, size);
        buf += ret;
        total += ret;
    }
    return total;
}
