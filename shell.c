#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <stdbool.h>
extern int errno;
char *line;
char **args;
int num;
size_t is = 1000;
const char *del = " \t\r\n\a";
struct stat st;
char *sp;
char *csp;
void cd()
{
    int flagP = 0;
    int flagE = 0;
    for (int a = 0; a < num; a++)
    {
        if (strcmp(args[a], "-P") == 0)
            flagP = 1;
        else if (strcmp(args[a], "-e") == 0)
            flagE = 1;
    }
    if (flagE + flagP + 2 < num)
        printf("cd: Too many arguments\n");
    else if (flagE + flagP + 1 == num)
    {
        char tmp[1024];
        if (flagE && getcwd(tmp, sizeof(tmp)) == NULL)
        {
            printf("Error no: %d", errno);
            perror("cd");
            return;
        }
        int status = chdir(sp);
        if (status == -1)
            perror("cd");
    }
    else
    {
        for (int a = 1; a < num; a++)
        {
            if (strcmp(args[a], "-P") == 0 || strcmp(args[a], "-e") == 0)
                continue;
            if (args[a][0] == '~')
            {
                char temp10[1000];
                strcpy(temp10, sp);
                strcat(temp10, args[a] + 1);
                int status = chdir(temp10);
                if (status == -1)
                    perror("cd ");
                return;
                if (strcmp(args[a+1], NULL) != 0)
                {
                    printf("cd: no such file or directory: %s", args[a+1]);
                }
            }
            char tmp[1024];
            if (flagE && getcwd(tmp, sizeof(tmp)) == NULL)
            {
                printf("Error no: %d", errno);
                perror("cd");
                return;
            }
            int status = chdir(args[a]);
            if (status == -1)
                perror("cd");
        }
    }
}
void echo()
{
    int st_ec = 0;
    if (num == 1)
    {
        st_ec = printf("\n");
        if (st_ec == -1)
            perror("echo");
    }
    else
    {
        if (strcmp(args[1], "--help") == 0)
        {
            printf("Type echo and an argument after that.\n");
        }
        else if (strcmp(args[1], "-n") == 0)
        {
        }
        else
        {
            char dest[250];
            for (int i = 1; i < num; i++)
            {
                strcat(dest, args[i]);
                strcat(dest, " ");
            }
            printf("%s", dest);
            printf("\n");
        }
    }
}
void pwd()
{
    if (num != 1)
    {
        if (strcmp(args[1], "-L") == 0 || strcmp(args[1], "-P") == 0)
        {
            char tmp2[1000];
            if (getcwd(tmp2, sizeof(tmp2)) == NULL)
            {
                printf("Error no : %d\n", errno);
                perror("pwd");
            }
            else
            {
                printf("%s\n", tmp2);
            }
        }
    }
    else if(num>2){
        printf("pwd: Too many arguments.");
    }

    else
    {
        char tmp[1000];
        if (getcwd(tmp, sizeof(tmp)) == NULL)
        {
            printf("Error no : %d\n", errno);
            perror("pwd ");
        }
        else
        {
            printf("%s\n", tmp);
        }
    }
}
int main()
{
    args = (char **)malloc(sizeof(char *) * is);
    char pathS[1000];
    char pathF[1000];
    sp = getcwd(pathS, 1000);
    printf("\n\t======================================================\n");
    printf("\t              Welcome to Mahir's shell\n");
    printf("\t========================================================\n\n");
    while (1)
    {
        csp = getcwd(pathF, 1000);
        printf("%s ~$ ", csp);
        getline(&line, &is, stdin);
        char *temp = strtok(line, del);
        num = 0;
        while (temp != NULL)
        {
            args[num] = temp;
            num++;
            temp = strtok(NULL, del);
        }
        args[num] = NULL;
        if (strcmp(args[0], "cd") == 0)
            cd();
        else if (strcmp(args[0], "echo") == 0)
            echo();
        else if (strcmp(args[0], "pwd") == 0)
            pwd();
        else if (strcmp(args[num - 1], "&t") == 0)
        {

            pthread_t tid;
            char tmp[1024];
            strcpy(tmp, sp);
            if (strcmp(args[0], "ls") == 0)
                strcat(tmp, "/ls");
            else if (strcmp(args[0], "cat") == 0)
                strcat(tmp, "/cat");
            else if (strcmp(args[0], "date") == 0)
                strcat(tmp, "/date");
            else if (strcmp(args[0], "rm") == 0)
                strcat(tmp, "/rm");
            else if (strcmp(args[0], "mkdir") == 0)
                strcat(tmp, "/mkdir");
            else
            {
                printf("Command not found\n");
                exit(0);
            }
            if (system(*args) < 0)
                perror("system");
            exit(0);
        }
        else
        {
            pid_t pid = fork();
            if (pid < 0)
                printf("Could not create a child process\n");
            else if (pid == 0)
            {
                char tmp[1024];
                strcpy(tmp, sp);
                if (strcmp(args[0], "ls") == 0)
                    strcat(tmp, "/ls");
                else if (strcmp(args[0], "cat") == 0)
                    strcat(tmp, "/cat");
                else if (strcmp(args[0], "date") == 0)
                    strcat(tmp, "/date");
                else if (strcmp(args[0], "rm") == 0)
                    strcat(tmp, "/rm");
                else if (strcmp(args[0], "mkdir") == 0)
                    strcat(tmp, "/mkdir");
                else
                {
                    printf("Command not found\n");
                    exit(0);
                }
                if (execv(tmp, args) < 0)
                    perror("execv");
                exit(0);
            }
            else if (pid > 0)
            {
                waitpid(0, NULL, 0);
            }
        }
    }
    return 0;
}