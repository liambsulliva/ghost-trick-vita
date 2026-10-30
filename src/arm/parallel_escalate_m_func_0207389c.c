extern unsigned int func_02075cc4();
extern unsigned int func_02075c98();
extern void func_02075cd8();
extern void func_02075cac();
extern int func_0207316c();

int func_0207389c(unsigned int lockID, unsigned int *lockp, void (*ctrlFuncp)(), unsigned int disableFiq)
{
    unsigned int enabled;
    int lastLockFlag;

    if (disableFiq != 0) {
        enabled = func_02075cc4();
    } else {
        enabled = func_02075c98();
    }
    lastLockFlag = func_0207316c(lockID, lockp);
    if (lastLockFlag == 0) {
        if (ctrlFuncp != 0) {
            ctrlFuncp();
        }
        ((unsigned short *)lockp)[2] = (unsigned short)lockID;
    }
    if (disableFiq != 0) {
        func_02075cd8(enabled);
    } else {
        func_02075cac(enabled);
    }
    return lastLockFlag;
}
