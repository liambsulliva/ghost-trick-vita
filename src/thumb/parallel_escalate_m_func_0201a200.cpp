class Res0201a200 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual int slot2(int size, char *name);
};

typedef struct {
    int first;
    int second;
} Pair0201a200;

typedef struct {
    char pad[0x68];
    int main;
    char pad2[0x74];
    Pair0201a200 pairs[3];
} Self0201a200;

extern "C" {
extern char data_02084cd8;
extern void *func_02007bd8(void);
extern Res0201a200 *func_02007b70(void *object);
extern void func_0201a4ec(void *object);

void func_0201a200(char *self)
{
    unsigned char index;

    *(int *)(self + 0x68) = func_02007b70(func_02007bd8())->slot2(0x2044, &data_02084cd8);
    index = 0;
    do {
        *(int *)(self + index * 8 + 0xe0) = func_02007b70(func_02007bd8())->slot2(0xa00, &data_02084cd8);
        index = index + 1;
    } while (index < 3);
    func_0201a4ec(self);
}
}
