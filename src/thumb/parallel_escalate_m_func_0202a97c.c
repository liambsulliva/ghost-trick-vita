struct Voice {
    unsigned short id;
    unsigned short pad;
    int handle;
};

extern int func_02006f5c(unsigned short id, int mode);
extern int func_02029d4c(char *object, int value, int mode);
extern void func_02007038(unsigned short id, int handle, int mode);

int func_0202a97c(char *object, unsigned int id)
{
    struct Voice *voice;
    int i;
    int handle;

    voice = (struct Voice *)(object + 0x1d4);
    if (*(unsigned char *)(object + 0x24) == 1) {
        return 0;
    }
    if (*(unsigned short *)(object + 0x2e) >= 2) {
        return 0;
    }
    for (i = 0; i < 2; i++) {
        if (id == ((struct Voice *)(object + 0x1d4))[i].id) {
            return 0;
        }
    }
    for (i = 0; i < 3; i++) {
        if (i >= 2) {
            return 0;
        }
        if (voice->id == 0xffff) {
            voice->id = id;
            break;
        }
        voice++;
    }
    handle = func_02029d4c(object, -func_02006f5c(voice->id, 8), 1);
    voice->handle = handle;
    func_02007038(voice->id, handle, 8);
    *(unsigned short *)(object + 0x2e) = *(unsigned short *)(object + 0x2e) + 1;
    return 1;
}
