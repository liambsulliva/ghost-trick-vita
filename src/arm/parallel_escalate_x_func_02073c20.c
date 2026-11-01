typedef struct Thread73c20 {
    char pad[0x68];
    struct Thread73c20 *next;
    int f6c;
    unsigned int priority;
} Thread73c20;

typedef struct Info73c20 {
    char pad[0x24];
    Thread73c20 *list;
} Info73c20;

extern Info73c20 data_020d0f0c;

void func_02073c20(Thread73c20 *thread)
{
    Thread73c20 *t = data_020d0f0c.list;
    Thread73c20 *pre = 0;
    while (t != 0 && t->priority < thread->priority) {
        pre = t;
        t = t->next;
    }
    if (pre == 0) {
        thread->next = data_020d0f0c.list;
        data_020d0f0c.list = thread;
    } else {
        thread->next = pre->next;
        pre->next = thread;
    }
}
