void func_02011f90(char *object, int first, int second, int third)
{
    *(int *)(object + 16) = first;
    *(int *)(object + 20) = second;
    *(int *)(object + 24) = third;
}
