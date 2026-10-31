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

typedef struct SharedWork SharedWork;

extern Command data_020d1620[256];
extern CommandStatics data_020d1344;
extern SharedWork data_020d13a0;
extern SharedWork *data_020d2e80;
extern void func_02076f10(void);
extern void func_0207727c(SharedWork *work);
extern Command *func_02076aa0(unsigned int flags);
extern void func_02076b28(Command *command);
extern int func_02076b60(unsigned int flags);

void func_020768d0(void)
{
    Command *command;
    int i;

    func_02076f10();

    data_020d1344.freeListHead = &data_020d1620[0];
    for (i = 0; i < 256 - 1; i++) {
        data_020d1620[i].next = &data_020d1620[i + 1];
    }
    data_020d1620[256 - 1].next = 0;
    data_020d1344.freeListTail = &data_020d1620[256 - 1];

    data_020d1344.reserveListHead = 0;
    data_020d1344.reserveListTail = 0;

    data_020d1344.waitingCount = 0;
    data_020d1344.queueRead = 0;
    data_020d1344.queueWrite = 0;
    data_020d1344.currentTag = 1;
    data_020d1344.finishedTag = 0;

    data_020d2e80 = &data_020d13a0;
    func_0207727c(data_020d2e80);

    command = func_02076aa0(1);
    if (command == 0) {
        return;
    }

    command->id = 29;
    command->arg[0] = (unsigned int)data_020d2e80;

    func_02076b28(command);
    func_02076b60(1);
}
