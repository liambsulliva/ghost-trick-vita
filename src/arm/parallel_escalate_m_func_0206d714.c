typedef int (*Proc)(void *, unsigned int);
typedef int (*Command)(void *);

struct Archive {
    char pad[0x2c];
    Proc proc;
    unsigned int flag;
};

struct Holder {
    char pad[0x20];
    struct Archive *arc;
};

struct File {
    int a;
    int b;
    struct Holder *holder;
};

extern Command data_0209e384[];
extern int func_0206b350(struct File *file, int result);

int func_0206d714(struct File *file, unsigned int command, int block)
{
    int ret;
    struct Archive *arc;

    ret = 0x101;
    arc = file->holder->arc;
    if (arc->flag & (1 << command)) {
        ret = arc->proc(file, command);
        switch (ret) {
        case 0:
        case 1:
        case 4:
            break;
        case 0x100:
            break;
        case 0x102:
            arc->flag &= ~(1 << command);
            ret = 0x101;
            break;
        }
    }
    if (ret == 0x101) {
        if (command < 13) {
            ret = data_0209e384[command](file);
        }
        else {
            ret = 4;
        }
    }
    if (block) {
        ret = func_0206b350(file, ret);
    }
    return ret;
}
