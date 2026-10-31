typedef void (*Run02068958)(void *object, int value);

void func_02068958(void **object, int value)
{
    (*(Run02068958 *)*object)(object, value);
}
