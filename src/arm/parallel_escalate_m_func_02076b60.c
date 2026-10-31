typedef struct Command {
    struct Command *next;
    int id;
    unsigned int arg[4];
} Command;

typedef struct {
    Command *freeListHead;
    unsigned int finishedTag;
    Command *reserveListHead;
    Command *reserveListTail;
    Command *freeListTail;
    int queueRead;
    int queueWrite;
    int waitingCount;
    unsigned int currentTag;
} CommandStatics;

extern Command data_020d1620[256];
extern CommandStatics data_020d1344;
extern Command *data_020d1368[];

extern int func_02075c98(void);
extern int func_02075cac(int enabled);
extern int func_020769a0(unsigned int flags);
extern void func_0207487c(void *start, unsigned int size);
extern int func_02076240(int tag, unsigned int data, int err);
extern void func_02076f74(void);

int func_02076b60(unsigned int flags)
{
    int enabled;

    enabled = func_02075c98();
    if (data_020d1344.reserveListHead == 0) {
        func_02075cac(enabled);
        return 1;
    }
    if (data_020d1344.waitingCount >= 8) {
        if ((flags & 1) == 0) {
            func_02075cac(enabled);
            return 0;
        }
        do {
            func_020769a0(1);
        } while (data_020d1344.waitingCount >= 8);
        if (data_020d1344.reserveListHead == 0) {
            func_02075cac(enabled);
            return 1;
        }
    }
    func_0207487c(data_020d1620, sizeof(data_020d1620));
    if (func_02076240(7, (unsigned int)data_020d1344.reserveListHead, 0) < 0) {
        if ((flags & 1) == 0) {
            func_02075cac(enabled);
            return 0;
        }
        while (data_020d1344.waitingCount >= 8 || func_02076240(7, (unsigned int)data_020d1344.reserveListHead, 0) < 0) {
            func_02075cac(enabled);
            func_020769a0(0);
            enabled = func_02075c98();
            func_0207487c(data_020d1620, sizeof(data_020d1620));
            if (data_020d1344.reserveListHead == 0) {
                func_02075cac(enabled);
                return 1;
            }
        }
    }
    data_020d1368[data_020d1344.queueWrite] = data_020d1344.reserveListHead;
    data_020d1344.queueWrite++;
    if (data_020d1344.queueWrite > 8) {
        data_020d1344.queueWrite = 0;
    }
    data_020d1344.reserveListHead = 0;
    data_020d1344.reserveListTail = 0;
    data_020d1344.waitingCount++;
    data_020d1344.currentTag++;
    func_02075cac(enabled);
    if (flags & 2) {
        func_02076f74();
    }
    return 1;
}
