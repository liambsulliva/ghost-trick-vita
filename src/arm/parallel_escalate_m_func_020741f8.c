typedef struct Thread {
    unsigned char context[0x64];
    unsigned int state;
    struct Thread *next;
    unsigned int id;
    unsigned int priority;
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
extern Thread data_020d0f38;
extern unsigned int func_02075c98(void);
extern unsigned int func_02075cac(unsigned int bits);
extern void func_02073c20(Thread *thread);
extern void func_02073cc4(void);

int func_020741f8(Thread *thread, unsigned int prio)
{
    Thread *t = data_020d0f0c.info.list;
    Thread *pre = 0;
    unsigned int enable;

    enable = func_02075c98();

    while (t && t != thread) {
        pre = t;
        t = t->next;
    }

    if (!t || t == &data_020d0f38) {
        func_02075cac(enable);
        return 0;
    }

    if (t->priority != prio) {
        if (!pre) {
            data_020d0f0c.info.list = thread->next;
        } else {
            pre->next = thread->next;
        }
        thread->priority = prio;
        func_02073c20(thread);
        func_02073cc4();
    }

    func_02075cac(enable);
    return 1;
}
