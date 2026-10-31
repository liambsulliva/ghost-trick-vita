struct Base {
    void *vtable;
    unsigned char pad004[0xc4];
    int wc8;
    int wcc;
    unsigned char padd0[4];
    int wd4;
    int wd8;
    int wdc;
    unsigned char be0;
    unsigned char pade1[3];
    int we4;
    int we8;
    unsigned char padec[2];
    signed char bee;
    unsigned char bef;
    unsigned short hf0;
    unsigned char bf2;
    unsigned char padf3;
    unsigned char sub[0xc];
    int w100;
    int w104;
};

extern void func_02044524(struct Base *object);
extern void func_0200f70c(void *object);
extern char data_020a844c;

struct Base *func_02044558(struct Base *param_1)
{
    func_02044524(param_1);
    param_1->vtable = &data_020a844c;
    param_1->wc8 = 0;
    param_1->wcc = 0;
    param_1->wd4 = 0;
    param_1->wd8 = -1;
    param_1->wdc = 0;
    param_1->be0 = 0;
    param_1->we4 = 0;
    param_1->we8 = 0;
    param_1->bee = -1;
    param_1->bef = 0;
    param_1->hf0 = 0;
    param_1->bf2 = 1;
    func_0200f70c(param_1->sub);
    param_1->w100 = 0;
    param_1->w104 = 0;
    return param_1;
}
