typedef void ***(*Find0202da10)(void *self, int key);
typedef void (*Apply0202da10)(void *self, int value);

void func_0202da10(void ***object, int key, int value)
{
    void ***child = ((Find0202da10)(*object)[0x54 / 4])(object, key);
    if (child != 0) {
        ((Apply0202da10)(*child)[0x94 / 4])(child, value);
    }
}
