extern char data_020cf7c8[280];
extern char data_0209d8f0;
extern char data_020cfe2c;
extern char data_020cf7ac[220];
extern char data_0209d900;
extern char data_020cf7e4[228];
extern char data_0209d910;
extern char data_020cf800[228];
extern char data_0209d920;
void func_0205888c(void *a, void *b, void *c, int d, int e, int f);
void func_020588a8(void *a, void *b, void *c, int d, int e, int f);
void func_020588c4(void *a, void *b, void *c, int d, int e, int f);
void func_020588e0(void *a, void *b, void *c, int d, int e, int f);

void __sinit_02080700(void)
{
    unsigned char *first;
    unsigned char *second;

    func_0205888c(&data_020cf7c8, &data_0209d8f0, &data_020cfe2c, sizeof(data_020cf7c8), 0, 0);
    first = (unsigned char *)&data_020cf7ac;
    func_020588a8(first, &data_0209d900, &data_020cfe2c, sizeof(data_020cf7ac), 0, 0);
    second = (unsigned char *)&data_020cf7e4;
    func_020588c4(second, &data_0209d910, first, sizeof(data_020cf7e4), 0, 0);
    func_020588e0(&data_020cf800, &data_0209d920, second, sizeof(data_020cf800), 0, 0);
}
