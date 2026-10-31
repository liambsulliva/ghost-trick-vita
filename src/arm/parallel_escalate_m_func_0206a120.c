typedef struct {
    int result;
} CommandArg;

typedef struct {
    int flags;
    void *current;
} ThreadInfo;

typedef struct {
    CommandArg *cmd;
    volatile unsigned int flag;
    char pad[0x514];
    void *thread;
} CardCommon;

extern ThreadInfo data_020d0f28;

extern int func_0207621c(int tag, int proc);
extern void func_02075d14(unsigned int cycles);
extern void func_0207487c(void *start, unsigned int size);
extern void func_020748a0(void);
extern int func_02076240(int tag, unsigned int data, int err);
extern int func_02075c98(void);
extern int func_02075cac(int enabled);
extern void func_020740d0(void *queue);
extern void func_02074844(void *start, unsigned int size);

static inline void send_pxi(unsigned int data)
{
    while (func_02076240(11, data, 1) < 0) {
    }
}

int func_0206a120(CardCommon *p, int req_type, int retry_count)
{
    if ((p->flag & 2) == 0) {
        p->flag |= 2;
        while (!func_0207621c(11, 1)) {
            func_02075d14(50);
        }
        func_0206a120(p, 0, 1);
    }
    func_0207487c(p->cmd, 0x60);
    func_020748a0();
    p->thread = data_020d0f28.current;
    do {
        p->flag |= 0x20;
        send_pxi((unsigned int)req_type);
        if (req_type == 0) {
            send_pxi((unsigned int)p->cmd);
        }
        {
            int enabled = func_02075c98();
            while ((p->flag & 0x20) != 0) {
                func_020740d0(0);
            }
            func_02075cac(enabled);
        }
        func_02074844(p->cmd, 0x60);
    } while (p->cmd->result == 4 && --retry_count > 0);
    return p->cmd->result == 0;
}
