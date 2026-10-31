typedef int code();
extern int func_02043644(int p);
extern int func_0204372c(int p);
extern void func_020437c4(int p);

typedef struct {
    char pad[0x48];
    int *a;
    int *b;
    int *c;
    short dir;
    unsigned short idx;
} Slot;

int func_02043500(int param_1)
{
    unsigned int i;
    Slot *s;

    if (func_02043644(param_1) == 0) {
        return 0;
    }
    i = 0;
    do {
        s = (Slot *)(param_1 + i * 0x10);
        if (s->a != 0) {
            (**(code **)(*s->a + 0xd4))(s->a, 0);
        }
        if (s->b != 0) {
            (**(code **)(*s->b + 0xd4))(s->b, 0);
        }
        if (s->c != 0) {
            (**(code **)(*s->c + 0xd4))(s->c, 0);
        }
        if (s->dir != 0x100) {
            s->dir--;
        }
        if (s->dir == -2) {
            s->dir = 0x100;
            s->idx = 0xffff;
        }
        i++;
    } while (i < 5);
    if (*(int **)(param_1 + 0x44) != 0) {
        (**(code **)(**(int **)(param_1 + 0x44) + 0xd4))(*(int **)(param_1 + 0x44), 0);
    }
    func_020437c4(param_1);
    while (func_0204372c(param_1) != 0) {
    }
    return 1;
}
