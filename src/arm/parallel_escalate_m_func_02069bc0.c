extern int func_02069c84(int id, int flag);
extern void func_02075fd0(void *event);
extern void func_020759b4(void *alarm);
extern void func_020699f4(void *work);
extern int func_02075fe4(void *event, int pattern, int mode, int clear);
extern unsigned int func_02069a4c(unsigned short *id);

struct Event {
    void *head;
    void *tail;
    unsigned int flag;
};

struct Alarm {
    unsigned int w[10];
};

struct WaitWork {
    struct Event event;
    struct Alarm alarm;
    int (*func)(void *);
    void *arg;
    unsigned int reserved;
};

void func_02069bc0(unsigned short id)
{
    struct WaitWork work;

    func_02069c84(id, 1);
    func_02075fd0(&work.event);
    func_020759b4(&work.alarm);
    work.func = (int (*)(void *))func_02069a4c;
    work.arg = &id;
    func_020699f4(&work);
    func_02075fe4(&work.event, 1, 0, 1);
}
