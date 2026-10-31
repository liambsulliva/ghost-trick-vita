typedef int code();

typedef struct {
    int value;
    int other;
} Pair;

extern Pair data_02083a8c[];
extern Pair data_02083a90[];

extern int func_0200902c(unsigned char *p);
extern void func_02009750(unsigned char *p);
extern int func_02009440(int a, int b, int c, int d, int e);
extern int func_0206a8bc(void);
extern void func_020740d0(int queue);
extern int func_02069b94(void);
extern void func_02009760(unsigned char *p);
extern int func_02007bd8(void);
extern int *func_02007b70(int heap);
extern void func_02009378();

void func_02009388(unsigned char *p)
{
    int off;
    int base;
    int arg;
    int size;
    int ret;
    int result;
    int *heap;

    arg = *(int *)(p + 0xc);
    size = data_02083a90[p[8]].value;
    base = data_02083a8c[p[8]].value;
    off = (signed char)p[7] * size;
    ret = func_0200902c(p);
    result = 1;
    if (ret == 1) {
        func_02009750(p);
        func_02009440(base + off, arg, size, (int)func_02009378, (int)p);
        result = 0;
        while (func_0206a8bc() == 0) {
            func_020740d0(result);
        }
        result = func_02069b94();
        func_02009760(p);
    }
    if (ret != 1) {
        result = 1;
    }
    if (*(int *)(p + 0xc) != 0) {
        if (func_02007b70(func_02007bd8()) != 0) {
            heap = func_02007b70(func_02007bd8());
            (*(code **)(*heap + 0x10))(heap, *(int *)(p + 0xc));
            *(int *)(p + 0xc) = 0;
        }
    }
    if (result == 0) {
        p[6] = 1;
        return;
    }
    if (ret == 2 || result != 0) {
        p[6] = 2;
        return;
    }
    p[6] = 2;
}
