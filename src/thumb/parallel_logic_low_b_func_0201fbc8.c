extern int func_0201f944();

int func_0201fbc8(unsigned int *param_1)
{
    unsigned int *object;
    int count;
    unsigned int i;

    object = param_1;
    count = 0;
    i = 0;
    while (i < object[2]) {
        if (func_0201f944(object,i) >= 0) {
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}
