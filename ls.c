#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(int num, char *argv[])
{
    char location[100];
    int flag = 0;
    getcwd(location, sizeof(location));
    struct dirent *dir_ptr;
    DIR *dir = opendir(location);
    if (!dir)
        return 0;
    if (dir == NULL)
    {
        printf("Error no %d\n",errno );
        perror("ls");
        return 0;
    }
    if (argv[1]==NULL)
    {
        while ((dir_ptr = readdir(dir)) != NULL)
        {
            if (dir_ptr->d_name[0] != '.')
                printf("%s  ", dir_ptr->d_name);
        }
    }
    else if (strcmp(argv[1], "-a") == 0)
    {
        while ((dir_ptr = readdir(dir)) != NULL)
        {
                printf("%s  ", dir_ptr->d_name);
        }
    }
    else if (strcmp(argv[1], "-m") == 0)
    {
        while ((dir_ptr = readdir(dir)) != NULL)
        {
            if (dir_ptr->d_name[0] != '.')
                printf("%s,  ", dir_ptr->d_name);
        }
    }
    printf("\n");
    closedir(dir);
    return 0;
}