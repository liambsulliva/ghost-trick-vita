struct WithMethods {
    void *methods;
};

extern char data_020cfe10[216];
extern char data_0209e1e0;
extern char data_020cf0c0;
extern char data_020cfe2c[216];
extern char data_0209e1ec;
extern char data_020cfe48[216];
extern char data_0209e1fc;
extern char data_020cff44[216];
extern char data_0209e210;
extern char data_020cfe64[224];
extern char data_0209e220;
extern char data_020cff28[216];
extern char data_0209e230;
extern char data_020cfe80[216];
extern char data_0209e248;
extern char data_020cfed4[216];
extern char data_0209e25c;
extern char data_020cfe9c[216];
extern char data_0209e26c;
extern char data_020cfef0[228];
extern char data_0209e27c;
extern char data_020cfeb8[228];
extern char data_0209e28c;
extern char data_020cff0c[228];
extern char data_0209e2a4;
struct WithMethods *func_02067868(void *object, void *a, void *b, int c, int d, int e);
struct WithMethods *func_02067884(void *object, void *a, void *b, int c, int d, int e);
struct WithMethods *func_020678a0(void *object, void *a, void *b, int c, int d, int e);
struct WithMethods *func_020678bc(void *object, void *a, void *b, int c, int d, int e);
struct WithMethods *func_020678d8(void *object, void *a, void *b, int c, int d, int e);
struct WithMethods *func_020678f4(void *object, void *a, void *b, int c, int d, int e);
struct WithMethods *func_02067910(void *object, void *a, void *b, int c, int d, int e);
struct WithMethods *func_0206792c(void *object, void *a, void *b, int c, int d, int e);
struct WithMethods *func_02067948(void *object, void *a, void *b, int c, int d, int e);
struct WithMethods *func_02067964(void *object, void *a, void *b, int c, int d, int e);
struct WithMethods *func_02067980(void *object, void *a, void *b, int c, int d, int e);
struct WithMethods *func_0206799c(void *object, void *a, void *b, int c, int d, int e);

void __sinit_02080c80(void)
{
    unsigned char *first;
    unsigned char *second;
    func_02067868(&data_020cfe10, &data_0209e1e0, &data_020cf0c0, sizeof(data_020cfe10), 0, 0);
    first = (unsigned char *)&data_020cfe2c;
    func_02067884(first, &data_0209e1ec, &data_020cfe10, sizeof(data_020cfe2c), 0, 0);
    func_020678a0(&data_020cfe48, &data_0209e1fc, first, sizeof(data_020cfe48), 0, 0);
    func_020678bc(&data_020cff44, &data_0209e210, first, sizeof(data_020cff44), 0, 0);
    func_020678d8(&data_020cfe64, &data_0209e220, first, sizeof(data_020cfe64), 0, 0);
    func_020678f4(&data_020cff28, &data_0209e230, first, sizeof(data_020cff28), 0, 0);
    func_02067910(&data_020cfe80, &data_0209e248, first, sizeof(data_020cfe80), 0, 0);
    func_0206792c(&data_020cfed4, &data_0209e25c, first, sizeof(data_020cfed4), 0, 0);
    func_02067948(&data_020cfe9c, &data_0209e26c, first, sizeof(data_020cfe9c), 0, 0);
    second = (unsigned char *)&data_020cfef0;
    func_02067964(second, &data_0209e27c, first, sizeof(data_020cfef0), 0, 0);
    func_02067980(&data_020cfeb8, &data_0209e28c, second, sizeof(data_020cfeb8), 0, 0);
    func_0206799c(&data_020cff0c, &data_0209e2a4, second, sizeof(data_020cff0c), 0, 0);
}
