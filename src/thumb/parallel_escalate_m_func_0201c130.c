typedef struct {
    unsigned short key;
    unsigned short pad;
    int rest;
} Rec0201c130;

extern Rec0201c130 data_02084ae8[];

int func_0201c130(unsigned int value)
{
    unsigned int index;
    unsigned short entry;

    index = 0;
    goto test;
body:
    if (value == entry) {
        return 1;
    }
    index = index + 1;
test:
    entry = data_02084ae8[index].key;
    if (entry != 0) {
        goto body;
    }
    return 0;
}
