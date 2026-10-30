typedef struct Thread {
    char before[0x84];
    void *mutex;
} Thread;
typedef struct {
    int flags;
    Thread *current;
} ThreadInfo;

extern ThreadInfo data_020d0f28;
extern int func_02075c98(void);
extern int func_02074630(void *mutex);
extern void func_020740d0(void *queue);
extern int func_02075cac(int enabled);

static inline Thread *current_thread(void)
{
    return data_020d0f28.current;
}

void func_02074588(void *mutex)
{
    int enabled = func_02075c98();
    Thread *current = current_thread();

    for (;;) {
        if (func_02074630(mutex) != 0) {
            break;
        }
        current->mutex = mutex;
        func_020740d0(mutex);
        current->mutex = 0;
    }
    func_02075cac(enabled);
}
