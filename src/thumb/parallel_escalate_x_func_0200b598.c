struct AnimEntry {
    unsigned short sprite;
    unsigned short palette;
    unsigned short sound;
    unsigned short bank;
};

struct AnimInfo {
    unsigned short seq : 2;
    unsigned short entry : 14;
};

struct AnimSlot {
    int f00;
    int bank_id;
    unsigned short id;
    unsigned char slot;
    unsigned char pad0b[9];
    unsigned char sub14[4];
    void *objs[2];
    int sound_id;
    unsigned char pad24[4];
    unsigned short frames;
    unsigned char pad2a[0x2a];
    unsigned char ctrl[4];
};

extern unsigned char data_020cff60;
extern unsigned short data_02086f84[][5];
extern struct AnimInfo data_02088910[];
extern struct AnimEntry data_0208c064[];

extern void func_0200b4fc(void *ctrl);
extern void func_0200b760(void *sub, int slot);
extern int func_0200b764(void *obj);
extern void func_0200b074(void *obj);
extern int func_0200b774(void *obj);
extern void func_0200b020(void *obj, unsigned int sprite, unsigned int slot);
extern void func_0200af44(void *obj, unsigned int sprite, unsigned int slot);
extern unsigned char func_0200b794(void *obj);
extern void func_0200b784(void *sub, unsigned char value);
extern void func_0200b060(void *obj);
extern void func_0200af34(void *obj, unsigned int palette);
extern void *func_0200b208(void);
extern void func_02023f24(void *cache, int id);
extern int func_020420e0(unsigned int id);
extern void *func_02023df4(void *cache, int id);
extern int func_02023e80(void *cache, int id);
extern void func_0206930c(void *file, void **bank);
extern void func_02023e44(void *cache, int id);
extern unsigned char *func_02069470(void *file, unsigned int kind);
extern void func_02075f84(void);
extern void *func_02069458(void *bank, unsigned int index);
extern void func_0200b7a8(void *ctrl);
extern void func_0206980c(void *ctrl, void *seq);
extern unsigned short func_0200b7b0(void *ctrl);
extern void func_0200b7b8(void *ctrl);

void func_0200b598(struct AnimSlot *self, unsigned int id, unsigned int flags)
{
    struct AnimEntry *entry;
    void *obj;
    void *file;
    void *bank;
    unsigned char *block;
    void *seq;

    func_0200b4fc(self->ctrl);
    self->id = (flags & 2) == 0 ? 0 : id;
    if (id == 0) {
        return;
    }
    if (id & 0x8000) {
        id = data_02086f84[id & ~0x8000][data_020cff60 - 1];
    }
    obj = self->objs[self->slot];
    if (obj == 0) {
        return;
    }
    entry = &data_0208c064[data_02088910[id].entry];
    func_0200b760(self->sub14, self->slot);
    if (id != 0) {
        if ((flags & 1) && entry->sprite != 0 && func_0200b764(obj)) {
            func_0200b074(obj);
            if (func_0200b774(obj)) {
                func_0200b020(obj, entry->sprite, self->slot);
            } else {
                func_0200af44(obj, entry->sprite, self->slot);
            }
            func_0200b784(self->sub14, func_0200b794(obj));
        }
        if ((flags & 2) && entry->palette != 0 && func_0200b764(obj)) {
            func_0200b060(obj);
            func_0200af34(obj, entry->palette);
        }
        if ((flags & 4) && entry->sound != 0) {
            func_02023f24(func_0200b208(), self->sound_id);
            self->sound_id = func_020420e0(entry->sound);
        }
        if ((flags & 8) && entry->bank != 0) {
            func_02023f24(func_0200b208(), self->bank_id);
            self->bank_id = func_020420e0(entry->bank);
            file = func_02023df4(func_0200b208(), self->bank_id);
            if (file != 0) {
                bank = 0;
                if (func_02023e80(func_0200b208(), self->bank_id) == 0) {
                    func_0206930c(file, &bank);
                    func_02023e44(func_0200b208(), self->bank_id);
                } else {
                    block = func_02069470(file, 0x41424e4b);
                    if (block != 0) {
                        bank = block + 8;
                    } else {
                        func_02075f84();
                    }
                }
                seq = func_02069458(bank, data_02088910[id].seq);
                if (seq != 0) {
                    func_0200b7a8(self->ctrl);
                    func_0206980c(self->ctrl, seq);
                    self->frames = func_0200b7b0(self->ctrl);
                    func_0200b7b8(self->ctrl);
                }
            }
        }
    }
}
