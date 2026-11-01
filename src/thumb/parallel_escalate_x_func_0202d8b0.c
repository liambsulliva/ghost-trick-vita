typedef void ***(*Find0202d8b0)(void *self, int key);
typedef void (*Apply0202d8b0)(void *self, int value);

void func_0202d8b0(void ***object, int key, int value)
{
    void ***child = ((Find0202d8b0)(*object)[0x54 / 4])(object, key);
    if (child != 0) {
        ((Apply0202d8b0)(*child)[0x6c / 4])(child, value);
    }
}
