#pragma opt_common_subs off
#pragma opt_propagation off
typedef struct Bank0200128c {
    char pad0[8];
    unsigned int size;
    char padc[0x2c];
    unsigned int count;
    unsigned int table[1];
} Bank0200128c;

struct Scan0200128c {
    unsigned int state[2];
    unsigned char flag;
    unsigned char pad;
    unsigned short f1;
    unsigned short f2;
    unsigned short pad2;
    unsigned int tail;
};

typedef struct Item0200128c {
    unsigned int key;
    unsigned int length;
} Item0200128c;

typedef struct List0200128c {
    int count;
    Item0200128c items[1];
} List0200128c;

struct State0200128c {
    char pad0[0x1540];
    unsigned int bits[16];
    char pad1580[0x24];
    unsigned int flags;
    char pad15a8[0x604];
    List0200128c *list_a;
    List0200128c *list_b;
};

struct Root0200128c {
    char pad0[0x10];
    struct State0200128c *state;
};

extern struct Root0200128c data_020b18a0;
extern void func_02000f70(void *dest, int value, int size);
extern void func_02077518();
extern int func_02077538();

void func_0200128c(Bank0200128c *a, Bank0200128c *b, unsigned char c)
{
    struct Scan0200128c s;
    unsigned int entry;
    unsigned char *p;
    int idx;
    int val;
    unsigned int cur;
    unsigned int next;
    unsigned int length;
    List0200128c *list;
    Item0200128c *item;
    unsigned short n;
    int w;
    int bit;
    unsigned int word;
    unsigned int m;
    int k;
    int k2;
    int k3;
    unsigned int off;
    Item0200128c *ents;
    unsigned int cnt;
    unsigned int *bits;
    unsigned int *wp;
    struct State0200128c *state;

    func_02000f70(data_020b18a0.state->bits, 0, sizeof(data_020b18a0.state->bits));
    func_02077518(&s, a);
    while (func_02077538(a, &s.flag, &s) != 0) {
        if (s.flag != 1 || s.f2 != c) {
            continue;
        }
        if ((data_020b18a0.state->bits[(int)(unsigned int)s.f1 / 32] >> (s.f1 - (int)(unsigned int)s.f1 / 32 * 32)) & 1) {
            continue;
        }
        bits = data_020b18a0.state->bits;
        w = (int)(unsigned int)s.f1 / 32;
        n = s.f1;
        bits[w] |= 1 << (n - w * 32);
        if (s.state[1] != 0) {
            idx = s.state[0];
            entry = a->table[idx];
            if ((entry & 0xff) == 0x10) {
                val = ((unsigned char *)a)[(entry >> 8) & 0xffff];
                val = val + s.state[1] - 1;
            } else {
                p = (unsigned char *)a + ((entry >> 8) & 0xffff);
                if (s.state[1] != 0) {
                    val = p[s.state[1] - 1];
                } else {
                    val = 0xff;
                }
            }
        } else {
            idx = s.state[0] - 1;
            val = 0;
        }
        k = s.f1;

        cur = b->table[k];
        if (k < b->count - 1) {
            k2 = k + 1;
            next = b->table[k2];
        } else {
            next = b->size;
        }
        length = next - cur;
        if ((data_020b18a0.state->flags & 0x40000000) == 0) {
            list = data_020b18a0.state->list_a;
            ents = list->items;
        } else {
            list = data_020b18a0.state->list_b;
            ents = list->items;
        }
        cnt = list->count;
        item = (Item0200128c *)(int)(long long)(int)(ents + cnt);
        list->count = cnt + 1;
        item->key = (c << 24) | ((unsigned char)val | ((unsigned short)idx << 8));
        item->length = length;
    }
    func_02000f70(data_020b18a0.state->bits, 0, sizeof(data_020b18a0.state->bits));
}

