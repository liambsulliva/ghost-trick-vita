extern int func_0206c334();
extern int func_02079020(signed char *s);

int func_0206c3fc(signed char *path)
{
    int len;
    int pos;

    len = func_02079020(path);
    pos = func_0206c334(path, len);
    if (pos >= 0 && (*(unsigned char *)(path + pos) == '/' || *(unsigned char *)(path + pos) == '\\')) {
        *(unsigned char *)(path + pos) = 0;
        len = pos;
    }
    return len;
}
