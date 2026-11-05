#pragma opt_propagation off
typedef struct Bank02001c64 {
    char pad[0x1500];
    unsigned int bits[0x1a6];
    unsigned int last;
    unsigned int kind;
    int count;
} Bank02001c64;

typedef struct Game02001c64 {
    char pad[0x10];
    Bank02001c64 *bank;
} Game02001c64;

extern Game02001c64 data_020b18a0;
extern unsigned short *func_02006c90(unsigned int id);

int func_02001c64(unsigned int id)
{
    unsigned short *info;
    Bank02001c64 *bank;
    unsigned int idx;
    unsigned int *bits;
    unsigned int word;

    info = func_02006c90(id);
    if (info == 0) {
        return 0;
    }
    bank = data_020b18a0.bank;
    if (bank->count == 0) {
        bank->kind = *info;
    } else if (bank->kind != *info) {
        return 0;
    }
    bank = data_020b18a0.bank;
    idx = id >> 5;
    if (((bank->bits[idx] >> (id - (idx << 5))) & 1) == 0) {
        bank->count++;
    }
    bits = data_020b18a0.bank->bits;
    word = id / 32;
    bits[word] |= 1 << (id - word * 32);
    data_020b18a0.bank->last = id;
    return 1;
}
