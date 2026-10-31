extern char data_020ce03c[];
extern char data_020ce058[];
extern const char data_0209ad24[];
extern const char data_0209ad2c[];
extern const char data_0209ad34[];
extern const char data_0209ad3c[];
extern void *func_02013514(void *p);
extern void *func_02011264(void);
extern void *func_0201123c(void *p, int v);
extern void *func_02022fc0(void *p, void *name);
extern void func_02022f2c(void *p, void *name, int v);
extern int func_020790c8(const void *a, const void *b);
extern void func_0203e4a4(void *p);
extern void func_0203e508(void *p);

void func_0203e538(int param_1, int param_2, void *param_3)
{
    int z = 0;
    void *s;
    void *obj;

    s = func_02013514(param_3);
    obj = func_02022fc0(func_0201123c(func_02011264(), 0), data_020ce03c);
    if (obj != 0) {
        if (func_020790c8(s, data_0209ad24) == 0) {
            func_0203e4a4(obj);
        } else if (func_020790c8(s, data_0209ad2c) == 0) {
            func_0203e508(obj);
        }
    } else if (func_020790c8(s, data_0209ad34) == 0 || func_020790c8(s, data_0209ad24) == 0) {
        func_02022f2c(func_0201123c(func_02011264(), 0), data_020ce058, 0);
    } else if (func_020790c8(s, data_0209ad3c) == 0) {
        func_02022f2c(func_0201123c(func_02011264(), z), data_020ce058, 1);
    }
}
