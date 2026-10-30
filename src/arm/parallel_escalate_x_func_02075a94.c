typedef struct {
    void (*handler)(void *arg);
    void *arg;
    int tag;
    int frame;
    short fire;
    short delay;
    void *prev;
    void *next;
    int period;
    int finish;
    int canceled;
} VAlarm;

typedef struct {
    char pad[8];
    int frame;
    VAlarm *head;
} VAlarmState;

extern VAlarmState data_020d126c;
extern char data_027e0000;
void func_020735f0(unsigned int mask);
void func_02073620(unsigned int mask);
int func_02075c48(int vcount);
int func_02075bfc(VAlarm *alarm, int frame, int vcount);
void func_02075a54(VAlarm *alarm);
void func_0207597c(VAlarm *alarm);
void func_020758c8(VAlarm *alarm);

void func_02075a94(void *arg)
{
    VAlarm *alarm;
    void (*handler)(void *arg);
    int dispstat;
    int vcount;
    int frame;
    unsigned int id;
    int now;

    id = 4;
    func_020735f0(id);
    *(volatile unsigned short *)(0x04000000 + id) &= ~0x20;
    *(unsigned int *)((int)&data_027e0000 + 0x3ff8) |= 4;
    dispstat = *(volatile unsigned short *)(0x04000000 + id);
    func_02075c48((((dispstat >> 8) & 0xff) | ((dispstat << 1) & 0x100)) - 1);
    alarm = data_020d126c.head;
    if (alarm == 0) {
        return;
    }
    do {
        vcount = *(volatile unsigned short *)0x04000006;
        frame = func_02075c48(vcount);
        switch (func_02075bfc(alarm, frame, vcount)) {
        case 0:
            func_02075a54(alarm);
            now = *(volatile unsigned short *)0x04000006;
            if (alarm->fire != now || alarm->frame != frame) {
                return;
            }
            func_020735f0(4);
            *(volatile unsigned short *)0x04000004 &= ~0x20;
            func_02073620(4);
        case 1:
            handler = alarm->handler;
            func_0207597c(alarm);
            alarm->handler = 0;
            if (handler != 0) {
                handler(alarm->arg);
            }
            if (alarm->period != 0 && alarm->canceled == 0) {
                alarm->handler = handler;
                goto insert;
            }
            break;
        case 2:
            func_0207597c(alarm);
        insert:
            alarm->frame = data_020d126c.frame + 1;
            func_020758c8(alarm);
            break;
        }
    } while ((alarm = data_020d126c.head) != 0);
}
