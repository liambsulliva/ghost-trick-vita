typedef struct FSFile FSFile;

typedef struct FSArchive {
    int pad0[2];
    FSFile *list;
    int pad1[3];
    unsigned int command;
    int result;
} FSArchive;

struct FSFile {
    FSFile *next;
    void *userdata;
    FSArchive *arc;
    unsigned int stat;
    void *argument;
    int error;
    int queue[2];
};

extern int func_02075c98(void);
extern void func_02075cac(int bak);
extern int func_0206b284(unsigned int command);
extern void func_02074120(void *queue);

void func_0206b2ac(FSFile *file, int ret)
{
    int bak = func_02075c98();
    FSArchive *const arc = file->arc;
    unsigned int command;

    if (arc) {
        FSFile *prev = (FSFile *)&arc->list;
        FSFile *cur = prev->next;
        while (cur) {
            if (cur == file) {
                prev->next = file->next;
                break;
            }
            prev = cur;
            cur = cur->next;
        }
        file->next = 0;
    }
    command = (file->stat >> 8) & 0xff;
    if (!func_0206b284(command) && arc) {
        arc->command = command;
        arc->result = ret;
    }
    file->error = ret;
    file->stat &= ~0xcf;
    func_02074120(file->queue);
    func_02075cac(bak);
}
