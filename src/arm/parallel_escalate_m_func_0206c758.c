typedef struct ReadArg {
    void *buffer;
    int length;
} ReadArg;

typedef struct File {
    char pad0[0x10];
    void *argument;
    char pad14[0x1c];
    ReadArg arg;
} File;

extern unsigned int func_0206e074();
extern unsigned int func_0206e034();
extern void func_0206b9dc();

int func_0206c758(File *file, void *buffer, int length)
{
    unsigned int end;
    int pos;

    if (func_0206e074(file, &pos) && func_0206e034(file, &end)) {
        if ((unsigned int)(pos + length) > end) {
            length = end - pos;
        }
    }
    file->argument = &file->arg;
    file->arg.buffer = buffer;
    ((ReadArg *)file->argument)->length = length;
    func_0206b9dc(file, 0, 0, file->argument);
    return length;
}
