struct ScriptMachine;

struct ScriptMachineMethods {
    void *slots[19];
    void *(*find_object)(struct ScriptMachine *machine);
};

struct ScriptMachine {
    struct ScriptMachineMethods *methods;
    unsigned short *code;
    int pc;
    int wait;
    int unknown_10;
    void *object;
};

void *func_02028148(struct ScriptMachine *machine)
{
    void *object = machine->object;
    if (object == 0) {
        object = machine->methods->find_object(machine);
    }
    return object;
}
