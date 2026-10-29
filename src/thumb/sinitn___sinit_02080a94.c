extern char data_020cfbb8;
extern char data_0209dca0;
extern char data_020cb030;
void func_02061ca0(void *a, void *b, void *c, int d, int e, int f);
extern char data_020cfbd4;
extern char data_0209dca8;
void func_02061cbc(void *a, void *b, void *c, int d, int e, int f);

void __sinit_02080a94(void)
{
    func_02061ca0(&data_020cfbb8, &data_0209dca0, &data_020cb030, 88, 0, 0);
    func_02061cbc(&data_020cfbd4, &data_0209dca8, &data_020cfbb8, 112, 0, 0);
}
