typedef struct Table41c88 {
    signed char c[0x61];
} Table41c88;

extern char data_0209b3a0[];

int func_02041c88(signed char *src, signed char *dst)
{
    Table41c88 table;
    signed char *p;

    table = *(Table41c88 *)data_0209b3a0;
    while (*src != '\0' || *src == ' ') {
        p = table.c;
        while (*p != *src) {
            p++;
            if (*p == '\0') {
                *dst = '\0';
                return 1;
            }
        }
        *dst++ = p - table.c + 0x20;
        src++;
    }
    *dst = '\0';
    return p == 0;
}
