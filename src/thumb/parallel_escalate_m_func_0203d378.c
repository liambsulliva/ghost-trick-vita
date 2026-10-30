extern char data_0209aae4;
extern char data_0209ab48;
extern unsigned int *func_02008444(void);
extern int func_0201fa54(unsigned int *object);
extern unsigned char *func_0200847c(void);
extern void func_0202a034(unsigned char *object, int value);
extern void *func_02022dfc(unsigned char *object);
extern void func_02022f34(void *object, void *data, int value);
extern void func_0203c604(unsigned char *object, int value);
extern unsigned char *func_020312e4(void);
extern void func_0203cbe0(unsigned char *object, int value);
extern void func_02011510(unsigned int *object);
extern void func_020125e8(unsigned int *object);
extern void func_02010298(unsigned int *object, void *data);

void func_0203d378(unsigned char *param_1)
{
    if (func_0201fa54(func_02008444()) == 0x12) {
        param_1[0x4d] = 1;
    } else {
        func_0202a034(func_0200847c(), 0x32);
        param_1[0x52] = 1;
    }
    func_02022f34(func_02022dfc(param_1), &data_0209aae4, 0);
    if (param_1[0x4d] != 0) {
        func_0203c604(param_1, 1);
        func_0203cbe0(func_020312e4(), 1);
    } else {
        func_0203c604(param_1, 0);
    }
    func_02011510(func_02008444());
    func_020125e8(func_02008444());
    func_02010298(func_02008444(), &data_0209ab48);
    param_1[0x4e] = 1;
}
