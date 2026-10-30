typedef struct VAlarm {
    char pad[0x14];
    struct VAlarm *prev;
    struct VAlarm *next;
} VAlarm;

typedef struct {
    char pad[0xc];
    VAlarm *head;
    VAlarm *tail;
} VAlarmList;

extern VAlarmList data_020d126c;

void func_0207597c(VAlarm *alarm)
{
    VAlarm *prev;
    VAlarm *next;

    if (alarm != 0) {
        next = alarm->next;
        prev = alarm->prev;
        if (next != 0) {
            next->prev = prev;
        } else {
            data_020d126c.tail = prev;
        }
        if (prev != 0) {
            prev->next = next;
        } else {
            data_020d126c.head = next;
        }
    }
}
