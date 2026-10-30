extern void *func_0200883c(void);
extern void *func_02008824(void *);
extern int func_0200cb5c(void *);
extern void *func_02009c1c(void);
extern void func_0202f1bc(void *, int, int, int, int);
extern unsigned short *func_0201e888(void *, unsigned short, unsigned short, int, int);

void func_ov006_020d7838(int param_1, unsigned int param_2, unsigned int param_3, int param_4, int param_5, unsigned int param_6)
{
    unsigned short *table;
    int r6;
    unsigned int i;

    table = func_0201e888(func_02008824(func_0200883c()), (unsigned short)param_3, (unsigned short)param_2, 1, 0);
    r6 = func_0200cb5c(func_02008824(func_0200883c()));
    if (param_6 != 0) {
        for (i = 0; i < ((unsigned int)r6 >> 1); i++) {
            table[i] |= (unsigned short)(param_6 << 12);
        }
    }
    func_0202f1bc(func_02009c1c(), (int)table, param_5, r6, param_4);
}
