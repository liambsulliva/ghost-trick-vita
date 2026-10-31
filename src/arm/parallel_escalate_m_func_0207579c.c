typedef void (*OSAlarmHandler)(void *arg);

typedef struct OSAlarm OSAlarm;
struct OSAlarm {
    OSAlarmHandler handler;
    void *arg;
    unsigned int tag;
    unsigned long long fire;
    OSAlarm *prev;
    OSAlarm *next;
    unsigned long long period;
    unsigned long long start;
};

typedef struct OSiAlarmQueue {
    int use;
    OSAlarm *head;
    OSAlarm *tail;
} OSiAlarmQueue;

extern OSiAlarmQueue data_020d1260;
extern char data_027e0000;
extern unsigned int func_020735f0(unsigned int mask);
extern unsigned long long func_020753bc(void);
extern void func_0207545c(OSAlarm *alarm);
extern void func_02075504(OSAlarm *alarm, unsigned long long fire);

void func_0207579c(void)
{
    unsigned long long tick;
    OSAlarm *alarm;
    OSAlarmHandler handler;
    OSAlarm *next;

    *(volatile unsigned short *)0x04000106 = 0;
    (void)func_020735f0(0x10);
    *(unsigned int *)(((int)&data_027e0000) + 0x3ff8) = *(unsigned int *)(((int)&data_027e0000) + 0x3ff8) | 0x10;

    tick = func_020753bc();
    alarm = data_020d1260.head;

    if (!alarm) {
        return;
    }

    if (tick < alarm->fire) {
        func_0207545c(alarm);
        return;
    }

    next = alarm->next;
    data_020d1260.head = next;
    if (next == 0) {
        data_020d1260.tail = 0;
    } else {
        next->prev = 0;
    }

    handler = alarm->handler;
    if (!alarm->period) {
        alarm->handler = 0;
    }

    if (handler) {
        (handler)(alarm->arg);
    }

    if (alarm->period) {
        alarm->handler = handler;
        func_02075504(alarm, 0);
    }

    if (data_020d1260.head) {
        func_0207545c(data_020d1260.head);
    }
}
