extern char data_020856b0;
extern char data_020856b8;
extern char data_020856cc;
extern char data_020bbc18;
extern char data_020caff8;
extern char data_020cb014;
extern char data_020cb030[0x28];
void func_0202fdcc(void *a, void *b, void *c, int d, int e, int f);
void func_0202fde8(void *a, void *b, void *c, int d, int e, int f);
void func_0202fe04(void *a, void *b, void *c, int d, int e, int f);

void __sinit_0207eb34(void)
{
    func_0202fdcc(&data_020cb030, &data_020856b0, &data_020bbc18, sizeof(data_020cb030), 0, 0);
    func_0202fde8(&data_020cb014, &data_020856b8, &data_020bbc18, 296, 0, 0);
    func_0202fe04(&data_020caff8, &data_020856cc, &data_020bbc18, 16, 0, 0);
}
