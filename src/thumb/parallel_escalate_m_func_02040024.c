struct Task {
    int head;
    unsigned char state;
    unsigned char pad[27];
    void *child;
};
extern char data_020cf9fc;
extern char data_0209b138;
extern unsigned int *func_02008444(void);
extern void func_020113f8(unsigned int *object);
extern void func_02011404(unsigned int *object);
extern void *func_02009900(void);
extern void func_0202fad4(void *object, void *data);
extern void *func_0202f624(int size);
extern void *func_020400c4(void *object);
extern void func_0202f7e8(void *param_1, void *param_2, unsigned int param_3, int param_4);
extern int func_0205bf2c(void *object);
extern void *func_02011264(void);
extern void *func_0201123c(void *object, int value);
extern void func_02022f34(void *object, void *data, int value);

void func_02040024(struct Task *task)
{
    void *child;

    func_020113f8(func_02008444());
    switch (task->state) {
    case 0:
        func_0202fad4(func_02009900(), &data_020cf9fc);
        task->state++;
        return;
    case 1:
        child = func_0202f624(0xbc);
        if (child != 0) {
            child = func_020400c4(child);
        }
        task->child = child;
        if (child != 0) {
            func_0202f7e8(func_02009900(), child, 1, 0);
        }
        task->state = 2;
        return;
    case 2:
        if (task->child == 0 || func_0205bf2c(task->child) != 0) {
            func_02011404(func_02008444());
            task->state = 99;
        }
        return;
    default:
        if (task->child == 0 || func_0205bf2c(task->child) != 0) {
            func_02022f34(func_0201123c(func_02011264(), 1), &data_0209b138, 0);
        }
        return;
    }
}
