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

int func_0201d938(Log0201d7bc *log, unsigned short c)
{
    unsigned int length;

    length = log->length;
    if (length < 0xea6) {
        log->length = length + 1;
        log->buf[length] = c;
        return 1;
    } else {
        log->length = 0;
        return 0;
    }
}
