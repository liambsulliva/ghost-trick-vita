typedef struct {
    void *handle;
    int a;
    int b;
    int c;
    int d;
    void *table;
    int size;
} State;

extern State data_020d0dc0;
extern char data_0209e344[];
extern char data_020b1730[];
unsigned short func_02075d50(void);
void *func_0206bb14(void *source, int mode);

void func_0206c7f8(void)
{
    if (func_02075d50() == 2) {
        data_020d0dc0.a = -1;
        data_020d0dc0.b = 0;
        data_020d0dc0.c = -1;
        data_020d0dc0.d = 0;
    } else {
        data_020d0dc0.a = 0;
        data_020d0dc0.b = 0;
        data_020d0dc0.c = 0;
        data_020d0dc0.d = 0;
    }
    data_020d0dc0.table = data_0209e344;
    data_020d0dc0.size = 64;
    data_020d0dc0.handle = func_0206bb14(data_020b1730, 3);
}
