typedef struct Entry0201d7bc {
    unsigned short *text;
    int time;
    unsigned short *pos;
    char pad[8];
} Entry0201d7bc;

typedef struct Log0201d7bc {
    unsigned int count;
    int index;
    unsigned int length;
    unsigned short *text;
    int time;
    int pad14;
    unsigned char kind;
    unsigned char busy;
    unsigned short buf[0xea7];
    Entry0201d7bc entries[30];
} Log0201d7bc;

void func_0201da80(Log0201d7bc *log, int *index)
{
    unsigned int value;
    unsigned int next;

    value = *index;
    if (value >= 0x1d) {
        next = 0;
    } else {
        next = value + 1;
    }
    *index = next;
}
