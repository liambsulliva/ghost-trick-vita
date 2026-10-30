extern char data_020ce704[];
extern void func_020106a0(char *object);
extern void func_02010428(void *object, int value);
extern void *func_02009900(void);
extern void func_0202f7e8(void *manager, void *object, int kind, int flag);
extern void func_02010680(void *object, int value);
extern void func_020105b0(void *object, unsigned char value);
extern void *func_02044ee0(char *heap);

void func_0201062c(char *object, int a, int b, int c, int d)
{
    void *effect;
    func_020106a0(object);
    if (b == 0) {
        return;
    }
    effect = func_02044ee0(data_020ce704);
    if (effect == 0) {
        return;
    }
    func_02010680(effect, a);
    func_020105b0(effect, c);
    func_02010428(effect, d);
    func_0202f7e8(func_02009900(), effect, 8, 0);
    *(void **)(object + 0xe00) = effect;
    *(int *)(object + 0xe20) = b;
}
