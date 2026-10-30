class Channel02029d4c {
public:
    virtual void slot0();
    virtual void slot1();
    virtual int slot2(int value, void *table);
};

struct Sound02029d4c {
    char pad[8];
    Channel02029d4c *channels[2];
    char pad10[0x14];
    unsigned char mode;
};

extern "C" {
extern char data_02085220[];

int func_02029d4c(Sound02029d4c *sound, int value, int channel)
{
    int size;
    unsigned int padded;

    size = value;
    if (value < 0) {
        size = -value;
    }
    padded = size + 0x14;
    size = ((padded + 0x3ff) & 0xfffffc00) - 0x14;
    if (sound->mode != 0) {
        channel = 1;
    }
    if (value > 0) {
        return sound->channels[channel]->slot2(size, data_02085220);
    }
    return sound->channels[channel]->slot2(-size, data_02085220);
}
}
