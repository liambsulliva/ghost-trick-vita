typedef struct Event75fe4 {
    volatile unsigned int flag;
    int queue[2];
} Event75fe4;

extern int func_02075c98(void);
extern void func_02075cac(int enabled);
extern void func_020740d0(void *queue);

unsigned int func_02075fe4(Event75fe4 *event, unsigned int pattern, int mode, unsigned int clear)
{
    unsigned int value = 0;
    int enabled;

    enabled = func_02075c98();
    switch (mode) {
    case 0:
        while ((event->flag & pattern) != pattern) {
            func_020740d0(&event->queue);
        }
        value = event->flag;
        break;
    case 1:
        while ((event->flag & pattern) == 0) {
            func_020740d0(&event->queue);
        }
        value = event->flag;
        break;
    }
    if (value) {
        event->flag &= ~clear;
    }
    func_02075cac(enabled);
    return value;
}
