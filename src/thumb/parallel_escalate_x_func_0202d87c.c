typedef void ***(*Find0202d87c)(void *self, int key);
typedef void (*Apply0202d87c)(void *self, int value);

void func_0202d87c(void ***object, int key, int value)
{
    void ***child = ((Find0202d87c)(*object)[0x54 / 4])(object, key);
    if (child != 0) {
        ((Apply0202d87c)(*child)[0x64 / 4])(child, value);
    }
}
