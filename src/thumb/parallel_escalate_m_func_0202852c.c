struct Vector {
    int x;
    int y;
    int z;
};

extern unsigned char *func_02028170(unsigned char *, unsigned int, unsigned int);
extern int func_02008828(unsigned char *, int);
extern struct Vector *func_020083a0(unsigned char *);
extern void func_0205958c(struct Vector *, unsigned char *, unsigned int);
extern void func_02008378(struct Vector *, struct Vector *, struct Vector *);
extern int func_020285cc(int);
extern int data_020cf898[];

void func_0202852c(unsigned char *param_1, int param_2)
{
    struct Vector sum1;
    struct Vector sum2;
    struct Vector offset1;
    struct Vector offset2;
    unsigned char *first;
    unsigned char *second;
    struct Vector *pos;
    unsigned short *entry;
    int diff;

    entry = (unsigned short *)(*(int *)(param_1 + 4) + *(int *)(param_1 + 8) * 2);
    *(int *)(param_1 + 0x18) = (int)0xfffff000;
    first = func_02028170(param_1, entry[1], 0);
    if (first != 0) {
        if (func_02008828(first, (int)data_020cf898) != 0) {
            second = func_02028170(param_1, entry[3], 0);
            if (second != 0) {
                if (func_02008828(second, (int)data_020cf898) != 0) {
                    pos = func_020083a0(first);
                    func_0205958c(&offset1, first, entry[2]);
                    func_02008378(&sum1, pos, &offset1);
                    pos = func_020083a0(second);
                    func_0205958c(&offset2, second, entry[4]);
                    func_02008378(&sum2, pos, &offset2);
                    if (param_2 == 0) {
                        diff = sum2.x - sum1.x;
                    } else {
                        diff = sum2.y - sum1.y;
                    }
                    *(int *)(param_1 + 0x18) = func_020285cc(diff);
                }
            }
        }
    }
}
