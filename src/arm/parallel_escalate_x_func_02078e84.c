char *func_02078e84(char *destp, const signed char *srcp)
{
    char *retval;

    retval = destp;
    if (*srcp != 0) {
        do {
            *destp++ = (char)*srcp++;
        } while (*srcp != 0);
    }
    *destp = 0;
    return retval;
}
