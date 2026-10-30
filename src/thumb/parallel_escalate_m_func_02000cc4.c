typedef struct {
    void *archive;
    unsigned int file;
} FileId;

extern int func_0206c46c(FileId *id,const char *path);
extern void func_0206c440(void *file);
extern int func_0206c538(void *file,FileId id);

int func_02000cc4(const char *path,void *file,FileId *id)
{
    int result;

    result = func_0206c46c(id,path);
    if (result) {
        func_0206c440(file);
        result = func_0206c538(file,*id);
    }
    return result;
}
