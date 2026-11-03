int func_0207da40(const char *throwtype, const char *catchtype, long *offset_result)
{
    const char *t;
    const char *c;
    long offset;

    *offset_result = 0;
    if ((c = catchtype) == 0) {
        return 1;
    }
    t = throwtype;
    if (*c == 'P') {
        c++;
        if (*c == 'V') {
            c++;
        }
        if (*c == 'K') {
            c++;
        }
        if (*c == 'v') {
            if (*t == 'P' || *t == '*') {
                return 1;
            }
        }
        c = catchtype;
    }
    switch (*t) {
    case '*':
    case '!':
        if (*t++ != *c++) {
            return 0;
        }
        for (;;) {
            if (*t == *c++) {
                if (*t++ == '!') {
                    offset = 0;
                    while (*t != '!') {
                        offset = offset * 10 + *t++ - '0';
                    }
                    *offset_result = offset;
                    return 1;
                }
            } else {
                while (*t++ != '!') {
                }
                while (*t++ != '!') {
                }
                if (*t == 0) {
                    return 0;
                }
                c = catchtype + 1;
            }
        }
    }
    while ((*t == 'P' || *t == 'R') && *t == *c) {
        t++;
        c++;
        if (*c == 'K') {
            if (*t == 'K') {
                t++;
            }
            c++;
        }
        if (*t == 'K') {
            return 0;
        }
        if (*c == 'V') {
            if (*t == 'V') {
                t++;
            }
            c++;
        }
        if (*t == 'V') {
            return 0;
        }
    }
    for (; *t == *c; t++, c++) {
        if (*t == 0) {
            return 1;
        }
    }
    return 0;
}
