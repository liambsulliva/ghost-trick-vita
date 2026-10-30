extern int func_0202a90c(char *object, int kind);
extern void *func_0202f624(int size);
extern void *func_020619ac(void *actor);
extern void func_0202a8f4(void *actor, int kind, int slot, unsigned short e, unsigned short x, short y, int value);
extern void *func_02009900(void);
extern void func_0202f7e8(void *manager, void *actor, int a, int b);

void *func_0202a890(char *object, int kind, unsigned short x, short y, unsigned short e)
{
    int slot;
    void *actor;

    slot = func_0202a90c(object, kind);
    if (slot != 0xff) {
        actor = func_0202f624(0x70);
        if (actor != 0) {
            actor = func_020619ac(actor);
        }
        if (actor != 0) {
            func_0202a8f4(actor, kind, slot, e, x, y, *(int *)(object + slot * 12 + 0x1e8));
            func_0202f7e8(func_02009900(), actor, 1, 0);
        }
    } else {
        actor = 0;
    }
    return actor;
}
