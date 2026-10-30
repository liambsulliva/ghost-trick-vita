struct Record {
    int first;
    int second;
    int third;
    short key;
    short extra;
};
struct Owner {
    unsigned char before[0x48];
    struct Record records[5];
};

struct Record *func_02043400(struct Owner *param_1, int param_2)
{
    unsigned int i;
    i = 0;
    while (i < 5) {
        if (param_2 == param_1->records[i].key) {
            return &param_1->records[i];
        }
        i = i + 1;
    }
    return 0;
}
