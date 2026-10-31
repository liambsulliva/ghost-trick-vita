typedef int code();

typedef struct {
    code *pc0;
    code *pc1;
    int result;
    unsigned int u3;
    unsigned int u4;
    unsigned int u5;
    unsigned int u6;
    unsigned int u7;
    unsigned int u8;
} Task;

typedef struct {
    char pad[0xc0];
    Task *task;
} Worker;

extern char data_020d30c8[];
extern void func_02072fa8(void *p, int v, unsigned int n);
extern int func_02075c98(void);
extern int func_02075cac(int enabled);
extern void func_020740d0(void *queue);
extern void func_02073fc4(void);

void func_0207a264(Worker *w)
{
    Task task;
    int enabled;

    while (1) {
        func_02072fa8(&task, 0, sizeof(task));
        enabled = func_02075c98();
        while (*(int *)((char *)w + 0xc0) == 0) {
            func_020740d0(0);
        }
        task = *w->task;
        func_02075cac(enabled);
        if (task.pc0 != 0) {
            task.result = (*task.pc0)(&task);
        }
        enabled = func_02075c98();
        data_020d30c8[0x26] = 0;
        if (task.pc1 != 0) {
            (*task.pc1)(&task);
        }
        if (*(int *)data_020d30c8 == 0) {
            break;
        }
        w->task = 0;
        func_02075cac(enabled);
    }
    func_02073fc4();
}
