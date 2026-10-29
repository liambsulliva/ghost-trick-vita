typedef int code();
extern int func_02008314(char *a,char *b);

int func_02022f74(int param_1,char *param_2)
{
    int *object;
    unsigned int i;
    int elem;
    char *item;

    object = (int *)param_1;
    for (i = 0; i < (unsigned int)object[9]; i = i + 1) {
        elem = *(int *)(param_1 + i * 4 + 4);
        item = (char *)(**(code **)(*(int *)elem + 0x10))(elem);
        if (func_02008314(item,param_2) != 0) {
            return 1;
        }
    }
    return 0;
}
