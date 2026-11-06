#ifndef SCRIPT_OPCODES_H
#define SCRIPT_OPCODES_H

#define SCRIPT_OPCODE_COUNT 0x16b

enum script_op_family {
    SCRIPT_OP_ABSENT,
    SCRIPT_OP_CONTROL,
    SCRIPT_OP_ACTOR,
    SCRIPT_OP_VIEW,
    SCRIPT_OP_MEDIA,
    SCRIPT_OP_STORES_ONLY
};

unsigned int script_opcode_length(unsigned int op);
enum script_op_family script_opcode_family(unsigned int op);

#endif
