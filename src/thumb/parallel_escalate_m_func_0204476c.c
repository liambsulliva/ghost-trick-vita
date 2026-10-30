struct Global {
    int first;
    int second;
    unsigned int flags;
};
struct Flags {
    unsigned char on : 1;
    unsigned char rest : 7;
};
extern struct Global data_020cff60;
extern int data_0209b4f8[];
extern void *func_02008eb0(void);
extern unsigned int func_01ff80b8(void *object);

void func_0204476c(unsigned char *p)
{
    if (((struct Flags *)(p + 0xe0))->on) {
        if (p[0xed] != 0) {
            if ((data_020cff60.flags & 1) == 0) {
                *(int *)(p + 0xe4) = data_0209b4f8[p[0xec] * 2 + (func_01ff80b8(func_02008eb0()) & 1)];
                *(int *)(p + 0xe8) = data_0209b4f8[p[0xec] * 2 + (func_01ff80b8(func_02008eb0()) & 1)];
            }
            p[0xed]--;
        } else {
            ((struct Flags *)(p + 0xe0))->on = 0;
            *(int *)(p + 0xe4) = 0;
            *(int *)(p + 0xe8) = 0;
        }
    }
}
