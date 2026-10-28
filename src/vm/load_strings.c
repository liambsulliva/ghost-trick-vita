struct LmgHeader {
    int magic;
    int unknown_4;
    int code_size;
    int string_size;
};

extern char data_020ca00c;
void func_020223b8(void *table, char *strings, int size);

void func_02027bec(struct LmgHeader *file)
{
    func_020223b8(&data_020ca00c, (char *)file + 0x34 + file->code_size, file->string_size);
}
