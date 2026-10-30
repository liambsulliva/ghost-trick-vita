extern void func_02010384();
extern int func_02008444();
extern void func_02011598();
extern int func_02011264();
extern int func_0201123c();
extern int func_02022f04();
extern int func_02008828();
extern void func_02022f34();
extern char data_020cd85c;
extern char data_02083ed8;

void func_020123a0(char *param_1, int param_2)
{
    int flag;
    char *obj;

    flag = 1;
    func_02010384(param_1, 1);
    switch (param_2) {
    case 0:
        obj = (char *)func_02008444();
        func_02011598(obj);
        flag = 0;
        obj = (char *)func_02011264();
        obj = (char *)func_0201123c(obj, 0);
        obj = (char *)func_02022f04(obj, -1);
        if (obj != 0 && func_02008828(obj, &data_020cd85c) != 0) {
            flag = 1;
        }
        obj = (char *)func_02011264();
        obj = (char *)func_0201123c(obj, 0);
        func_02022f34(obj, &data_02083ed8, flag);
        break;
    case 1:
        break;
    case 2:
        break;
    }
}
