struct Alarm {
    void *handler;
    void *arg;
    unsigned int tag;
    unsigned long long fire;
    struct Alarm *prev;
    struct Alarm *next;
    unsigned long long period;
    unsigned long long start;
};

struct AlarmQueue {
    int pad00;
    struct Alarm *head;
    struct Alarm *tail;
};

extern struct AlarmQueue data_020d1260;
extern int func_02075c98(void);
extern void func_02075cac(int state);
extern void func_0207545c(struct Alarm *alarm);

void func_02075708(struct Alarm *alarm)
{
    int state;
    struct Alarm *next;

    state = func_02075c98();
    if (alarm->handler == 0) {
        func_02075cac(state);
        return;
    }
    next = alarm->next;
    if (next == 0) {
        data_020d1260.tail = alarm->prev;
    } else {
        next->prev = alarm->prev;
    }
    if (alarm->prev != 0) {
        alarm->prev->next = next;
    } else {
        data_020d1260.head = next;
        if (next != 0) {
            func_0207545c(next);
        }
    }
    alarm->handler = 0;
    alarm->period = 0;
    func_02075cac(state);
}
