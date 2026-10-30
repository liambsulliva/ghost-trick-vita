struct Voice {
    unsigned short id;
    unsigned short pad;
    int handle;
};

struct Sound {
    char pad[0x24];
    unsigned char mode;
    char pad25[9];
    unsigned short count;
    char pad30[0x1d4 - 0x30];
    struct Voice voices[3];
};

extern void func_020070b0(unsigned short id, int mode);
extern void func_0202a65c(struct Sound *sound, int value, int flag);

void func_0202aa64(struct Sound *sound, unsigned int id)
{
    struct Voice *voice;
    int i;

    voice = sound->voices;
    if (sound->mode == 1) {
        return;
    }
    for (i = 0; i <= 2; i++) {
        if (i == 2) {
            return;
        }
        if (id == voice->id) {
            break;
        }
        voice++;
    }
    func_020070b0(voice->id, 8);
    func_0202a65c(sound, voice->handle, 1);
    voice->id = 0xffff;
    voice->handle = 0;
    sound->count--;
}
