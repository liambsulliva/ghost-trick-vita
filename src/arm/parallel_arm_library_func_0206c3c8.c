extern int func_0206c334(unsigned char *path, int pos);

int func_0206c3c8(unsigned char *path, int pos)
{
    do {
        pos = func_0206c334(path, pos);
        if (pos < 0) {
            break;
        }
        if (path[pos] == '/') {
            break;
        }
    } while (path[pos] != '\\');
    return pos;
}
