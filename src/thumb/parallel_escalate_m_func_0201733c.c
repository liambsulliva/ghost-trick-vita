extern int data_020b9d10;
extern int data_020b9d04;
extern void *func_0200883c(void);
extern int func_02008824(void *object);
extern int func_0201e888(int a, int b, int c, int d, int e);
extern int *func_02009c1c(void);
extern void func_0202f1bc(int *table, int a, int b, int c, int d);

void func_0201733c(unsigned char *object, int kind, int value)
{
    int id;
    id = func_0201e888(func_02008824(func_0200883c()), 0, value, 1, 0);
    if (object[0x23] == 1) {
        if (kind == 1) {
            func_0202f1bc(func_02009c1c(), id, data_020b9d10, 0x20, 0x12);
            return;
        }
        func_0202f1bc(func_02009c1c(), id, data_020b9d04, 0x20, 4);
        return;
    }
    if (object[0x23] == 0) {
        if (object[0x22] == 1) {
            func_0202f1bc(func_02009c1c(), id, 0, 0x20, 0x15);
            return;
        }
        func_0202f1bc(func_02009c1c(), id, 0, 0x20, 7);
    }
}
