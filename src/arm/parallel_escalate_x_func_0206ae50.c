struct DmaOps {
    void *start;
    void (*stop)(unsigned int dma);
};

struct CardCommon {
    char pad0[0xc];
    unsigned int range;
    unsigned int range2;
    char pad14[0x4e8];
    unsigned int src;
    void *dst;
    unsigned int len;
    unsigned int dma;
    struct DmaOps *dma_ops;
};

struct RomStat {
    unsigned int base;
    int flush;
    int pad8;
    void *read;
    int pad10;
    void *done;
    int w18;
    unsigned int src;
    void *dst;
    unsigned int len;
    int w28;
};

struct CacheFlag {
    int pad0;
    int enabled;
};

extern struct CardCommon data_020d02a0;
extern struct RomStat data_020d08e0;
extern struct CacheFlag data_020b1724;
extern char data_020d08f0;
extern void func_02069b6c(void);
extern unsigned int func_02069d7c(void);
extern void func_02075f84(void);
extern void func_02069f74(struct CardCommon *p, int wait, void *callback, void *arg);
extern struct DmaOps *func_0206a038(unsigned int dma);
extern void func_0206ad20(void);
extern void func_0206aae8(void);
extern void func_0206ae04(void);
extern int func_0206ad50(unsigned int dma, void *dst, unsigned int src, unsigned int len);
extern int func_02075c98(void);
extern void func_02075cac(int enabled);
extern void func_0206a074(void *dst, unsigned int len, unsigned int range);
extern void func_0206a090(void *dst, unsigned int len, unsigned int range);
extern void func_0206aca4(void *task);
extern void func_0206b050(void);
extern void func_02069eb4(void *task, int is_async);

void func_0206ae50(unsigned int dma, unsigned int src, void *dst, unsigned int len, void *callback, void *arg, int is_async)
{
    struct CardCommon *p;
    unsigned int addr;
    int enabled;

    p = &data_020d02a0;
    func_02069b6c();
    if ((func_02069d7c() & 4) == 0) {
        func_02075f84();
    }
    func_02069f74(p, 1, callback, arg);
    p->dma_ops = func_0206a038(dma);
    p->dma = (p->dma_ops != 0) ? (dma & 3) : -1;
    if (p->dma <= 3) {
        p->dma_ops->stop(p->dma);
    }
    addr = src + data_020d08e0.base;
    p->src = addr;
    p->dst = dst;
    p->len = len;
    data_020d08e0.done = (void *)func_0206ad20;
    data_020d08e0.w18 = 0;
    data_020d08e0.src = addr;
    data_020d08e0.dst = dst;
    data_020d08e0.len = len;
    data_020d08e0.w28 = 0;
    if (data_020d08e0.read == (void *)func_0206aae8 && func_0206ad50(p->dma, dst, addr, len) != 0) {
        enabled = func_02075c98();
        if (data_020d08e0.flush != 0) {
            func_0206a074(p->dst, p->len, p->range);
        }
        if (data_020b1724.enabled != 0) {
            func_0206a090(p->dst, p->len, p->range2);
        }
        func_02075cac(enabled);
        func_0206aca4(&data_020d08f0);
        if (is_async == 0) {
            func_0206b050();
        }
        return;
    }
    if (data_020d08e0.flush != 0) {
        func_0206a074(p->dst, p->len, p->range);
    }
    func_02069eb4((void *)func_0206ae04, is_async);
}
