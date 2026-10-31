extern void func_0200c71c(unsigned char *param_1);
extern void func_0200c728(unsigned char *param_1, int param_2);
extern void func_0200c72c(int param_1, int param_2);
extern unsigned char *func_0200cd90(unsigned char *param_1, unsigned int param_2, int param_3, unsigned int param_4, unsigned int param_5);
extern unsigned char *func_02031254(int param_1);

void func_0200c680(int param_1, int param_2, unsigned int param_3, int param_4, int param_5)
{
    unsigned int count;
    unsigned char *obj;
    unsigned int size;
    int kind;

    func_0200c72c(param_1, param_2);
    switch (param_5) {
    case 0:
    case 0x10:
    default:
        size = 0x8000;
        count = 1;
        break;
    case 0x100010:
        size = 0x10000;
        count = 2;
        break;
    case 0x200010:
        size = 0x20000;
        count = 4;
        break;
    case 0x300010:
        size = 0x40000;
        count = 8;
        break;
    }
    obj = func_02031254(0x48);
    if (obj != 0) {
        obj = func_0200cd90(obj, param_3, param_4, size, count);
    }
    if (obj != 0) {
        func_0200c71c(obj);
        kind = 5;
        if (param_2 != 0) {
            kind = 0x13;
        }
        func_0200c728(obj, kind);
        *(unsigned char **)(param_1 + param_2 * 4 + 0x1510) = obj;
    }
}
