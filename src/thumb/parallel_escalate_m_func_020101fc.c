extern char data_020ce7d4;
extern unsigned char *func_02009900();
extern void func_0202fad4();
extern unsigned char *func_0202f624();
extern unsigned char *func_02010240();
extern void func_02010260();
extern void func_0202f7e8();

void func_020101fc(unsigned int param_1, int param_2)
{
    unsigned char *obj;

    if (param_2 != 0) {
        obj = func_0202f624(0x34);
        if (obj != 0) {
            obj = func_02010240(obj);
        }
        if (obj != 0) {
            func_02010260(obj, param_2);
            func_0202f7e8(func_02009900(), obj, 1, 0);
        }
    } else {
        func_0202fad4(func_02009900(), &data_020ce7d4);
    }
}
