struct ScriptMachine {
    int header;
    unsigned short *code;
    int pc;
};

extern char data_020cf8b4;
void *func_02028148(struct ScriptMachine *machine);
int func_02008828(void *object, void *kind);
unsigned int func_020265c4(unsigned short *address);
void func_0205991c(void *object, unsigned int motion_address, unsigned int motion_id, int first_flag, unsigned char second_flag);

void func_020280c8(struct ScriptMachine *machine, unsigned int motion_address, int first_flag, unsigned char second_flag)
{
    void *object = func_02028148(machine);
    if (object != 0) {
        if (func_02008828(object, &data_020cf8b4) != 0) {
            func_0205991c(object, motion_address, func_020265c4(machine->code + (machine->pc + 1)) >> 1, first_flag, second_flag);
        }
    }
}
