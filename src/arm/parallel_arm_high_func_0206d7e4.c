struct Span {
    char pad[8];
    unsigned int end;
    unsigned int pos;
};

struct File {
    int a;
    struct Span *span;
    void *holder;
    char pad[0x24];
    unsigned int f30;
    unsigned int f34;
    unsigned int f38;
};

int func_0206d714(struct File *file, unsigned int command, int block);

void func_0206d7e4(int unused, struct File *file, unsigned int value, unsigned int *out)
{
    unsigned int room;
    unsigned int want;

    want = *out;
    room = file->span->end - file->span->pos;
    if (want > room) {
        *out = room;
    }
    file->f34 = want;
    file->f30 = value;
    file->f38 = *out;
    func_0206d714(file, 0, 0);
}
