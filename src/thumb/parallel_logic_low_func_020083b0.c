void func_020083b0(int *dst,int *src)
{
    int mid;

    mid = *src;
    *dst = mid;
    mid = src[1];
    src = (int *)src[2];
    dst[1] = mid;
    dst[2] = (int)src;
}
