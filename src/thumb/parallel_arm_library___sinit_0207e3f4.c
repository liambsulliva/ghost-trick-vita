extern char data_020b9b30[4];
extern char data_020846a0;
extern char data_020bbc18;
extern char data_020b9c10[4];
extern char data_020846ac;
extern char data_020b9b4c[4];
extern char data_020846bc;
extern char data_020b9bd8[4];
extern char data_020846d0;
extern char data_020b9b84[4];
extern char data_020846e8;
extern char data_020b9bf4[4];
extern char data_02084700;
extern char data_020b9ba0[4];
extern char data_02084714;
extern char data_020b9b68[4];
extern char data_02084728;
extern char data_020b9bbc[4];
extern char data_0208473c;
void func_020144b8(void *a, void *b, void *c, int d, int e, int f);
void func_020144d4(void *a, void *b, void *c, int d, int e, int f);
void func_020144f0(void *a, void *b, void *c, int d, int e, int f);
void func_0201450c(void *a, void *b, void *c, int d, int e, int f);
void func_02014528(void *a, void *b, void *c, int d, int e, int f);
void func_02014544(void *a, void *b, void *c, int d, int e, int f);
void func_02014560(void *a, void *b, void *c, int d, int e, int f);
void func_0201457c(void *a, void *b, void *c, int d, int e, int f);
void func_02014598(void *a, void *b, void *c, int d, int e, int f);

void __sinit_0207e3f4(void)
{
    unsigned char *first;

    first = (unsigned char *)&data_020b9b30;
    func_020144b8(first, &data_020846a0, &data_020bbc18, sizeof(data_020b9b30), 0, 1);
    func_020144d4(&data_020b9c10, &data_020846ac, first, sizeof(data_020b9c10), 0, 0);
    func_020144f0(&data_020b9b4c, &data_020846bc, first, sizeof(data_020b9b4c), 0, 0);
    func_0201450c(&data_020b9bd8, &data_020846d0, first, sizeof(data_020b9bd8), 0, 0);
    func_02014528(&data_020b9b84, &data_020846e8, first, sizeof(data_020b9b84), 0, 0);
    func_02014544(&data_020b9bf4, &data_02084700, first, sizeof(data_020b9bf4), 0, 0);
    func_02014560(&data_020b9ba0, &data_02084714, first, sizeof(data_020b9ba0), 0, 0);
    func_0201457c(&data_020b9b68, &data_02084728, first, sizeof(data_020b9b68), 0, 0);
    func_02014598(&data_020b9bbc, &data_0208473c, first, sizeof(data_020b9bbc), 0, 0);
}
