extern char data_020cf844[0x18];
extern char data_0209d9d4;
extern char data_020bbc18;
extern char data_020cf898[0x114];
extern char data_0209d9e4;
extern char data_020cb04c;
extern char data_020cf8b4[0x154];
extern char data_0209d9f0;
extern char data_020cf860[0x154];
extern char data_0209d9fc;
extern char data_020cf87c[0x154];
extern char data_0209da04;
extern char data_020cf820;
extern char data_020cf824;
extern char data_020cf81c;
extern char data_020cf828;
void func_02059e30(void *a, void *b, void *c, int d, int e, int f);
void func_02059e4c(void *a, void *b, void *c, int d, int e, int f);
void func_02059e68(void *a, void *b, void *c, int d, int e, int f);
void func_02059e84(void *a, void *b, void *c, int d, int e, int f);
void func_02059ea0(void *a, void *b, void *c, int d, int e, int f);
void func_0200eb28(void *a);
void func_0200eb3c(void *a);
void func_0202f4b0(void *a);
void func_02059ebc(void *a);

void __sinit_02080774(void)
{
    unsigned char *second;
    unsigned char *third;

    func_02059e30(&data_020cf844, &data_0209d9d4, &data_020bbc18, sizeof(data_020cf844), 0, 0);
    second = (unsigned char *)&data_020cf898;
    func_02059e4c(second, &data_0209d9e4, &data_020cb04c, sizeof(data_020cf898), 0, 0);
    third = (unsigned char *)&data_020cf8b4;
    func_02059e68(third, &data_0209d9f0, second, sizeof(data_020cf8b4), 0, 0);
    func_02059e84(&data_020cf860, &data_0209d9fc, third, sizeof(data_020cf860), 0, 0);
    func_02059ea0(&data_020cf87c, &data_0209da04, third, sizeof(data_020cf87c), 0, 0);
    func_0200eb28(&data_020cf820);
    func_0200eb3c(&data_020cf824);
    func_0202f4b0(&data_020cf81c);
    func_02059ebc(&data_020cf828);
}
