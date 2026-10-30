typedef struct {
    char pad[0x388];
    unsigned int pxiHandleChecker[2];
} OSSystemWork;

extern void *data_020d12a8[32];
extern unsigned int func_02075c98(void);
extern void func_02075cac(unsigned int enabled);

void func_020761d0(int fifotag, void *callback)
{
    OSSystemWork *p;
    unsigned int enabled;

    p = (OSSystemWork *)0x02fffc00;
    enabled = func_02075c98();
    data_020d12a8[fifotag] = callback;
    if (callback != 0) {
        p->pxiHandleChecker[0] |= (1UL << fifotag);
    } else {
        p->pxiHandleChecker[0] &= ~(1UL << fifotag);
    }
    func_02075cac(enabled);
}
