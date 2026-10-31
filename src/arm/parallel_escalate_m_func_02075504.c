typedef unsigned long long OSTick;

typedef struct OSAlarm OSAlarm;
struct OSAlarm {
    void *handler;
    void *arg;
    unsigned int tag;
    OSTick fire;
    OSAlarm *prev;
    OSAlarm *next;
    OSTick period;
    OSTick start;
};

struct AlarmQueue {
    int pad00;
    OSAlarm *head;
    OSAlarm *tail;
};

extern struct AlarmQueue data_020d1260;
extern OSTick func_020753bc(void);
extern void func_0207545c(OSAlarm *alarm);

void func_02075504(OSAlarm *alarm, OSTick fire)
{
    OSAlarm *prev;
    OSAlarm *next;

    if (alarm->period > 0) {
        OSTick tick = func_020753bc();

        fire = alarm->start;
        if (alarm->start < tick) {
            fire += alarm->period * ((tick - alarm->start) / alarm->period + 1);
        }
    }

    alarm->fire = fire;

    for (prev = 0, next = data_020d1260.head; next; prev = next, next = next->next) {
        if ((long long)(fire - next->fire) < 0) {
            alarm->prev = next->prev;
            next->prev = alarm;
            alarm->next = next;
            prev = alarm->prev;
            if (prev) {
                prev->next = alarm;
            } else {
                data_020d1260.head = alarm;
                func_0207545c(alarm);
            }
            return;
        }
    }

    alarm->next = 0;
    prev = data_020d1260.tail;
    data_020d1260.tail = alarm;
    alarm->prev = prev;
    if (prev) {
        prev->next = alarm;
    } else {
        data_020d1260.head = data_020d1260.tail = alarm;
        func_0207545c(alarm);
    }
}
