typedef struct FSFile FSFile;

typedef struct FSArchive {
    int pad0[2];
    FSFile *list;
    int pad1[2];
    unsigned int flag;
} FSArchive;

struct FSFile {
    FSFile *next;
    void *userdata;
    FSArchive *arc;
    unsigned int stat;
    void *argument;
    int error;
};

extern void func_02075f84(void);
extern int func_02075c98(void);
extern void func_02075cac(int bak);
extern void func_0206b2ac(FSFile *file, int ret);
extern int func_0206b728(FSArchive *arc, int start);
extern void func_0206b958(FSFile *file);
extern void func_0206b8d4(void);

int func_0206b9dc(FSFile *file, int command, int blocking)
{
    FSArchive *const arc = file->arc;
    int bak;
    int start;
    int ret;
    int started;

    ret = 0;
    if (((file->stat & 1) ? 1 : 0) != 0) {
        func_02075f84();
    }
    if (arc == 0) {
        file->error = 6;
        return 0;
    }
    file->error = 2;
    file->stat = (file->stat & ~0xff00) | (command << 8) | 1;
    file->next = 0;
    if (blocking) {
        file->stat |= 4;
    }
    bak = func_02075c98();
    if (arc->flag & 0x80) {
        func_0206b2ac(file, 3);
    } else {
        FSFile *p = (FSFile *)&arc->list;
        while (p->next) {
            p = p->next;
        }
        p->next = file;
    }
    start = 0;
    if (arc->list == file && !(arc->flag & 0x10)) {
        start = 1;
    }
    func_02075cac(bak);
    if (file->error != 3) {
        started = func_0206b728(arc, start);
        if (blocking) {
            func_0206b958(file);
            ret = (file->error == 0);
        } else {
            if (started) {
                func_0206b8d4();
            }
            ret = 1;
        }
    }
    return ret;
}
