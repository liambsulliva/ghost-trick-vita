extern void *func_02008444(void);
extern int func_020322d4(void *p);
extern void *func_0200883c(void);
extern void *func_02008824(void *p);
extern unsigned int func_0201e888();
extern void *func_02009c1c(void);
extern int func_0200cb5c(void *p);
extern void func_0202f1bc();

void func_02040774(int param_1, const unsigned int *param_2)
{
    int flag;
    unsigned int id;
    unsigned int arg;
    unsigned int obj;

    flag = func_020322d4(func_02008444());
    for (; (id = *param_2) != 0; param_2 += 4) {
        if (flag != 0 && id == 0xe && (arg = param_2[1]) == 0) {
            obj = func_0201e888(func_02008824(func_0200883c()), (unsigned short)arg, 0x11, 1, 0);
        } else {
            obj = func_0201e888(func_02008824(func_0200883c()), (unsigned short)param_2[1], (unsigned short)id, 1, 0);
        }
        if (obj != 0) {
            void *mgr = func_02009c1c();
            func_0202f1bc(mgr, obj, param_2[2], func_0200cb5c(func_02008824(func_0200883c())), param_2[3]);
        }
    }
}
