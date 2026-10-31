struct Sub {
    unsigned char data[0xc];
};

struct Base {
    void *vtable;
    unsigned char pad004[0xd4];
    int wd8;
    unsigned char pad0dc[2];
    unsigned char bde;
    unsigned char bdf;
    unsigned char pad0e0[8];
    int words[6];
    unsigned char pad100[0xc];
    unsigned char first[6];
    unsigned char second[6];
    unsigned char third[5];
    unsigned char pad11d[3];
    struct Sub subs[4];
    unsigned short h150;
};

extern void func_02011774(struct Base *object);
extern void func_0200f70c(struct Sub *object);
extern char data_020aa4b0;

struct Base *func_02047b54(struct Base *param_1)
{
    struct Sub *sub;
    unsigned int i;

    func_02011774(param_1);
    param_1->vtable = &data_020aa4b0;
    param_1->wd8 = 0;
    param_1->bde = 0;
    param_1->bdf = 0;
    sub = param_1->subs;
    do {
        func_0200f70c(sub);
        sub++;
    } while (sub < param_1->subs + 4);
    param_1->h150 = 0;
    for (i = 0; i < 6; i++) {
        param_1->words[i] = 0;
        param_1->first[i] = 0;
        param_1->second[i] = 0;
    }
    for (i = 0; i < 5; i++) {
        param_1->third[i] = 0;
    }
    return param_1;
}
