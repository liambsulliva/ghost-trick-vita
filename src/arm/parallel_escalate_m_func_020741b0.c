typedef struct Thread {
    unsigned char context[0x64];
    unsigned int state;
    struct Thread *next;
} Thread;

typedef struct {
    char pad[0x24];
    Thread *list;
} ThreadStatics;

extern ThreadStatics data_020d0f0c;

Thread *func_020741b0(void)
{
    Thread *thread;

    for (thread = data_020d0f0c.list; thread != 0 && thread->state != 1; thread = thread->next) {
    }
    return thread;
}
