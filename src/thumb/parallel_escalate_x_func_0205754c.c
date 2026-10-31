extern int func_0200b208();
extern int func_02023f24();

struct FlagWord0205754c {
    unsigned int bit0 : 1;
    unsigned int bit1 : 1;
    unsigned int bit2 : 1;
    unsigned int bit3 : 1;
    unsigned int bit4 : 1;
    unsigned int rest : 27;
};

typedef struct {
    char pad00[0x4c];
    int f4c;
    char pad50[0xc];
    unsigned short f5c;
    unsigned char f5e;
    unsigned char f5f;
    char pad60[8];
    int f68;
    char pad6c[8];
    int f74;
    struct FlagWord0205754c f78;
} Obj0205754c;

void func_0205754c(Obj0205754c *param_1)
{
    func_02023f24(func_0200b208(), param_1->f4c);
    param_1->f4c = 0;
    param_1->f78.bit2 = 0;
    param_1->f78.bit1 = 0;
    param_1->f5f = 1;
    param_1->f78.bit3 = 0;
    param_1->f78.bit4 = 0;
    param_1->f68 = -1;
    param_1->f5c = 0;
    param_1->f74 = 0;
    param_1->f5e = 0xff;
}
