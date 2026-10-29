extern char data_020cf0c0[0xb4];
extern char data_0209c40c;
extern char data_020cf778;
extern char data_020cf09c;
extern char data_020cf0a0;
extern char data_020cf0a4;
void func_02052c60(void *a, void *b, void *c, int d, int e, int f);
void func_0200eb28(void *a);
void func_0200eb3c(void *a);
void func_0202f4b0(void *a);

void __sinit_020800f8(void)
{
    func_02052c60(&data_020cf0c0, &data_0209c40c, &data_020cf778, sizeof(data_020cf0c0), 0, 0);
    func_0200eb28(&data_020cf09c);
    func_0200eb3c(&data_020cf0a0);
    func_0202f4b0(&data_020cf0a4);
}
