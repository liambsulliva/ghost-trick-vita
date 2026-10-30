typedef struct Thread {
    char context[0x64];
    int state;
} Thread;

typedef void (*SwitchCallback)(Thread *from, Thread *to);

typedef struct {
    unsigned short isNeedRescheduling;
    unsigned short irqDepth;
    Thread *current;
    Thread *list;
    SwitchCallback switchCallback;
} ThreadInfo;

typedef struct {
    SwitchCallback switchThreadCallback;
    unsigned int rescheduleCount;
    Thread **currentThreadPtr;
    char pad[0x10];
    ThreadInfo info;
} ThreadGlobals;

extern ThreadGlobals data_020d0f0c;
extern ThreadInfo data_020d0f28;
extern int func_02075cfc(void);
extern Thread *func_020741b0(void);
extern int func_02074470(Thread *thread);
extern void func_020744bc(Thread *thread);

void func_02073cc4(void)
{
    ThreadInfo *info;
    Thread *current;
    Thread *next;

    if (data_020d0f0c.rescheduleCount != 0) {
        return;
    }
    info = &data_020d0f28;
    if (data_020d0f0c.info.irqDepth != 0 || func_02075cfc() == 0x12) {
        info->isNeedRescheduling = 1;
        return;
    }
    current = *data_020d0f0c.currentThreadPtr;
    next = func_020741b0();
    if (current == next || next == 0) {
        return;
    }
    if (current->state != 2 && func_02074470(current) != 0) {
        return;
    }
    if (data_020d0f0c.switchThreadCallback != 0) {
        data_020d0f0c.switchThreadCallback(current, next);
    }
    if (info->switchCallback != 0) {
        info->switchCallback(current, next);
    }
    data_020d0f0c.info.current = next;
    func_020744bc(next);
}
