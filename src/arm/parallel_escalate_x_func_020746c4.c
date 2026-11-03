typedef struct Mutex746c4 {
    int queue[2];
    void *thread;
    unsigned int count;
} Mutex746c4;

typedef struct Threads746c4 {
    int pad0;
    void *current;
} Threads746c4;

extern Threads746c4 data_020d0f28;
extern int func_02075c98(void);
extern void func_02075cac(int enabled);
extern void func_020747c0(void *thread, Mutex746c4 *m);
extern void func_02074120(Mutex746c4 *m);

void func_020746c4(Mutex746c4 *m, unsigned int type)
{
    int enabled;
    int unlock;
    void *cur;

    enabled = func_02075c98();
    cur = data_020d0f28.current;
    unlock = 0;
    if (type != 0 && type != (m->count & 0xff000000)) {
        func_02075cac(enabled);
        return;
    }
    switch (m->count & 0xff000000) {
    case 0x10000000:
    case 0x30000000:
        if (m->thread == cur) {
            m->count = (m->count & 0xff000000) | ((*(unsigned int *)((char *)m + 0xc) - 1) & 0x00ffffff);
            if ((m->count & 0x00ffffff) == 0) {
                unlock = 1;
            }
        }
        break;
    case 0x20000000:
        m->count = (m->count & 0xff000000) | ((m->count - 1) & 0x00ffffff);
        if ((m->count & 0x00ffffff) == 0) {
            unlock = 1;
        }
        break;
    default:
        func_02075cac(enabled);
        return;
    }
    if (unlock) {
        func_020747c0(cur, m);
        m->thread = 0;
        m->count = m->count & 0x00ffffff;
        func_02074120(m);
    }
    func_02075cac(enabled);
}
