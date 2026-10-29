extern char data_020cfdd0;
extern char data_0209e114;
extern char data_020cb030;
void func_02065a50(void *a, void *b, void *c, int d, int e, int f);
extern char data_020cfd98;
extern char data_0209e124;
void func_02065a6c(void *a, void *b, void *c, int d, int e, int f);
extern char data_020cfdb4;
extern char data_0209e130;
void func_02065a88(void *a, void *b, void *c, int d, int e, int f);

void __sinit_02080c2c(void)
{
    func_02065a50(&data_020cfdd0, &data_0209e114, &data_020cb030, 56, 0, 0);
    func_02065a6c(&data_020cfd98, &data_0209e124, &data_020cfdd0, 124, 0, 0);
    func_02065a88(&data_020cfdb4, &data_0209e130, &data_020cfdd0, 160, 0, 0);
}
