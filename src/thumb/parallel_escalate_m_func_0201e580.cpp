class Reader {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual void seek(unsigned int offset, int mode);
    virtual void read(void *buffer, unsigned int size);
};

struct Slot {
    char pad00[0xc];
    unsigned int key;
    unsigned int data;
    unsigned int kind;
    unsigned int pad18;
    unsigned int pad1c;
    unsigned int size;
};

struct Archive {
    Reader *reader;
    unsigned int pad04;
    unsigned char ready;
    char pad09[3];
    char slots[0xc0];
};

extern "C" {
extern void func_0201e574(void *object, unsigned int size);

Archive *func_0201e580(Archive *archive, Reader *reader)
{
    unsigned int i;
    unsigned int j;
    unsigned int skip;
    unsigned int header[2];
    unsigned int pair[2];
    unsigned int chunk[2];
    Slot *slot;

    archive->ready = 0;
    archive->reader = reader;
    func_0201e574(archive->slots, 0xc0);
    for (i = 0; i < 8; i++) {
        reader->seek(i << 3, 0);
        reader->read(header, 8);
        if (i == 0) {
            skip = header[0] >> 3;
        }
        slot = (Slot *)((char *)archive + i * 0x18);
        slot->size = header[1];
        reader->seek(header[0], 0);
        reader->read(pair, 8);
        for (j = 0; j < pair[1]; j++) {
            reader->read(chunk, 8);
            switch (chunk[0]) {
            case 0x504b4559:
                slot->kind = 0;
                slot->key = header[0] + chunk[1];
                break;
            case 0x424b4559:
                slot->kind = 1;
                slot->key = header[0] + chunk[1];
                break;
            case 0x42444154:
            case 0x50444154:
                slot->data = header[0] + chunk[1];
                break;
            }
        }
        skip--;
        if (skip == 0) {
            break;
        }
    }
    return archive;
}
}
