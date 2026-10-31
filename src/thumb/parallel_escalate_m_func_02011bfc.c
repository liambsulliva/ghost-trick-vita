struct Flags02011bfc {
    unsigned int low : 11;
    unsigned int busy : 1;
    unsigned int middle : 13;
    unsigned int keep : 1;
    unsigned int high : 6;
};

extern int func_020790c8(int first, int second);
extern void func_02051cd4(int object);
extern void func_0200e5c0(void *object);
extern void func_0201f33c(void *object, int a, int b, int c);
extern void func_02011a3c(void *first, void *second);
extern void func_0201f38c(void *object);

void func_02011bfc(int object)
{
    int handle;
    unsigned char keep;
    char lock[4];
    int scope[6];

    handle = *(int *)(object + 0x1098);
    if (handle != 0) {
        keep = ((struct Flags02011bfc *)(object + 0xc))->keep;
        if (func_020790c8(object + 0x1c, object + 0x1058) != 0) {
            func_02051cd4(object + 0x1058);
        }
        func_0200e5c0(lock);
        func_0201f33c(scope, 0, 0x30000, handle);
        ((struct Flags02011bfc *)(object + 0xc))->busy = 1;
        func_02011a3c(lock, scope);
        ((struct Flags02011bfc *)(object + 0xc))->busy = 0;
        ((struct Flags02011bfc *)(object + 0xc))->keep = keep;
        func_0201f38c(scope);
    }
}
