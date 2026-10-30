struct Span;

struct File {
    int a;
    struct Span *span;
    void *holder;
    unsigned int flags;
    char pad[0x10];
    char inner[0x10];
    unsigned int f30;
    unsigned int f34;
    unsigned int f38;
};

int func_0206d714(struct File *file, unsigned int command, int block);

int func_0206dabc(unsigned int param_1, struct File *file, unsigned int param_3, unsigned int param_4)
{
    int ret;

    file->f34 = param_3;
    file->f30 = param_1;
    ret = func_0206d714(file, 6, 1);
    if (ret == 0) {
        file->flags = (file->flags | 0x10) & ~0x20;
        file->span = (struct Span *)((char *)file + 0x20);
        file->holder = (void *)param_1;
    }
    return ret;
}
