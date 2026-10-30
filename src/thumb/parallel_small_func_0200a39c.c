extern int func_0201e2ec(char *a, int b, unsigned int c, int d, unsigned int e);
extern void func_0200a478(char *object);
extern void func_0200a424(char *object);
extern int func_02009900(void);
extern void func_0202fad4(int a, int b);
extern int func_020097d0(int a);
extern void func_0200a420(int a, char *b);
extern void func_0202f7e8(int a, int b, int c, int d);
void func_0200a39c(char *param_1, int param_2, unsigned int param_3)
{
    unsigned int product;
    int status;
    int flag;
    int value;
    status = func_0201e2ec(param_1 + 0xaa04, 0x100, param_3 >> 8, param_2, param_3);
    if (status != 0) {
        func_0200a478(param_1);
        product = *(unsigned char *)(param_1 + 0xaa3e) * *(unsigned char *)(param_1 + 0xaa3f);
        if ((product << 8) < (param_3 - 0x100)) {
            flag = 1;
        } else {
            flag = 0;
        }
        *(unsigned char *)(param_1 + 0xaa42) = flag;
        func_0202fad4(func_02009900(), 0x020b42bc);
        value = func_020097d0(0x020b42bc);
        if (value != 0) {
            func_0200a420(value, param_1);
            func_0202f7e8(func_02009900(), value, 0xe, 0);
        }
    } else {
        func_0200a424(param_1);
    }
}
