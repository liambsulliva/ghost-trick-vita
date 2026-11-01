typedef struct Entry02043424 {
    char pad[0xe];
    unsigned short value;
} Entry02043424;

typedef struct Obj02043424 {
    char pad[0x48];
    Entry02043424 entries[5];
} Obj02043424;

Entry02043424 *func_02043424(Obj02043424 *obj)
{
    Entry02043424 *best = 0;
    unsigned int i;
    unsigned int j;
    for (i = 0; i < 5; i++) {
        if (obj->entries[i].value != 0xffff) {
            best = &obj->entries[i];
            break;
        }
    }
    for (j = 1; j < 5; j++) {
        if (obj->entries[j].value != 0xffff && best->value < obj->entries[j].value) {
            best = &obj->entries[j];
        }
    }
    return best;
}
