typedef struct Info0207dfd4 {
    unsigned int start;
    const unsigned char *table;
    const unsigned char *action;
    char *first;
    char *last;
} Info0207dfd4;

extern char __exception_table_start__[];
extern char __exception_table_end__[];

int __FindExceptionTable(Info0207dfd4 *info, unsigned int pc)
{
    info->first = __exception_table_start__;
    info->last = __exception_table_end__;
    return 1;
}
