typedef struct Thread Thread;
typedef struct Mutex Mutex;

typedef struct ThreadQueue {
    Thread *head;
    Thread *tail;
} ThreadQueue;

typedef struct ThreadLink {
    Thread *prev;
    Thread *next;
} ThreadLink;

typedef struct MutexQueue {
    Mutex *head;
    Mutex *tail;
} MutexQueue;

typedef struct Context {
    unsigned int cpsr;
    unsigned int r[13];
    unsigned int sp;
    unsigned int lr;
    unsigned char rest[0x24];
} Context;

struct Thread {
    Context context;
    unsigned int state;
    Thread *next;
    unsigned int id;
    unsigned int priority;
    void *profiler;
    ThreadQueue *queue;
    ThreadLink link;
    Mutex *mutex;
    MutexQueue mutexQueue;
    unsigned int stackTop;
    unsigned int stackBottom;
    unsigned int stackWarningOffset;
    ThreadQueue joinQueue;
    void *specific[3];
    void *alarmForSleep;
    void *destructor;
};

extern int func_02075c98(void);
extern int func_02075cac(int enable);
extern int func_02073ad8(void);
extern void func_02073c20(Thread *thread);
extern void func_02074404(void *context, unsigned int newpc, unsigned int newsp);
extern void func_02072ec4(unsigned int data, void *destp, unsigned int size);
extern void func_020743fc(Thread *thread, void *destructor);
extern void func_02073fc4(void);

void func_02073ed4(Thread *thread, void (*func)(void *), void *arg, void *stack, unsigned int stackSize, unsigned int prio)
{
    int enable;
    int index;

    enable = func_02075c98();
    index = func_02073ad8();

    thread->id = (unsigned int)index;
    thread->priority = prio;
    thread->state = 0;
    thread->profiler = 0;

    func_02073c20(thread);

    thread->stackBottom = (unsigned int)stack;
    thread->stackWarningOffset = 0;
    thread->stackTop = (unsigned int)stack - stackSize;

    *(unsigned int *)(thread->stackBottom - 8) = 0xfddb597d;
    *(unsigned int *)thread->stackTop = 0x7bf9dd5b;

    thread->joinQueue.head = thread->joinQueue.tail = 0;

    func_02074404(&thread->context, (unsigned int)func, (unsigned int)stack - 8);
    thread->context.r[0] = (unsigned int)arg;
    thread->context.lr = (unsigned int)func_02073fc4;

    func_02072ec4(0, (void *)((unsigned int)stack - stackSize + 4), stackSize - 12);

    thread->mutex = 0;
    thread->mutexQueue.head = 0;
    thread->mutexQueue.tail = 0;

    func_020743fc(thread, 0);

    thread->queue = 0;
    thread->link.prev = thread->link.next = 0;

    func_02072ec4(0, thread->specific, 12);

    thread->alarmForSleep = 0;

    func_02075cac(enable);
}
