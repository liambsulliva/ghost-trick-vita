typedef struct {
    char pad[0xbe];
    unsigned char count;
    char pad2[5];
    int items[4];
} Owner02059040;

extern int func_02058a10(int param_1);
extern int func_02058910(int param_1);
extern void func_02059094(int param_1, Owner02059040 *param_2, unsigned int param_3);

int func_02059040(Owner02059040 *owner)
{
    int result;

    if (owner->count < 4) {
        result = func_02058a10(0x18);
        if (result != 0) {
            result = func_02058910(result);
        }
        if (result != 0) {
            func_02059094(result, owner, owner->count);
            owner->items[owner->count++] = result;
        }
        return result;
    }
    return 0;
}
