typedef struct {
    unsigned short key;
    unsigned short pad;
    int rest;
} Key0201c0fc;

typedef struct {
    unsigned short value;
    unsigned short pad;
    int rest;
} Value0201c0fc;

extern Key0201c0fc data_02084ae8[];
extern Value0201c0fc data_02084aec[];
extern int func_0201c130(unsigned int value);

unsigned short func_0201c0fc(unsigned int value)
{
    unsigned int index;
    unsigned int offset;
    unsigned short entry;

    if (func_0201c130(value) != 0) {
        index = 0;
        goto test;
body:
        if (value == entry) {
            return *(unsigned short *)((char *)data_02084aec + offset);
        }
        index = index + 1;
test:
        offset = index * 8;
        entry = data_02084ae8[index].key;
        if (entry != 0) {
            goto body;
        }
    }
    return 0;
}
