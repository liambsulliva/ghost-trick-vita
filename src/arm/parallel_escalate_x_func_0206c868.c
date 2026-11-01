typedef struct Res6c868 {
    char pad[8];
    unsigned int size;
    char pad2[0x10];
    unsigned int offset : 24;
    unsigned int flags : 8;
} Res6c868;

unsigned int func_0206c868(Res6c868 *res)
{
    if (res->flags & 1) {
        return res->offset;
    }
    return res->size;
}
