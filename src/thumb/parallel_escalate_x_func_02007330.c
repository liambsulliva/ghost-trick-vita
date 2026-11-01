typedef struct Command07330 {
    int f0;
    int type;
    int f8;
    int fc;
    int f10;
    int f14;
    int f18;
    int f1c;
} Command07330;

extern Command07330 data_020b18d8;
extern char data_020b18dc[];
extern void func_0200743c(void *command);

void func_02007330(int a, int b, int c, int d, int e, unsigned char f, unsigned char g, short h, unsigned short i, signed char j, unsigned short k, unsigned char l)
{
    data_020b18d8.type = 10;
    data_020b18d8.f8 = (a << 16) | (b & 0xffff);
    data_020b18d8.fc = (c << 16) | (d & 0xffff);
    data_020b18d8.f10 = e;
    data_020b18d8.f14 = ((f & 0xf) << 8) | (g & 0xf);
    data_020b18d8.f18 = (l << 24) | ((j & 0xff) << 16) | k;
    data_020b18d8.f1c = (h << 16) | i;
    func_0200743c(data_020b18dc);
}
