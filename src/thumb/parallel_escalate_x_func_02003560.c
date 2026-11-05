#pragma opt_common_subs off
#pragma opt_propagation off
typedef struct Iter03560 {
    unsigned int index;
    int pos;
} Iter03560;

typedef struct Entry03560 {
    unsigned char kind;
    char pad1[3];
    unsigned short id;
    char pad6[6];
} Entry03560;

typedef struct Archive03560 {
    char pad[0x3c];
    unsigned int types[1];
} Archive03560;

typedef struct Bank03560 {
    char pad[0x13e0];
    unsigned int bits[0x1f1];
    unsigned int count;
    unsigned char list[4];
} Bank03560;

typedef struct Game03560 {
    char pad[0x10];
    Bank03560 *bank;
} Game03560;

extern Game03560 data_020b18a0;
extern void func_02077518(Iter03560 *it, Archive03560 *arc);
extern int func_02077538(Archive03560 *arc, Entry03560 *entry, Iter03560 *it);

void func_02003560(Archive03560 *arc, int id)
{
    Entry03560 entry;
    Iter03560 iter;
    unsigned int i;
    unsigned int w;
    char *b;
    unsigned int n;
    unsigned int found;

    func_02077518(&iter, arc);
    while (func_02077538(arc, &entry, &iter) != 0) {
        if (entry.kind == 1 && entry.id == id && (unsigned char)arc->types[iter.index] == 0x10) {
            b = (char *)data_020b18a0.bank + iter.index / 32 * 4; w = iter.index / 32; if ((*(unsigned int *)(b + 0x13e0) >> (iter.index - w * 32)) & 1) {
                n = data_020b18a0.bank->count;
                if (n > 4) {
                    n = 4;
                }
                found = 0xff;
                for (i = 0; i < n; i++) {
                    if ((unsigned char)*(unsigned int *)&iter == (*(Bank03560 **)((char *)&data_020b18a0 + 0x10))->list[i]) {
                        found = i;
                        break;
                    }
                }
                if (found == 0xff) {
                    if ((*(Bank03560 **)((char *)&data_020b18a0 + 0x10))->count < 4) {
                        (*(Bank03560 **)((char *)&data_020b18a0 + 0x10))->list[(*(Bank03560 **)((char *)&data_020b18a0 + 0x10))->count] = *(unsigned int *)&iter;
                    }
                    (*(Bank03560 **)((char *)&data_020b18a0 + 0x10))->count++;
                }
            }
        }
    }
}
