struct ScriptMachine {
    void *methods;
    unsigned char *code;
};

char *func_02028160(struct ScriptMachine *machine, int offset)
{
    unsigned char *code = machine->code;
    return (char *)(code + *(int *)(code - 0x2c) + offset);
}
