extern int func_0201123c(int object, int index);
extern int func_02011264(void);
extern int func_02011320(void *object);
extern int func_0201132c(void *object);
extern void func_02022f2c(int object, void *name, int flag);
extern char data_020cd96c;
extern char data_020cd7ec;

void func_020112d4(void *object)
{
    int state;

    state = func_02011320(object);
    if (state != 2 && state != 3) {
        return;
    }
    if (func_0201132c(object) != 0) {
        func_02022f2c(func_0201123c(func_02011264(), 0), &data_020cd96c, 0);
        return;
    }
    func_02022f2c(func_0201123c(func_02011264(), 0), &data_020cd7ec, 0);
}
