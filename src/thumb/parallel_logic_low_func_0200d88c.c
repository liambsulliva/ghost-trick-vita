extern int func_020790c8(int left,int right);

int func_0200d88c(signed char *left,signed char *right)
{
    int i;

    i = 0;
    if (left[i] != right[i]) {
        return i - 1;
    }
    return func_020790c8((int)(left + 1),(int)(right + 1));
}
