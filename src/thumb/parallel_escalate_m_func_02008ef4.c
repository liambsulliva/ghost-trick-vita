extern void func_0200965c(char *object, int value);
extern void func_02007bd8(void);
extern char *func_02007b70(void);
extern void func_02073ed4(char *a, int b, char *c, char *d, unsigned int e, int f);
extern void func_02074188(char *a);
struct Bank {
    unsigned int size;
    unsigned int extra;
};
extern struct Bank data_02083a90[];
extern char data_02083a94;
extern void func_02008ee8(void);
int func_02008ef4(char *param_1, int param_2, unsigned int param_3, unsigned int param_4)
{
    unsigned int table_value;
    char *obj;
    func_0200965c(param_1, 0);
    table_value = data_02083a90[param_4].size;
    if (param_3 > table_value - 0x20) {
        *(unsigned char *)(param_1 + 6) = 2;
        return 2;
    }
    *(unsigned char *)(param_1 + 6) = 0;
    if (*(unsigned int *)(param_1 + 0xc) != 0) {
        func_02007bd8();
        if (func_02007b70() != 0) {
            func_02007bd8();
            obj = func_02007b70();
            (*(void (**)(char *, unsigned int))(*(int *)obj + 0x10))(obj, *(unsigned int *)(param_1 + 0xc));
            *(unsigned int *)(param_1 + 0xc) = 0;
        }
    }
    func_02007bd8();
    obj = func_02007b70();
    *(unsigned int *)(param_1 + 0xc) = (*(unsigned int (**)(char *, unsigned int, unsigned int))(*(int *)obj + 8))(obj, table_value, (unsigned int)&data_02083a94);
    *(unsigned int *)(param_1 + 0x10) = param_2;
    *(unsigned int *)(param_1 + 0x14) = param_3;
    *(unsigned char *)(param_1 + 8) = (unsigned char)param_4;
    *(unsigned char *)(param_1 + 0x14dc) = 0;
    *(unsigned char *)(param_1 + 0x14dd) = (unsigned char)*(unsigned int *)(param_1 + (param_4 << 5) + 0x14e0);
    func_02073ed4(param_1 + 0x41c, (int)func_02008ee8, param_1, param_1 + 0x14dc, 0x1000, 5);
    func_02074188(param_1 + 0x41c);
    return 1;
}
