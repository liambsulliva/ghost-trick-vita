typedef struct {
    void *handler;
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

int func_02075c98(void);
void func_02075cac(int enabled);
void func_02075f84(void);
int func_02075c48(int vcount);
void func_020758c8(VAlarm *alarm);

void func_020759c8(VAlarm *alarm, int count, short delay, void *handler, void *arg)
{
    int enabled;
    int vcount;
    int frame;

    enabled = func_02075c98();
    if (!alarm || alarm->handler) {
        func_02075f84();
    }
    vcount = *(volatile unsigned short *)0x04000006;
    frame = func_02075c48(vcount);
    alarm->period = 0;
    if (count <= vcount) {
        frame++;
    }
    alarm->frame = frame;
    alarm->fire = count;
    alarm->delay = delay;
    alarm->handler = handler;
    alarm->arg = arg;
    alarm->canceled = 0;
    func_020758c8(alarm);
    func_02075cac(enabled);
}
