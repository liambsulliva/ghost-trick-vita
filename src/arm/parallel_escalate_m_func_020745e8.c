typedef struct {
    int head;
    int tail;
    void *thread;
    unsigned int count : 24;
    unsigned int type : 8;
} Mutex;

extern Mutex *func_02073bf0(int *queue);
extern void func_02074120(Mutex *queue);

void func_020745e8(int param_1)
{
    Mutex *mutex;

    if (*(int *)(param_1 + 0x88) == 0) {
        return;
    }
    do {
        mutex = func_02073bf0((int *)(param_1 + 0x88));
        mutex->count = 0;
        mutex->thread = 0;
        mutex->type = 0;
        func_02074120(mutex);
    } while (*(int *)(param_1 + 0x88) != 0);
}
