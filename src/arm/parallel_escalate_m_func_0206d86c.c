struct DirPos {
    void *arc;
    unsigned short own_id;
    unsigned short index;
    unsigned int pos;
};

struct File {
    void *prev;
    void *next;
    void *arc;
    unsigned int stat;
    int command;
    int error;
    int queue[2];
    int prop[4];
    struct DirPos pos;
};

extern int func_0206d714(struct File *file, unsigned int command, int block);

int func_0206d86c(void *arc, struct File *file, unsigned int id, unsigned int pos)
{
    int ret;

    file->pos.own_id = (unsigned short)id;
    file->arc = arc;
    file->pos.arc = arc;
    file->pos.index = (unsigned short)(id >> 16);
    file->pos.pos = pos;
    ret = func_0206d714(file, 2, 1);
    if (ret != 0) {
        return ret;
    }
    file->stat = (file->stat | 0x20) & ~0x10;
    file->next = file->prop;
    file->arc = arc;
    return ret;
}
