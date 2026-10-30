typedef struct FSArchive FSArchive;

typedef struct FSDirPos {
    FSArchive *arc;
    unsigned short own_id;
    unsigned short index;
    unsigned int pos;
} FSDirPos;

typedef struct FSArchiveState {
    FSArchive *list;
    FSDirPos current;
} FSArchiveState;

extern FSArchiveState data_020d0bac;
extern signed char data_020d0cbc[];
extern const signed char data_020b172c[];
extern FSArchive *func_0206bb14(const signed char *name, int name_len);
extern int func_0206bc74(signed char *dst, int dst_max, const signed char *src, int src_len, int *invalid);
extern int func_0206c3c8(signed char *path, int pos);
extern int func_0206c3fc(signed char *path);

static inline int is_slash(unsigned int c)
{
    return (c == '/') || (c == '\\');
}

static inline int is_lead_byte(unsigned int c)
{
    return ((c ^ 0x20) - 0xa1) < 0x3c;
}

static inline int is_trail_byte(unsigned int c)
{
    return (c != 0x7f) && ((unsigned char)(c - 0x40) <= 0xbc);
}

static inline int next_pos(const signed char *str, int pos)
{
    return pos + 1 + is_lead_byte((unsigned char)str[pos]);
}

FSArchive *func_0206bcd4(const signed char *path, unsigned int *baseid, signed char *relpath)
{
    FSArchive *arc = 0;
    int pathlen = 0;
    int pathmax = 0x104;
    int invalid = 0;

    if (data_020d0bac.current.arc == 0) {
        data_020d0bac.current.arc = data_020d0bac.list;
        data_020d0bac.current.own_id = 0;
        data_020d0bac.current.pos = 0;
        data_020d0bac.current.index = 0;
        data_020d0cbc[0] = '\0';
    }
    if (is_slash((unsigned char)*path)) {
        arc = data_020d0bac.current.arc;
        ++path;
        if (baseid) {
            *baseid = 0;
        }
    } else {
        int i;
        for (i = 0; ; i = next_pos(path, i)) {
            unsigned int c = (unsigned char)path[i];
            if (!c || is_slash(c)) {
                arc = data_020d0bac.current.arc;
                if (baseid) {
                    *baseid = data_020d0bac.current.own_id;
                }
                if (relpath && data_020d0bac.current.own_id == 0 && data_020d0cbc[0] != '\0') {
                    pathlen += func_0206bc74(relpath + pathlen, pathmax - pathlen, data_020d0cbc, pathmax, &invalid);
                    pathlen += func_0206bc74(relpath + pathlen, pathmax - pathlen, data_020b172c, 1, &invalid);
                }
                break;
            } else if (c == ':') {
                arc = func_0206bb14(path, i);
                path += i + 1;
                if (is_slash((unsigned char)*path)) {
                    ++path;
                }
                if (baseid) {
                    *baseid = 0;
                }
                break;
            }
        }
    }
    if (relpath) {
        int pos = 0;
        while (!invalid) {
            signed char c = path[pos];
            if (c != '\0' && !is_slash((unsigned char)c)) {
                pos += (is_lead_byte(c & 0xff) && is_trail_byte(path[pos + 1])) ? 2 : 1;
            } else {
                if (pos == 0) {
                } else if (pos == 1 && path[0] == '.') {
                } else if (pos == 2 && path[0] == '.' && path[1] == '.') {
                    if (pathlen > 0) {
                        --pathlen;
                    }
                    pathlen = func_0206c3c8(relpath, pathlen) + 1;
                } else {
                    pathlen += func_0206bc74(relpath + pathlen, pathmax - pathlen, path, pos, &invalid);
                    if (c) {
                        pathlen += func_0206bc74(relpath + pathlen, pathmax - pathlen, data_020b172c, 1, &invalid);
                    }
                }
                if (!c) {
                    break;
                }
                path += pos + 1;
                pos = 0;
            }
        }
        relpath[pathlen] = '\0';
        func_0206c3fc(relpath);
    }
    if (invalid) {
        arc = 0;
    }
    return arc;
}
