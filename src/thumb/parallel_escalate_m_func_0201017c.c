extern void func_0201013c(int object, int value);

void func_0201017c(int object)
{
    *(int *)(object + 0xe40) = 0;
    *(int *)(object + 0xe44) = 0;
    *(int *)(object + 0xe48) = 0;
    func_0201013c(object, 0);
}
