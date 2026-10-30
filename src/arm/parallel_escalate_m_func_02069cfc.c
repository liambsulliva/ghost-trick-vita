struct ThreadQueue {
    void *head;
    void *tail;
};

struct CardCommon {
    unsigned char pad[0x14];
    int lock_owner;
    int lock_ref;
    struct ThreadQueue lock_queue;
    int lock_target;
};

extern struct CardCommon data_020d02a0;
extern int func_02075c98(void);
extern int func_02075cac(int mode);
extern void func_02075f84(void);
extern void func_02074120(struct ThreadQueue *queue);

void func_02069cfc(int owner, int target)
{
    struct CardCommon *const cmn = &data_020d02a0;
    int bak_psr = func_02075c98();

    if (cmn->lock_owner != owner || !cmn->lock_ref) {
        func_02075f84();
    } else {
        if (cmn->lock_target != target) {
            func_02075f84();
        }
        if (!--cmn->lock_ref) {
            cmn->lock_owner = -3;
            cmn->lock_target = 0;
            func_02074120(&cmn->lock_queue);
        }
    }
    func_02075cac(bak_psr);
}
