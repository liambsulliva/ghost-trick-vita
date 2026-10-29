extern char data_020cf538;
extern char data_0209d770;
extern char data_020cb04c;
void func_02055ac8(void *a, void *b, void *c, int d, int e, int f);
extern char data_020cf500;
extern char data_0209d77c;
void func_02055ae4(void *a, void *b, void *c, int d, int e, int f);
extern char data_020cf554;
extern char data_0209d78c;
void func_02055b00(void *a, void *b, void *c, int d, int e, int f);
extern char data_020cf51c;
extern char data_0209d79c;
void func_02055b1c(void *a, void *b, void *c, int d, int e, int f);

void __sinit_020804bc(void)
{
    func_02055ac8(&data_020cf538, &data_0209d770, &data_020cb04c, 192, 0, 0);
    func_02055ae4(&data_020cf500, &data_0209d77c, &data_020cf538, 204, 0, 0);
    func_02055b00(&data_020cf554, &data_0209d78c, &data_020cf500, 204, 0, 0);
    func_02055b1c(&data_020cf51c, &data_0209d79c, &data_020cf500, 204, 0, 0);
}
