struct Field0200e478 {
    char pad[6];
    unsigned char mode;
};

signed char func_0200e478(struct Field0200e478 *field, unsigned int id)
{
    signed char result = 0;

    if (field->mode == 0) {
        switch (id) {
        case 0x2a:
        case 0x2d:
        case 0x33:
        case 0x34:
            result = 1;
            break;
        case 0xef:
        case 0xf3:
        case 0x101:
            result = 2;
            break;
        case 0xe9:
        case 0xea:
            result = 3;
            break;
        }
    } else if (field->mode == 1) {
        switch (id) {
        case 0x33:
        case 0x3c:
        case 0xe1:
        case 0xef:
            result = 1;
            break;
        case 0x2a:
        case 0x2d:
            result = 2;
            break;
        }
    } else if (field->mode == 2) {
        switch (id) {
        case 0xef:
            result += 1;
            break;
        case 0x2a:
        case 0x2d:
        case 0x33:
        case 0x34:
        case 0x3c:
            result += 2;
            break;
        }
    }
    return result;
}
