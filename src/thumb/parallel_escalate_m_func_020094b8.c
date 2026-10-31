typedef int code();

extern char data_02083a94;
extern void func_0200945c();
extern int func_02007bd8(void);
extern int *func_02007b70(int heap);
extern void func_02008ea4(int buf, int size);
extern void func_02009440(int kind, int buf, int size, void (*callback)(), void *object);
extern void func_02009750(void *object);
extern void func_02009760(void *object);
extern int func_02069b94(void);
extern int func_0206a8bc(void);
extern void func_020740d0(int value);

int func_020094b8(void *object)
{
    int *heap;
    int buf;
    int result;

    func_02009750(object);
    heap = func_02007b70(func_02007bd8());
    buf = (*(code **)(*heap + 8))(heap, 0x10, &data_02083a94);
    func_02008ea4(buf, 0x10);
    func_02009440(0, buf, 0x10, func_0200945c, object);
    while (func_0206a8bc() == 0) {
        func_020740d0(0);
    }
    if (buf != 0) {
        if (func_02007b70(func_02007bd8()) != 0) {
            heap = func_02007b70(func_02007bd8());
            (*(code **)(*heap + 0x10))(heap, buf);
        }
    }
    heap = func_02007b70(func_02007bd8());
    buf = (*(code **)(*heap + 8))(heap, 0x20, &data_02083a94);
    func_02008ea4(buf, 0x20);
    func_02009440(0, buf, 0x20, func_0200945c, object);
    while (func_0206a8bc() == 0) {
        func_020740d0(0);
    }
    if (buf != 0) {
        if (func_02007b70(func_02007bd8()) != 0) {
            heap = func_02007b70(func_02007bd8());
            (*(code **)(*heap + 0x10))(heap, buf);
        }
    }
    result = func_02069b94();
    func_02009760(object);
    return result;
}
