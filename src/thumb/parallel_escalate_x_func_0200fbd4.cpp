typedef int code(...);
typedef int (*MakeFn)(int *self, void *src, unsigned int arg);

struct Quad {
    unsigned int word[4];
};

extern "C" {
extern unsigned int data_02083d7c[];
extern unsigned int data_02083d2c[];
extern unsigned char data_020cff60[];
extern unsigned int data_02083e18[];
extern unsigned int data_02083e24[];
extern unsigned int data_02083d5c[];
extern unsigned int data_02083dbc[];

void *func_02007bd8(void);
int *func_02007b70(void *p);
void func_0201ef58(void *archive);
unsigned int func_0201efb4(void *archive, void *name, int flag);
void func_0201ef74(void *archive);
void *func_0201f02c(void *archive);
void func_0201f014(void *archive, int dst, void *src);
void func_02027c98(int dst, unsigned int index);
void func_02073a8c(unsigned int buf, unsigned int fmt, ...);
}

struct Archive {
    unsigned char data[80];
    Archive() { func_0201ef58(this); }
    ~Archive() { func_0201ef74(this); }
};

extern "C" void func_0200fbd4(unsigned char *param_1, unsigned int param_2)
{
    unsigned char name[128];
    unsigned int a[4];
    unsigned int b[4];
    unsigned char *slots;
    code *fn;
    void *src;
    int *mgr;
    int dst;

    if (param_2 < 4) {
        slots = param_1 + 0x1048;
        if (*(int *)(slots + param_2 * 4) == 0) {
            *(struct Quad *)a = *(struct Quad *)data_02083d7c;
            *(struct Quad *)b = *(struct Quad *)data_02083d2c;
            fn = ((code **)data_02083d5c)[data_020cff60[0]];
            func_02073a8c((unsigned int)name, (unsigned int)data_02083e18, a[param_2], (unsigned int)fn, b[param_2], (unsigned int)data_02083e24);
            Archive arc;
            if (func_0201efb4(&arc, name, 1) == 0) {
                return;
            }
            src = func_0201f02c(&arc);
            if (param_2 != 3) {
                mgr = func_02007b70(func_02007bd8());
                dst = (*(MakeFn *)(*mgr + 8))(mgr, src, (unsigned int)data_02083dbc);
            } else {
                dst = *(int *)(param_1 + 0xee4) + 0x40000;
            }
            func_0201f014(&arc, dst, src);
            *(int *)(slots + param_2 * 4) = dst;
            if (dst == 0) {
                return;
            }
            func_02027c98(dst, param_2);
        }
    }
}
