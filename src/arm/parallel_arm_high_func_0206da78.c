struct File {
    char pad[0x30];
    unsigned int f30;
    unsigned int f34;
    unsigned short h38;
    unsigned short h3a;
};

int func_0206d714(struct File *file, unsigned int command, int block);

int func_0206da78(int unused0, struct File *file, unsigned int unused1, unsigned int value, unsigned int *out)
{
    int ret;

    file->h38 = 0;
    file->h3a = 0;
    file->f30 = value;
    file->f34 = *out;
    ret = func_0206d714(file, 5, 1);
    if (ret == 0) {
        *out = file->f34;
    }
    return ret;
}
