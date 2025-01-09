#include <stdio.h>
#include <stdlib.h>
#define NAME_SIZE 256

typedef struct dir
{
    char name[NAME_SIZE];
    uint64_t count;
    uint64_t allocated_size;
    struct dir** children;
}Dir;

Dir* CreateDir(char* dir_name);
int AppendDir();
int Command();
void PrintPath();
int RemoveDir();
int MakeDir();

int main()
{
    Dir* origin = CreateDir("C:");

    return 0;
}

Dir* CreateDir(char* dir_name)
{
    struct dir* new_dir = (struct dir* )malloc(sizeof(struct dir*));

    if(new_dir != NULL)
    {
        strcpy(new_dir->name, dir_name);
        
    }
}
int AppendDir();
int Command();
void PrintPath();
int RemoveDir();
int MakeDir();