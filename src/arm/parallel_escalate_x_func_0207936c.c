typedef struct PrintBuf {
    unsigned int len;
    char *cur;
    char *base;
} PrintBuf;

void func_02079268(PrintBuf *p, signed char c);
void func_02079294(PrintBuf *p, signed char c, int n);
void func_020792e8(PrintBuf *p, const signed char *s, int n);

int func_0207936c(char *dst, unsigned int len, const signed char *fmt, char *ap)
{
    signed char buf[24];
    PrintBuf out;
    signed char prefix[2];
    const signed char *s;
    const signed char *start;
    int prefix_len;
    int flag;
    int width;
    int prec;
    unsigned long long val;
    unsigned long long q;
    unsigned int v;
    unsigned int vq;
    int n;
    int base;
    int xdigit;
    int c;
    int d;

    s = fmt;
    out.len = len;
    out.base = dst;
    out.cur = dst;
    while (*s != 0) {
        c = *s;
        if ((unsigned int)(((unsigned char)c ^ 0x20) - 0xa1) < 0x3c) {
            func_02079268(&out, *s);
            if (*++s != 0) {
                func_02079268(&out, *s++);
            }
        } else if (c != '%') {
            func_02079268(&out, *s++);
        } else {
            flag = 0;
            width = 0;
            start = s;
            prec = -1;
            base = 10;
            xdigit = 'a' - 10;
            for (;;) {
                c = *++s;
                switch (c) {
                case '+':
                    if (s[-1] != ' ') {
                        break;
                    }
                    flag |= 2;
                    continue;
                case ' ':
                    flag |= 1;
                    continue;
                case '-':
                    flag |= 8;
                    continue;
                case '0':
                    flag |= 0x10;
                    continue;
                }
                break;
            }
            if (c == '*') {
                width = *(int *)((ap += 4) - 4);
                s++;
                if (width < 0) {
                    width = -width;
                    flag |= 8;
                }
            } else {
                while (*s >= '0' && *s <= '9') {
                    width = width * 10 + *s++ - '0';
                }
            }
            if (*s == '.') {
                prec = 0;
                if (*++s == '*') {
                    prec = *(int *)((ap += 4) - 4);
                    s++;
                    if (prec < 0) {
                        prec = -1;
                    }
                } else {
                    while (*s >= '0' && *s <= '9') {
                        prec = prec * 10 + *s++ - '0';
                    }
                }
            }
            switch (*s) {
            case 'h':
                if (*++s != 'h') {
                    flag |= 0x40;
                } else {
                    s++;
                    flag |= 0x100;
                }
                break;
            case 'l':
                if (*++s != 'l') {
                    flag |= 0x20;
                } else {
                    s++;
                    flag |= 0x80;
                }
                break;
            }
            switch (*s) {
            case 'd':
            case 'i':
                goto put_int;
            case 'o':
                flag |= 0x1000;
                base = 8;
                goto put_int;
            case 'u':
                flag |= 0x1000;
                goto put_int;
            case 'X':
                xdigit = 'A' - 10;
                goto put_hex;
            case 'p':
                flag |= 4;
                prec = 8;
                goto put_hex;
            case 'c':
                if (prec >= 0) {
                    goto put_bad;
                }
                c = *(int *)((ap += 4) - 4);
                if (flag & 8) {
                    func_02079268(&out, (signed char)c);
                    func_02079294(&out, ' ', width - 1);
                } else {
                    func_02079294(&out, (flag & 0x10) ? '0' : ' ', width - 1);
                    func_02079268(&out, (signed char)c);
                }
                s++;
                break;
            case 's':
                {
                    int slen;
                    const signed char *str;
                    str = *(const signed char **)((ap += 4) - 4);
                    slen = 0;
                    if (prec < 0) {
                        while (str[slen] != 0) {
                            slen++;
                        }
                    } else {
                        while (slen < prec && str[slen] != 0) {
                            slen++;
                        }
                    }
                    width -= slen;
                    if (flag & 8) {
                        func_020792e8(&out, str, slen);
                        func_02079294(&out, ' ', width);
                    } else {
                        func_02079294(&out, (flag & 0x10) ? '0' : ' ', width);
                        func_020792e8(&out, str, slen);
                    }
                }
                s++;
                break;
            case '%':
                if (start + 1 != s) {
                    goto put_bad;
                }
                func_02079268(&out, *s++);
                break;
            default:
            put_bad:
                func_020792e8(&out, start, s - start);
                break;
            case 'x':
            put_hex:
                flag |= 0x1000;
                base = 16;
            put_int:
                prefix_len = 0;
                if (flag & 8) {
                    flag &= ~0x10;
                }
                if (prec >= 0) {
                    flag &= ~0x10;
                } else {
                    prec = 1;
                }
                if (flag & 0x1000) {
                    if (flag & 0x100) {
                        val = (unsigned char)*(int *)((ap += 4) - 4);
                    } else if (flag & 0x40) {
                        val = (unsigned short)*(int *)((ap += 4) - 4);
                    } else if (flag & 0x80) {
                        val = *(unsigned long long *)((ap += 8) - 8);
                    } else {
                        val = *(unsigned int *)((ap += 4) - 4);
                    }
                    flag &= ~3;
                    if (flag & 4) {
                        if (base == 16) {
                            if (val != 0) {
                                prefix[0] = xdigit + 33;
                                prefix[1] = '0';
                                prefix_len = 2;
                            }
                        } else if (base == 8) {
                            prefix[0] = '0';
                            prefix_len = 1;
                        }
                    }
                } else {
                    if (flag & 0x100) {
                        val = (signed char)*(int *)((ap += 4) - 4);
                    } else if (flag & 0x40) {
                        val = (short)*(int *)((ap += 4) - 4);
                    } else if (flag & 0x80) {
                        val = *(long long *)((ap += 8) - 8);
                    } else {
                        val = *(int *)((ap += 4) - 4);
                    }
                    if ((val >> 32) & 0x80000000) {
                        val = ~val + 1;
                        prefix[0] = '-';
                        prefix_len = 1;
                    } else if (val != 0 || prec != 0) {
                        if (flag & 2) {
                            prefix[0] = '+';
                            prefix_len = 1;
                        } else if (flag & 1) {
                            prefix[0] = ' ';
                            prefix_len = 1;
                        }
                    }
                }
                n = 0;
                switch (base) {
                case 8:
                    while (val != 0) {
                        int oct = (int)(val & 7);
                        val >>= 3;
                        buf[n++] = (char)(oct + '0');
                    }
                    break;
                case 10:
                    if ((val >> 32) == 0) {
                        v = (unsigned int)val;
                        while (v != 0) {
                            int dig;
                            vq = v / 10;
                            dig = (int)(v - vq * 10);
                            v = vq;
                            buf[n++] = (char)(dig + '0');
                        }
                    } else {
                        while (val != 0) {
                            q = val / 10;
                            buf[n++] = (int)(val - q * 10) + '0';
                            val = q;
                        }
                    }
                    break;
                case 16:
                    while (val != 0) {
                        d = (int)(val & 15);
                        val >>= 4;
                        buf[n++] = (d < 10) ? d + '0' : d + xdigit;
                    }
                    break;
                }
                if (prefix_len > 0 && prefix[0] == '0') {
                    buf[n++] = '0';
                    prefix_len = 0;
                }
                prec -= n;
                if (flag & 0x10) {
                    if (prec < width - n - prefix_len) {
                        prec = width - n - prefix_len;
                    }
                }
                if (prec > 0) {
                    width -= prec;
                }
                width -= prefix_len + n;
                if (!(flag & 8)) {
                    func_02079294(&out, ' ', width);
                }
                while (prefix_len > 0) {
                    func_02079268(&out, prefix[--prefix_len]);
                }
                func_02079294(&out, '0', prec);
                while (n > 0) {
                    func_02079268(&out, buf[--n]);
                }
                if (flag & 8) {
                    func_02079294(&out, ' ', width);
                }
                s++;
                break;
            }
        }
    }
    if (out.len != 0) {
        *out.cur = 0;
    } else if (len != 0) {
        *(out.base + len - 1) = 0;
    }
    return out.cur - out.base;
}
