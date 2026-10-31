typedef struct SNDCommand SNDCommand;
struct SNDCommand {
    SNDCommand *next;
};

typedef struct {
    SNDCommand *freeList;
    unsigned int finishedTag;
    SNDCommand *reserveList;
    SNDCommand *reserveListEnd;
    SNDCommand *freeListEnd;
    int queueRead;
    int queueWrite;
    int listCount;
} SNDCommandWork;

extern SNDCommandWork data_020d1344;
extern SNDCommand *data_020d1368[];
extern int func_02075c98(void);
extern int func_02075cac(int enabled);
extern void func_02075d14(int count);
extern unsigned int func_02077258(void);

const SNDCommand *func_020769a0(unsigned int flags)
{
    int bak_psr = func_02075c98();
    SNDCommand *commandList;
    SNDCommand *commandListTail;

    if (flags & 1) {
        unsigned int tag = func_02077258();
        while (data_020d1344.finishedTag == tag) {
            func_02075cac(bak_psr);
            func_02075d14(50);
            bak_psr = func_02075c98();
            tag = func_02077258();
        }
    } else {
        if (data_020d1344.finishedTag == func_02077258()) {
            func_02075cac(bak_psr);
            return 0;
        }
    }

    commandList = data_020d1368[data_020d1344.queueRead];
    data_020d1344.queueRead++;
    if (data_020d1344.queueRead > 8) {
        data_020d1344.queueRead = 0;
    }

    commandListTail = commandList;
    while (commandListTail->next != 0) {
        commandListTail = commandListTail->next;
    }

    if (data_020d1344.freeListEnd != 0) {
        data_020d1344.freeListEnd->next = commandList;
    } else {
        data_020d1344.freeList = commandList;
    }
    data_020d1344.freeListEnd = commandListTail;

    data_020d1344.listCount--;
    data_020d1344.finishedTag++;

    func_02075cac(bak_psr);
    return commandList;
}
