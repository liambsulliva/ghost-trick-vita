typedef struct {
    unsigned char reserved0[0xac];
    unsigned int gameCode;
    unsigned short makerCode;
    unsigned char fixed;
    unsigned char unitCode;
    unsigned char deviceType;
    unsigned char exLsiID[3];
    unsigned char reserved1[6];
    unsigned short moduleID;
} CartHeader;

typedef struct {
    unsigned short moduleID;
    unsigned char exLsiID[3];
    unsigned char isAgbCartridge:1;
    unsigned char detectPullOut:1;
    unsigned short makerCode;
    unsigned int gameCode;
} CartModuleInfo;

typedef struct {
    unsigned char reserved[6];
    unsigned short lockID;
} CartWork;

extern CartWork data_020d2efc;
extern int func_02079b48(void);
extern void func_02079d24(unsigned int lockID,unsigned int *info);
extern void func_02079d80(unsigned int lockID,unsigned int *info);
extern void func_02079ca8(unsigned int *cycle);
extern void func_02079cf0(unsigned int *cycle);

int func_02079ba4(void)
{
    int retval;
    unsigned int lockInfo[2];
    unsigned int cycle[2];
    CartHeader *chp;
    CartModuleInfo *cip;

    retval = 1;
    chp = (CartHeader *)0x08000000;
    cip = (CartModuleInfo *)0x02fffc30;
    if (func_02079b48() == 0) {
        return 0;
    }
    if (cip->detectPullOut == 1) {
        return 0;
    }
    func_02079d24(data_020d2efc.lockID,lockInfo);
    func_02079ca8(cycle);
    if ((chp->fixed == 0x96 && cip->moduleID != chp->moduleID)
        || (chp->fixed != 0x96 && cip->moduleID != *(unsigned short *)0x0801fffe)
        || (cip->gameCode != chp->gameCode && cip->isAgbCartridge)) {
        cip->detectPullOut = 1;
        retval = 0;
    }
    func_02079cf0(cycle);
    func_02079d80(data_020d2efc.lockID,lockInfo);
    return retval;
}
