typedef struct Thread {
    char pad[0xb0];
    void *alarmForSleep;
} Thread;

typedef struct {
    void *switchThreadCallback;
    unsigned int rescheduleCount;
    Thread **currentThreadPtr;
} ThreadGlobals;

typedef struct {
    int words[11];
} Alarm;

extern ThreadGlobals data_020d0f0c;
extern void func_020754f4(Alarm *alarm);
extern unsigned int func_02075c98(void);
extern unsigned int func_02075cac(unsigned int bits);
extern void func_020740d0(void *queue);
extern void func_02074340(void *arg);
extern void func_0207562c(Alarm *alarm, unsigned long long tick, void (*handler)(void *), void *arg);

void func_020742a0(unsigned int msec)
{
    Alarm alarm;
    Thread *thread;
    unsigned int bak;

    func_020754f4(&alarm);
    thread = *data_020d0f0c.currentThreadPtr;
    bak = func_02075c98();
    thread->alarmForSleep = &alarm;
    func_0207562c(&alarm, ((unsigned long long)msec * 33514) / 64, func_02074340, &thread);
    while (thread != 0) {
        func_020740d0(0);
    }
    func_02075cac(bak);
}
