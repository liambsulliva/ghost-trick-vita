typedef struct Args {
    int a;
    int b;
    int c;
    int d;
} Args;

typedef struct Table {
    int (*f0)(void *arc, void *file, int a, int *b);
    int (*f1)(void *arc, void *file, int a, int *b);
    int (*f2)(void *arc, void *file, int a, int b);
    int (*f3)(void *arc, void *file, int a);
    int (*f4)(void *arc, int a, int b, int *c, int d);
    int (*f5)(void *arc, void *file, int a, int b, int *c);
    int (*f6)(void *arc, void *file, int a, int b);
    int (*f7)(void *arc, void *file, int b, int c, int *a);
    int (*f8)(void *arc, void *file);
    void (*f9)(void *arc);
    void (*f10)(void *arc);
    void (*f11)(void *arc);
    void (*f12)(void *arc);
    int (*f13)(void *arc, void *file, int a, int b, int c);
    int (*f14)(void *arc, void *file, int *a, int b);
    int (*f15)(void *arc, void *file, struct Args *arg);
    int (*f16)(void *arc, void *file, struct Args *arg);
    void (*f17)(void *arc);
    void (*f18)(void *arc);
    int (*f19)(void *arc, struct Args *arg);
    int (*f20)(void *arc, int a, int b, int c);
    int (*f21)(void *arc, int a, int b);
    int (*f22)(void *arc, int a, int b, int c, int d);
    int (*f23)(void *arc, int a, int b, int c);
    int (*f24)(void *arc, int a, int b, int c);
    int (*f25)(void *arc, int a, int b, int c);
    int (*f26)(void *arc, int a, int b);
    int (*f27)(void *arc, int a, int b, int c, int d);
    int (*f28)(void *arc, int a);
    int (*f29)(void *arc);
    int (*f30)(void *arc, void *file);
    int (*f31)(void *arc, void *file, int a);
    int (*f32)(void *arc, void *file, int a, int b, int c);
    int (*f33)(void *arc, void *file);
    int (*f34)(void *arc, void *file, int a, int b);
} Table;

typedef struct Archive {
    unsigned char pad[0x24];
    Table *vtbl;
} Archive;

typedef struct File {
    int pad0;
    int pad4;
    Archive *arc;
    unsigned int stat;
    Args *argument;
} File;

extern int func_0206b284(int command);
extern int func_0206b350(File *file, int result);
extern void func_0206b2ac(File *file, int result);

int func_0206b3a8(File *file, int command)
{
    Archive *arc;
    Table *table;
    Args *arg;
    int result;

    arc = file->arc;
    table = arc->vtbl;
    if ((unsigned int)command >= 35) {
        result = 4;
    } else if (((void **)table)[command] == 0) {
        result = 4;
    } else {
        switch (command) {
        case 0:
            arg = file->argument;
            result = table->f0(arc, file, arg->a, &arg->b);
            break;
        case 1:
            arg = file->argument;
            result = table->f1(arc, file, arg->a, &arg->b);
            break;
        case 2:
            arg = file->argument;
            result = table->f2(arc, file, arg->a, arg->b);
            break;
        case 3:
            arg = file->argument;
            result = table->f3(arc, file, arg->a);
            break;
        case 4:
            arg = file->argument;
            result = table->f4(arc, arg->a, arg->b, &arg->c, arg->d);
            break;
        case 5:
            arg = file->argument;
            result = table->f5(arc, file, arg->a, arg->b, &arg->c);
            break;
        case 6:
            arg = file->argument;
            result = table->f6(arc, file, arg->a, arg->b);
            break;
        case 7:
            arg = file->argument;
            result = table->f7(arc, file, arg->b, arg->c, &arg->a);
            break;
        case 8:
            result = table->f8(arc, file);
            break;
        case 9:
            table->f9(arc);
            return 0;
        case 10:
            table->f10(arc);
            return 0;
        case 11:
            table->f11(arc);
            return 0;
        case 12:
            table->f12(arc);
            return 0;
        case 13:
            arg = file->argument;
            result = table->f13(arc, file, arg->a, arg->b, arg->c);
            break;
        case 14:
            arg = file->argument;
            result = table->f14(arc, file, &arg->a, arg->b);
            break;
        case 15:
            arg = file->argument;
            result = table->f15(arc, file, arg);
            break;
        case 16:
            arg = file->argument;
            result = table->f16(arc, file, arg);
            break;
        case 17:
            table->f17(arc);
            return 0;
        case 18:
            table->f18(arc);
            return 0;
        case 19:
            arg = file->argument;
            result = table->f19(arc, arg);
            break;
        case 20:
            arg = file->argument;
            result = table->f20(arc, arg->a, arg->b, arg->c);
            break;
        case 21:
            arg = file->argument;
            result = table->f21(arc, arg->a, arg->b);
            break;
        case 22:
            arg = file->argument;
            result = table->f22(arc, arg->a, arg->b, arg->c, arg->d);
            break;
        case 23:
            arg = file->argument;
            result = table->f23(arc, arg->a, arg->b, arg->c);
            break;
        case 24:
            arg = file->argument;
            result = table->f24(arc, arg->a, arg->b, arg->c);
            break;
        case 25:
            arg = file->argument;
            result = table->f25(arc, arg->a, arg->b, arg->c);
            break;
        case 26:
            arg = file->argument;
            result = table->f26(arc, arg->a, arg->b);
            break;
        case 27:
            arg = file->argument;
            result = table->f27(arc, arg->a, arg->b, arg->c, arg->d);
            break;
        case 28:
            arg = file->argument;
            result = table->f28(arc, arg->a);
            break;
        case 30:
            result = table->f30(arc, file);
            break;
        case 31:
            arg = file->argument;
            result = table->f31(arc, file, arg->a);
            break;
        case 32:
            arg = file->argument;
            result = table->f32(arc, file, arg->a, arg->b, arg->c);
            break;
        case 33:
            result = table->f33(arc, file);
            break;
        case 34:
            arg = file->argument;
            result = table->f34(arc, file, arg->a, arg->b);
            break;
        default:
            result = 4;
            break;
        }
    }
    if (func_0206b284(command) == 0) {
        if ((file->stat & 4) != 0) {
            result = func_0206b350(file, result);
        } else if (result != 0x100) {
            func_0206b2ac(file, result);
        }
    }
    return result;
}
