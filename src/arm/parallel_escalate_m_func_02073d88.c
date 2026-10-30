typedef struct ThreadQueue {
    struct Thread *head;
    struct Thread *tail;
} ThreadQueue;

typedef struct Thread {
    unsigned char context[0x64];
    unsigned int state;
    struct Thread *next;
    unsigned int id;
    unsigned int priority;
    void *profiler;
    unsigned char reserved[0x18];
    unsigned int stackTop;
    unsigned int stackBottom;
    unsigned int stackWarningOffset;
    ThreadQueue joinQueue;
} Thread;

typedef struct {
    unsigned short isNeedRescheduling;
    unsigned short irqDepth;
    Thread *current;
    Thread *list;
} ThreadInfo;

typedef struct {
    unsigned char reserved0[8];
    Thread **currentThreadPtr;
    unsigned int isInitialized;
    unsigned char reserved1[0xc];
    ThreadInfo info;
} ThreadStatics;

extern ThreadStatics data_020d0f0c;
extern Thread data_020d0ff8;
extern Thread data_020d0f38;
extern char data_020d1180[];
extern char data_00000000[];
extern char data_00000800[];
extern char data_027e0000[];
extern void func_02074360(void *callback);
extern void func_02074388(void *arg);
extern void func_02073ed4(Thread *thread,void (*func)(void *),void *arg,void *stack,unsigned int stackSize,unsigned int prio);

void func_02073d88(void)
{
    void *stackLo;

    if (data_020d0f0c.isInitialized) {
        return;
    }
    data_020d0f0c.isInitialized = 1;
    data_020d0f0c.currentThreadPtr = &data_020d0f0c.info.current;
    data_020d0ff8.priority = 16;
    data_020d0ff8.id = 0;
    data_020d0ff8.state = 1;
    data_020d0ff8.next = 0;
    data_020d0ff8.profiler = 0;
    data_020d0f0c.info.list = &data_020d0ff8;
    data_020d0f0c.info.current = &data_020d0ff8;
    stackLo = ((int)data_00000000 <= 0) ?
        (void *)(0x027e0080 - (int)data_00000000) :
        (void *)(((unsigned int)data_027e0000 + 0x3f80 - (int)data_00000800) - (int)data_00000000);
    data_020d0ff8.stackBottom = (unsigned int)data_027e0000 + 0x3f80 - (int)data_00000800;
    data_020d0ff8.stackTop = (unsigned int)stackLo;
    data_020d0ff8.stackWarningOffset = 0;
    *(unsigned int *)(data_020d0ff8.stackBottom - 8) = 0xfddb597d;
    *(unsigned int *)data_020d0ff8.stackTop = 0x7bf9dd5b;
    data_020d0ff8.joinQueue.head = data_020d0ff8.joinQueue.tail = 0;
    data_020d0f0c.info.isNeedRescheduling = 0;
    data_020d0f0c.info.irqDepth = 0;
    *(ThreadInfo **)0x02ffffa0 = &data_020d0f0c.info;
    func_02074360(0);
    func_02073ed4(&data_020d0f38,func_02074388,0,data_020d1180,200,31);
    data_020d0f38.priority = 32;
    data_020d0f38.state = 1;
}
