struct Node {
    void *vtable;
    unsigned char pad004[0x78];
    void *link;
    short h80;
    short h82;
    int w84;
    unsigned short h88;
    unsigned char b8a;
    unsigned char b8b;
    unsigned char b8c;
    unsigned char pad8d;
    unsigned short h8e;
    unsigned short h90;
    unsigned char pad92[2];
    int w94;
    int w98;
    int w9c;
    int wa0;
    unsigned short ha4;
    unsigned short ha6;
    int wa8;
    int wac;
    int wb0;
};

extern void func_020569f0(struct Node *object);
extern char data_020ac28c;
extern char data_0209c318;

struct Node *func_02052618(struct Node *param_1)
{
    func_020569f0(param_1);
    param_1->vtable = &data_020ac28c;
    param_1->h80 = -1;
    param_1->h82 = -1;
    param_1->w84 = -1;
    param_1->h88 = 0;
    param_1->b8a = 0;
    param_1->b8b = 0;
    param_1->b8c = 0;
    param_1->h8e = 0;
    param_1->h90 = 0;
    param_1->w94 = 0x1000;
    param_1->w98 = 0x1000;
    param_1->w9c = 0;
    param_1->wa0 = 0;
    param_1->ha4 = 0;
    param_1->ha6 = 0;
    param_1->wa8 = 0;
    param_1->wac = 0;
    param_1->wb0 = 0;
    param_1->link = &data_0209c318;
    return param_1;
}
