#pragma opt_common_subs off
#pragma opt_propagation off
typedef struct Bank02000e28 {
    char pad0[8];
    unsigned int size;
    char padc[0x2c];
    unsigned int count;
    unsigned int table[1];
} Bank02000e28;

struct Scan02000e28 {
    unsigned int state[2];
    unsigned char flag;
    unsigned char pad;
    unsigned short f1;
    unsigned short f2;
    unsigned short pad2;
    unsigned int tail;
};

struct State02000e28 {
    char pad0[0x13e0];
    unsigned int fill_13e0[8];
    unsigned int fill_1400[32];
    unsigned int fill_1480[32];
    char pad1500[0x67d];
    unsigned char byte_1b7d;
};

struct Root02000e28 {
    char pad0[0x10];
    struct State02000e28 *state;
};

extern struct Root02000e28 data_020b18a0;
extern void func_02000f70(void *dest, int value, int size);
extern void func_02077518();
extern int func_02077538();
extern int func_02077718();
extern int func_0200286c(Bank02000e28 *a, struct Scan02000e28 *s);

unsigned int func_02000e28(Bank02000e28 *a, Bank02000e28 *b, unsigned short c)
{
    struct Scan02000e28 s;
    unsigned int total;
    unsigned int idx;
    unsigned int kind;
    unsigned int w;
    int n;
    int q;
    unsigned int sum;
    unsigned int cur;
    unsigned int next;
    unsigned int *flags;

    total = 0;
    func_02077518(&s, a);
    while (func_02077538(a, &s.flag, &s) != 0) {
        if (s.flag != 1 || s.f2 != c) {
            continue;
        }
        idx = s.state[1] != 0 ? s.state[0] : s.state[0] - 1;
        kind = a->table[idx] & 0xff;
        if (data_020b18a0.state->byte_1b7d != 0) {
            if (kind == 0x10) {
                w = idx / 32;
                if (((data_020b18a0.state->fill_13e0[w] >> (idx - w * 32)) & 1) == 0) {
                    continue;
                }
                if (func_0200286c(a, &s) == 0) {
                    continue;
                }
            } else {
                w = idx / 32;
                if (((data_020b18a0.state->fill_13e0[w] >> (idx - w * 32)) & 1) == 0) {
                    continue;
                }
            }
        }
        if ((data_020b18a0.state->fill_1480[(int)(unsigned int)s.f1 / 32] >> (s.f1 - (int)(unsigned int)s.f1 / 32 * 32)) & 1) {
            continue;
        }
        if (func_02077718(b, s.f1) != 0) {
            continue;
        }
        sum = b->count + s.f1;
        cur = *(unsigned int *)((char *)b + sum * 4 + 0x3c);
        if (s.f1 < b->count - 1) {
            next = *(unsigned int *)((char *)b + (sum + 1) * 4 + 0x3c);
        } else {
            next = b->size;
        }
        total += next - cur;
        flags = data_020b18a0.state->fill_1480;
        q = (int)(unsigned int)s.f1 / 32;
        n = s.f1;
        flags[q] |= 1 << (n - q * 32);
    }
    func_02000f70(data_020b18a0.state->fill_1480, 0, sizeof(data_020b18a0.state->fill_1480));
    return total;
}
