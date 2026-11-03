typedef struct Lock736ec {
    volatile unsigned int flag;
    volatile unsigned short owner;
    volatile unsigned short extension;
} Lock736ec;

extern int func_02073808(int id, Lock736ec *lock, void (*ctrl)(void));
extern void WaitByLoop(int count);
extern void func_02072ec4(int value, void *dst, int size);
extern int func_0207388c(int id, Lock736ec *lock, void (*ctrl)(void));

void func_020736ec(void)
{
    static int data_020d0f08 = 0;
    Lock736ec *lock;

    if (data_020d0f08) {
        return;
    }
    lock = (Lock736ec *)0x02fffff0;
    lock->flag = 0;
    data_020d0f08 = 1;
    func_02073808(0x7e, lock, 0);
    while (lock->extension != 0) {
        WaitByLoop(0x400);
    }
    ((unsigned int *)0x02ffffb0)[0] = 0xffffffff;
    ((unsigned int *)0x02ffffb0)[1] = 0xffff0000;
    func_02072ec4(0, (void *)0x02ffffc0, 0x28);
    *(volatile unsigned short *)0x04000204 |= 0x800;
    *(volatile unsigned short *)0x04000204 |= 0x80;
    func_0207388c(0x7e, lock, 0);
    func_02073808(0x7f, lock, 0);
}
